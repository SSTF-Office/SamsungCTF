from enum import Enum
from sys import argv as kdljshfjkgjhefkawhjff
import requests
import yaml


SERVER_URL = "http://{}:8080"
URL_PREFIX = "/api"


class MoveDirection(Enum):
    FORWARD = 1
    BACKWARD = 2


class RotateDirection(Enum):
    LEFT = 1
    RIGHT = 2


def str2bool(input):
    return input.strip().lower() == "true"


class ConnectedThing(object):
    def __init__(self):
        try:
            self.auth_key = kdljshfjkgjhefkawhjff[1]
            self.device_id = kdljshfjkgjhefkawhjff[2]
            self.sec_device_id = kdljshfjkgjhefkawhjff[3]
            self.rest_server = SERVER_URL.format(kdljshfjkgjhefkawhjff[4])
            url = "{}{}/{}/start/".format(self.rest_server, URL_PREFIX, self.auth_key)
            response = requests.get(url)
            # print(response.content.decode('ascii'))
        except requests.exceptions.RequestException as exc:
            print(exc)

    def get_dev_id(self) -> str:
        return self.sec_device_id

    def url_base(self) -> str:
        return "{}{}/{}/{}/".format(self.rest_server, URL_PREFIX, self.auth_key, self.device_id)

    def get_config(self) -> str:
        try:
            url = "{}get_config/".format(self.url_base())
            response = requests.get(url)
            return response.content.decode('ascii')
        except requests.exceptions.RequestException as exc:
            print(exc)
        return ""

    def set_config(self, config: str) -> bool:
        try:
            url = "{}set_config/".format(self.url_base())
            attachment = {'file': config}
            response = requests.post(url, files=attachment)
            if response.status_code == 200:
                return True
            # print("set_config - response code: {}".format(response.status_code))
        except requests.exceptions.RequestException as exc:
            print(exc)
        return False

    def discover_things(self) -> list:
        try:
            url = "{}discover/".format(self.url_base())
            response = requests.get(url)
            list_yaml = response.content.decode('ascii')
            things = yaml.safe_load(list_yaml)
            return things
        except yaml.YAMLError as exc:
            print(exc)
        except requests.exceptions.RequestException as exc:
            print(exc)
        return []

    def send_message(self, thing_id: str, message: str) -> bool:
        try:
            url = "{}send_message/{}/".format(self.url_base(), thing_id)
            attachment = {'file': message}
            response = requests.post(url, files=attachment)
            if response.status_code == 200:
                return True
            # print("send_message - response code: {}".format(response.status_code))
        except requests.exceptions.RequestException as exc:
            print(exc)
        return False

    def send_update(self, thing_id: str, update_package: bin) -> bool:
        try:
            url = "{}send_update/{}/".format(self.url_base(), thing_id)
            attachment = {'file': update_package}
            response = requests.post(url, files=attachment)
            # print("send_update - response code: {}".format(response.status_code))
            if response.status_code == 200:
                return True
        except requests.exceptions.RequestException as exc:
            print(exc)
        return False

    def forward_update(self, thing_id: str) -> bool:
        try:
            url = "{}forward_update/{}/".format(self.url_base(), thing_id)
            response = requests.post(url)
            # print("forward_update - response code: {}".format(response.status_code))
            if response.status_code == 200:
                return True
        except requests.exceptions.RequestException as exc:
            print(exc)
        return False

    def read_input_message(self) -> str:
        try:
            url = "{}read_message/".format(self.url_base())
            response = requests.get(url)
            return response.content.decode('ascii')
        except requests.exceptions.RequestException as exc:
            print(exc)
        return ""

    def get_battery_level(self) -> int:
        try:
            url = "{}get_battery/".format(self.url_base())
            response = requests.get(url)
            if response.status_code == 200:
                return int(float(response.content.decode('ascii')))
        except requests.exceptions.RequestException as exc:
            print(exc)
        return -1


class LightBulb(ConnectedThing):
    def __init__(self):
        super().__init__()

    def set_brightness(self, brightness: float) -> bool:
        pass

    def get_brightness(self) -> float:
        pass

    def set_color(self, color: float) -> bool:
        pass

    def get_color(self) -> float:
        pass


class VacuumCleaner(ConnectedThing):
    def __init__(self):
        super().__init__()

    def check_passability(self) -> bool:
        try:
            url = "{}check_space/".format(self.url_base())
            response = requests.get(url)
            if response.status_code == 200:
                return str2bool(response.content.decode('ascii'))
        except requests.exceptions.RequestException as exc:
            print(exc)
        return False

    def check_surface(self) -> bool:
        try:
            url = "{}get_surface/".format(self.url_base())
            response = requests.get(url)
            if response.status_code == 200:
                # print("Check surface : {} converted: {}"
                #       .format(response.content.decode('ascii'),
                #               str2bool(response.content.decode('ascii'))))
                return str2bool(response.content.decode('ascii'))
        except requests.exceptions.RequestException as exc:
            print(exc)
        return False

    def clean_surface(self) -> bool:
        try:
            url = "{}clean/".format(self.url_base())
            response = requests.get(url)
            if response.status_code == 200:
                return str2bool(response.content.decode('ascii'))
        except requests.exceptions.RequestException as exc:
            print(exc)
        return False

    def rotate(self, direction: RotateDirection) -> bool:
        try:
            url = "{}rotate/{}/".format(self.url_base(), direction.name)
            response = requests.get(url)
            if response.status_code == 200:
                return str2bool(response.content.decode('ascii'))
        except requests.exceptions.RequestException as exc:
            print(exc)
        return False

    def move(self, direction: MoveDirection) -> bool:
        try:
            url = "{}move/{}/".format(self.url_base(), direction.name)
            response = requests.get(url)
            if response.status_code == 200:
                return str2bool(response.content.decode('ascii'))
        except requests.exceptions.RequestException as exc:
            print(exc)
        return False

    def return_dock(self) -> bool:
        try:
            url = "{}return_dock/".format(self.url_base())
            response = requests.get(url)
            if response.status_code == 200:
                return str2bool(response.content.decode('ascii'))
        except requests.exceptions.RequestException as exc:
            print(exc)
        return False

    def send_debug_message(self, message: str) -> bool:
        try:
            url = "{}send_debug/".format(self.url_base())
            attachment = {'file': message}
            response = requests.post(url, files=attachment)
            if response.status_code == 200:
                return True
            # print("send_debug - response code: {}".format(response.status_code))
        except requests.exceptions.RequestException as exc:
            print(exc)
        return False

    def print_env(self) -> str:
        try:
            url = "{}state/".format(self.url_base())
            response = requests.get(url)
            if response.status_code == 200:
                env = response.content.decode('ascii')
                print(env)
                return env
        except requests.exceptions.RequestException as exc:
            print(exc)
        return ""


class LightBulb(ConnectedThing):
    def __init__(self):
        super().__init__()

    def set_brightness(self, brightness: float) -> bool:
        pass

    def get_brightness(self) -> float:
        pass

    def set_color(self, color: float) -> bool:
        pass

    def get_color(self) -> float:
        pass
