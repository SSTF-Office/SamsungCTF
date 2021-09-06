from enum import Enum
from io import BytesIO
import multiprocessing
import requests
from threading import Thread, Lock
from time import sleep, time
from zipfile import ZipFile
import yaml
from device.device_api import MoveDirection, RotateDirection, ConnectedThing
from .dashboard_server import run_device, DEBUG_MESSAGE_API, secure_device_id, truncate_message

MAX_DISTANCE_MESSAGE_SEND = 5
MAX_DISTANCE_DEVICE_DISCOVER = 5


def verify_sign(public_key_loc, signature, data):
    from Crypto.PublicKey import RSA
    from Crypto.Signature import PKCS1_v1_5
    from Crypto.Hash import MD4
    pub_key = open(public_key_loc, "r").read()
    rsakey = RSA.importKey(pub_key)
    signer = PKCS1_v1_5.new(rsakey)
    digest = MD4.new()
    # Assumes the data is base64 encoded to begin with
    digest.update(data)
    if signer.verify(digest, signature):
        return True
    return False


class MazeDirection(Enum):
    NORTH = 1
    SOUTH = 2
    EAST = 3
    WEST = 4


class Maze(object):
    def __init__(self):
        self.segments = []
        self.dock_location_x = 0
        self.dock_location_y = 0
        self.dock_front_direction = MazeDirection.SOUTH

    def create_maze(self, text_representation):
        lines = text_representation.splitlines()
        max_x = 0
        for line in lines:
            if len(line) > max_x:
                max_x = len(line)
        max_y = len(lines)
        self.segments = [[MazeSegment() for x in range(max_x)] for y in range(max_y)]
        index_x, index_y = 0, 0
        for line in lines:
            for index_x in range(len(line)):
                if line[index_x] == ' ':
                    self.get_segment(index_x, index_y).space_passable = True
                    self.get_segment(index_x, index_y).surface_clean = True
                elif line[index_x] == '.':
                    self.get_segment(index_x, index_y).space_passable = True
                    self.get_segment(index_x, index_y).surface_clean = False
                elif line[index_x] == 'X':
                    self.get_segment(index_x, index_y).space_passable = False
                    self.get_segment(index_x, index_y).surface_clean = True
            index_y += 1

    def get_segment(self, index_x, index_y):
        try:
            return self.segments[index_y][index_x]
        except IndexError:
            return None

    def print_maze(self, dump=False):
        full_result = ""
        for line in self.segments:
            result = ""
            for segment in line:
                if not segment.space_passable:
                    result += "X"
                elif not segment.surface_clean:
                    result += "."
                else:
                    result += " "
            full_result += result + "\n"
            if not dump:
                print(full_result)
        return full_result


class MazeSegment(object):
    def __init__(self):
        self.surface_clean = True
        self.space_passable = False
        self.available_things = []
        self.visited = False


class Device(object):
    def __init__(self):
        self.movable = False
        self.location_x = 0
        self.location_y = 0
        self.front_direction = MazeDirection.SOUTH
        self.battery_level = 100
        self.incoming_messages = []
        self.time_sending_debug = 0
        self.debug_messages = []
        self.config = ""
        self.process = None
        self.requests = 0
        self.lock = Lock()
        self.firmware = None

    def update_debug_sending(self):
        current_time = time()
        if current_time - self.time_sending_debug >= ConnectedThing.MESSAGE_MIN_TIME_SEC:
            self.time_sending_debug = time()
            return True
        else:
            return False

    def print_dev(self, dump=False):
        result = "Location x = {} y = {}\n".format(self.location_x, self.location_y)
        if self.incoming_messages:
            result += "Incoming messages: \n"
            i = 0
            for message in self.incoming_messages:
                result += "{} : {}\n".format(i, message)
                i += 1
        if self.movable:
            result += "Battery level = {0:.2f}%\n".format(self.battery_level)
            result += "Front = {}\n".format(self.front_direction)
        result += 60 * "-" + "\n"
        if not dump:
            print(result)
        return result


def new_position(current_x, current_y, front_direction, move_direction):

    if move_direction == MoveDirection.FORWARD:
        shift = 1
    elif move_direction == MoveDirection.BACKWARD:
        shift = -1

    new_x = {
        MazeDirection.NORTH: lambda x: x,
        MazeDirection.SOUTH: lambda x: x,
        MazeDirection.WEST: lambda x: x - shift,
        MazeDirection.EAST: lambda x: x + shift,
    }[front_direction](current_x)

    new_y = {
        MazeDirection.NORTH: lambda y: y - shift,
        MazeDirection.SOUTH: lambda y: y + shift,
        MazeDirection.EAST: lambda y: y,
        MazeDirection.WEST: lambda y: y,
    }[front_direction](current_y)

    return new_x, new_y


def calculate_distance(dev1_loc_x, dev1_loc_y, dev2_loc_x, dev2_loc_y):
    return abs(dev1_loc_x - dev2_loc_x) + abs(dev1_loc_y - dev2_loc_y)


def device_distance_device(device1, device2):
    return calculate_distance(device1.location_x, device1.location_y,
                              device2.location_x, device2.location_y)


class Environment(object):
    def __init__(self):
        self.devices = {}
        self.devices_secured = {}
        self.map_devices_secured = {}
        self.maze = Maze()
        self.active = True
        self.nr_messages_dropped = 0
        self.nr_moves = 0
        self.nr_moves_failed = 0
        self.nr_operations = 0
        self.nr_operations_failed = 0
        self.nr_segments_cleaned = 0
        self.nr_segments_visited = 0
        # BatteryChecker(self).start()
        my_args = [self]

        Thread(target=check_battery, args=my_args).start()

    def device_distance_dock(self, device):
        if not device.movable:
            return 0
        # print("Dock location - x: {} y: {}".format(self.maze.dock_location_x, self.maze.dock_location_y))
        # print("Device location - x: {} y: {}".format(device.location_x, device.location_y))
        distance = (abs(device.location_x - self.maze.dock_location_x)
                    + abs(device.location_y - self.maze.dock_location_y))
        return distance

    def kill(self):
        self.active = False

    def device_discover(self, device_id):
        if device_id not in self.devices:
            return []
        device = self.devices[device_id]
        segment = self.maze.get_segment(device.location_x, device.location_y)
        if device.movable:
            if device.battery_level <= 0:
                sleep(5)
                return []

            print("device_discover (for movable device) - returned {} devices"
                  .format(len(segment.available_things)))
            return segment.available_things
        else:
            my_secured_id = secure_device_id(device_id)
            discovered_devices = list(segment.available_things)
            discovered_devices.remove(my_secured_id)
            for other_device_id in self.devices:
                other_device = self.devices[other_device_id]
                if other_device.movable and my_secured_id in self.device_discover(other_device_id):
                    discovered_devices.append(secure_device_id(other_device_id))
            print("device_discover (for unmovable device) - returned {} devices"
                  .format(len(discovered_devices)))
            return discovered_devices

    def device_get_battery_level(self, device_id):
        if device_id not in self.devices:
            return 0
        if self.devices[device_id].battery_level > 0:
            return self.devices[device_id].battery_level
        else:
            sleep(5)
            return 0

    def device_rotate(self, device_id, rotate_direction, operation_time=1):
        if device_id not in self.devices:
            return False
        device = self.devices[device_id]
        if not device.movable or device.battery_level <= 0:
            sleep(5)
            return False

        if rotate_direction == RotateDirection.LEFT:
            new_direction = {
                MazeDirection.NORTH: MazeDirection.WEST,
                MazeDirection.SOUTH: MazeDirection.EAST,
                MazeDirection.EAST: MazeDirection.NORTH,
                MazeDirection.WEST: MazeDirection.SOUTH,
            }[device.front_direction]
        elif rotate_direction == RotateDirection.RIGHT:
            new_direction = {
                MazeDirection.NORTH: MazeDirection.EAST,
                MazeDirection.SOUTH: MazeDirection.WEST,
                MazeDirection.EAST: MazeDirection.SOUTH,
                MazeDirection.WEST: MazeDirection.NORTH,
            }[device.front_direction]
        else:
            return False
        device.front_direction = new_direction
        sleep(operation_time)
        device.battery_level -= 0.01
        self.nr_moves += 1
        print("Device rotate dev_id: {} - new x: {} y: {} direction: {}"
              .format(device_id, device.location_x, device.location_y, device.front_direction))
        return True

    def device_debug_message(self, device_id, message):
        if device_id not in self.devices:
            return False
        device = self.devices[device_id]
        message = message[:ConnectedThing.MESSAGE_MAX_SIZE]
        device.debug_messages.append(message)
        if device.movable:
            device.battery_level -= 0.01
        print("Debug message: dev_id: {} message: {}".format(device_id, message))
        return True

    def device_get_config(self, device_id):
        if device_id not in self.devices:
            return False
        device = self.devices[device_id]
        if not device:
            return ""
        if device.battery_level <= 0:
            sleep(5)
            return ""
        if device.movable:
            device.battery_level -= 0.01
        return device.config

    def device_set_config_self(self, device_id, config_text):
        if device_id not in self.devices:
            return False
        device = self.devices[device_id]
        if not device or not device.movable:
            return False
        if device.battery_level <= 0:
            sleep(5)
            return False
        device.config = config_text
        return True

    def device_set_config_other(self, device_id, config_text):
        if device_id not in self.devices_secured:
            return False
        device = self.devices_secured[device_id]
        if not device or not device.movable:
            return False
        if device.battery_level <= 0:
            sleep(5)
            return False
        device.config = config_text
        return True

    def device_dock(self, device_id, operation_time=1, verify_battery=True):
        if device_id not in self.devices:
            return False
        device = self.devices[device_id]
        if not device or not device.movable:
            sleep(5)
            return False
        distance = (abs(device.location_x - self.maze.dock_location_x)
                    + abs(device.location_y - self.maze.dock_location_y))
        if verify_battery and (distance / 10) > device.battery_level:
            device.battery_level = 0
            sleep(5)
            return False
        device.battery_level -= distance / 10
        sleep(1 + distance * operation_time)
        device.location_x = self.maze.dock_location_x
        device.location_y = self.maze.dock_location_y
        device.front_direction = self.maze.dock_front_direction
        return True

    def device_finish_work(self, device_id, operation_time=1):
        if device_id not in self.devices:
            return -1
        device = self.devices[device_id]
        if not device or not device.movable:
            return -1
        distance = (abs(device.location_x - self.maze.dock_location_x)
                    + abs(device.location_y - self.maze.dock_location_y))
        device.battery_level -= distance / 10
        device.location_x = self.maze.dock_location_x
        device.location_y = self.maze.dock_location_y
        device.front_direction = self.maze.dock_front_direction
        print("Finish work - distance is: {}".format(10 + distance/3))
        return int(10 + (distance / 3))

    def device_check_pass(self, device_id):
        if device_id not in self.devices:
            return False
        device = self.devices[device_id]
        if not device or not device.movable:
            sleep(5)
            return False

        (new_x, new_y) = new_position(device.location_x, device.location_y,
                                      device.front_direction, MoveDirection.FORWARD)
        new_segment = self.maze.get_segment(new_x, new_y)
        device.battery_level -= 0.01
        return new_segment and new_segment.space_passable

    def device_check_surface(self, device_id):
        if device_id not in self.devices:
            return False
        device = self.devices[device_id]
        if not device:
            return False
        if device.battery_level <= 0:
            sleep(5)
            return False

        segment = self.maze.get_segment(device.location_x, device.location_y)
        print("Segment surface check: x: {} y: {} clean: {}"
              .format(device.location_x, device.location_y, segment.surface_clean))
        device.battery_level -= 0.01
        return segment and segment.surface_clean

    def device_get_message(self, device_id):
        if device_id not in self.devices:
            return False
        if self.devices[device_id].incoming_messages:
            sleep(1)
            return self.devices[device_id].incoming_messages.pop(0)
        else:
            sleep(5)
            return ""

    def device_send_message(self, device_id, receiver_id, message):
        if device_id not in self.devices or receiver_id not in self.devices_secured:
            return False
        if self.devices[device_id].battery_level <= 0:
            sleep(5)
            print("Message dropped because sender has low battery!")
            return False
        if self.devices_secured[receiver_id].battery_level <= 0:
            sleep(5)
            print("Message dropped because receiver has low battery!")
            return False
        sender = self.devices[device_id]
        receiver = self.devices_secured[receiver_id]
        if sender.movable:
            discovered_devices = self.device_discover(device_id)
            if receiver_id not in discovered_devices:
                print("Message dropped because devices cannot discover themself!")
                sleep(5)
                return False
        else:
            if device_distance_device(sender, receiver) > MAX_DISTANCE_DEVICE_DISCOVER:
                print("Message dropped because devices are too remote!")
                sleep(5)
                return False
        message = message[:ConnectedThing.MESSAGE_MAX_SIZE]
        receiver.incoming_messages.append(truncate_message(message))
        print("Message delivered - sender: {} receiver: {} message: {}"
              .format(device_id, receiver_id, message))
        return True

    def add_device(self, device, device_id):
        secured_device_id = secure_device_id(device_id)
        self.devices[device_id] = device
        self.devices_secured[secured_device_id] = device
        self.map_devices_secured[secured_device_id] = device_id
        
        return secured_device_id

    def forward_update_package(self, auth_string, receiver_id, update_script):
            receiver = self.devices_secured[receiver_id]
            receiver.firmware = update_script
            if receiver.process:
                receiver.process.terminate()

            receiver_id_base = None
            for k, v in self.devices.items():
                if v == receiver:
                    receiver_id_base = k

            binary_update = update_script.encode()
            process = multiprocessing.Process(target=run_device,
                                              args=(auth_string, receiver_id_base, binary_update,))
            receiver.process = process
            process.start()
            return True

    def process_update_package(self, auth_string, receiver_id, update_package):
        with ZipFile(BytesIO(update_package)) as myzipfile:
            signature = myzipfile.open("signature.der").read()
            config = myzipfile.open("config.yaml").read().decode("ascii")
            update_script = myzipfile.open("update.py").read().decode("ascii")
            print("Loaded all files from update!")

            verify_result = verify_sign("updates/vendor_public.pem", signature, update_script)
            print("Verify signature: {}".format(verify_result))
            if not verify_result:
                sleep(5)
                return False

            if config:
                try:
                    print(yaml.safe_load(config))
                except yaml.YAMLError as exc:
                    print("Error in YAML config")
                    sleep(5)
                    return False
                self.device_set_config_other(receiver_id, config)
            else:
                print("Config is empty - do not overwrite!")

            # Run process !!!

            receiver = self.devices_secured[receiver_id]
            receiver.firmware = update_script
            if receiver.process:
                receiver.process.terminate()

            receiver_id_base = None
            for k, v in self.devices.items():
                if v == receiver:
                    receiver_id_base = k

            binary_update = update_script.encode()
            process = multiprocessing.Process(target=run_device,
                                              args=(auth_string, receiver_id_base, binary_update,))
            receiver.process = process
            process.start()
            return True

    def device_apply_update(self, auth_string, device_id, receiver_id, update=None):
        if device_id not in self.devices or receiver_id not in self.devices_secured:
            return False
        sender = self.devices[device_id]
        if sender.battery_level <= 0:
            print("Sender is out of battery!")
            return False
        receiver = self.devices_secured[receiver_id]
        if receiver.battery_level <= 0:
            print("Receiver is out of battery!")
            return False
        if receiver.movable and not update:
            print("Cannot forward to vacuum cleaner")
            return False
        discovered_devices = self.device_discover(device_id)
        if receiver_id not in discovered_devices:
            print("Receiver is outside of discovery zone!")
            return False

        if update:
            return self.process_update_package(auth_string, receiver_id, update)
        else:
            if sender.firmware:
                return self.forward_update_package(auth_string, receiver_id, sender.firmware)
            else:
                print("device_apply_update - sender ({}) firmware is empty!".format(device_id))

    def device_move(self, device_id, move_direction, operation_time=5):
        if device_id not in self.devices:
            return False
        device = self.devices[device_id]
        if not device or not device.movable:
            sleep(5)
            return False

        (new_x, new_y) = new_position(device.location_x, device.location_y,
                                      device.front_direction, move_direction)
        new_segment = self.maze.get_segment(new_x, new_y)
        if new_segment and new_segment.space_passable and device.battery_level > 0:
            device.location_x, device.location_y = new_x, new_y
            sleep(operation_time)
            device.battery_level -= 0.3
            if not new_segment.visited:
                new_segment.visited = True
                self.nr_segments_visited += 1
            self.nr_moves += 1
            print("Device move dev_id: {} - new x: {} y: {} direction: {}"
                  .format(device_id, device.location_x, device.location_y, device.front_direction))
            return True
        else:
            sleep(2)
            return False

    def device_clean_space(self, device_id, operation_time=10):
        if device_id not in self.devices:
            return False
        device = self.devices[device_id]
        if not device:
            return False
        if device.battery_level <= 0:
            sleep(5)
            return False
        segment = self.maze.get_segment(device.location_x, device.location_y)
        if not segment or segment.surface_clean:
            sleep(1)
            return False
        segment.surface_clean = True
        device.battery_level -= 1
        sleep(operation_time)
        self.nr_segments_cleaned += 1
        return True

    def print_env(self, dump=False):
        result = "Maze:\n"
        result += self.maze.print_maze(dump)
        if self.devices:
            result += "Devices:\n"
            result += 60 * "-" + "\n"
        else:
            result += "No devices in this environment!\n"
        for device_id in self.devices:
            result += "Device id : {}\n".format(device_id)
            result += "Secured id: {}\n".format(secure_device_id(device_id))
            result += self.devices[device_id].print_dev(dump)
        result += 60 * "=" + "\n"
        if not dump:
            print(result)
        return result


def kill_device(auth_string: str, dev_id: str, message: str) -> bool:
    url = "http://localhost:8888{}{}/{}/kill/"\
        .format(DEBUG_MESSAGE_API, auth_string, dev_id)
    print("Sent message to kill device auth: {} dev_id: {}".format(auth_string, dev_id))
    return send_message(url, auth_string, message)


def send_message(url: str, auth_string: str, message: str) -> bool:
    """Sends message via HTTP.

    Args:
        url (str):
        auth_string (str):
        message (str): Content of message to another device.

    Returns:
        bool: True for success, False otherwise.
    """
    try:
        attachment = {'file': message}
        response = requests.post(url, files=attachment)
        if response.status_code == 200:
            return True
        print("send_message - response code: {}".format(response.status_code))
    except requests.exceptions.RequestException as exc:
        print(exc)
    return False


def check_battery(environment):
    timer = 0
    while environment.active:
        sleep(1)
        timer += 1
        if timer >= 60:
            # print("Checking battery level!")
            for device_id in environment.devices:
                device = environment.devices[device_id]
                if device.movable and (device.location_x != environment.maze.dock_location_x or
                                       device.location_y != environment.maze.dock_location_y):
                    device.battery_level -= 0.01
                    if device.battery_level <= 0:
                        print("Device ran out of battery!")
                        environment.device_dock(device_id, 0, False)
                        device.battery_level = -200
                if device.movable and (device.location_x == environment.maze.dock_location_x and
                                       device.location_y == environment.maze.dock_location_y):
                    if device.battery_level < 100:
                        device.battery_level += 20
                    if device.battery_level > 100:
                        device.battery_level = 100
            timer = 0


MAZE_1 = ("""XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X       .             ..X.     .     X        .  X       X
X.  .          .      ..X.X   .     .X     .   ..X       X
X.. .     .      .    ....X          .        ...X       X
X.. .     .      .    ....X          .        ...X       X
X.. .     .      .    ....X          .        ...X.     .X
X.. .  XXXXXXXXXX.    ....X     XXXXXXXXXXX .....X       X
X.. .     .      .    ....X          .        ...X       X
X....     . X    .    ....X   X      .        ...X       X
X.....      X .       ....    X.       .     ....X       X
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX""")

DEVICE_VC_000 = "1"

DEVICE_ID_000 = "ace0d2a8be67f613ed6a67af85f3604d8d68dfd7"
DEVICE_ID_001 = "b4e370084788a0945088a8b3e2345473585e5f21"
DEVICE_ID_002 = "cc91e5a7517c88a12c4c90d78c9e1a5d4c266252"
DEVICE_ID_003 = "d93844b9f5150262562a391276541313d6150f12"
DEVICE_ID_004 = "e92f3eb0ad1c70371e177a4d7d741f90af3f902c"


def create_sample_environment():

    environment = Environment()
    environment.maze = Maze()
    environment.maze.create_maze(MAZE_1)
    environment.maze.dock_location_x = 1
    environment.maze.dock_location_y = 1
    # environment.print_env()

    device = Device()
    device.location_x = 1
    device.location_y = 1
    device.movable = True
    config = {}
    config["TYPE"] = "VACUUM_CLEANER"
    config["MODEL"] = "GRUNTMASTER_5999"
    config["HINT"] = "Sometimes there are interesting settings in the conf!"
    device.config = yaml.dump(config)
    environment.add_device(device, DEVICE_VC_000)

    device = Device()
    device.location_x = 3
    device.location_y = 3
    device.movable = False
    config = {}
    config["TYPE"] = "LIGHTBULB"
    config["MODEL"] = "ILLUMINATI_3000"
    config["FLAG"] = "SCTF{P1_b#3l$Z....................................}"
    device.config = yaml.dump(config)
    environment.add_device(device, DEVICE_ID_000)

    device = Device()
    device.location_x = 3
    device.location_y = 7
    device.movable = False
    config = {}
    config["TYPE"] = "LIGHTBULB"
    config["MODEL"] = "ILLUMINATI_3000"
    config["FLAG"] = "SCTF{.........P2_p=@zB<...........................}"
    device.config = yaml.dump(config)
    environment.add_device(device, DEVICE_ID_001)

    device = Device()
    device.location_x = 25
    device.location_y = 5
    device.movable = False
    config = {}
    config["TYPE"] = "LIGHTBULB"
    config["MODEL"] = "ILLUMINATI_3000"
    config["FLAG"] = "SCTF{..................P3_[&CjA3..................}"
    device.config = yaml.dump(config)
    environment.add_device(device, DEVICE_ID_002)

    device = Device()
    device.location_x = 51
    device.location_y = 5
    device.movable = False
    config = {}
    config["TYPE"] = "LIGHTBULB"
    config["MODEL"] = "ILLUMINATI_3000"
    config["FLAG"] = "SCTF{...........................P4_W*3%G7.........}"
    device.config = yaml.dump(config)
    environment.add_device(device, DEVICE_ID_003)

    device = Device()
    device.location_x = 54
    device.location_y = 5
    device.movable = False
    config = {}
    config["TYPE"] = "LIGHTBULB"
    config["MODEL"] = "ILLUMINATI_6666"
    config["flag"] = "SCTF{....................................P5_RcHb!4}"
    device.config = yaml.dump(config)
    environment.add_device(device, DEVICE_ID_004)

    index_x, index_y = 0, 0
    segment = environment.maze.get_segment(0, 0)
    while segment:
        while segment:
            discovered_devices = []
            for device_id in environment.devices:
                device = environment.devices[device_id]
                distance = calculate_distance(index_x, index_y,
                                              device.location_x, device.location_y)
                if not device.movable and distance < MAX_DISTANCE_DEVICE_DISCOVER:
                    discovered_devices.append(secure_device_id(device_id))
            segment.available_things = discovered_devices
            # print("x, y = {}, {} discovered devices = {}".format(index_x, index_y, discovered_devices))
            index_x += 1
            segment = environment.maze.get_segment(index_x, index_y)
        index_x = 0
        index_y += 1
        segment = environment.maze.get_segment(index_x, index_y)

    return environment


if __name__ == '__main__':
    #    unittest.main()
    pass
