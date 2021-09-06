from threading import Lock
from flask import Flask, request, make_response
import time
import yaml
from .environment import create_sample_environment, send_message, kill_device
from device.device_api import RotateDirection, MoveDirection
from .dashboard_server import DEBUG_MESSAGE_API, truncate_message, load_users


ctf_users = load_users("server/ctf_users.yaml")
app = Flask(__name__)
environments = {}


def get_environment(user_id, locking=True):
    if user_id not in environments:
        return None
    (environment, lock) = environments[user_id]
    if environment and lock:
        if not locking or lock.acquire(False):
            return environment
        else:
            print("Cannot lock environment - auth_string: {}".format(user_id))
            raise ServerException.create("Device is busy!", 503)
    else:
        return None


def release_device(environment, device_id):
    if environment:
        device = environment.devices[device_id]
        if device:
            device.lock.acquire(False)
            device.lock.release()


def create_environment(user_id):
    environment = create_sample_environment()
    environments[user_id] = (environment, Lock())


class ServerException(Exception):
    def __init___(self):
        super().__init__()
        self.message = ""
        self.error_code = 200

    @staticmethod
    def create(message_text, error_code_text):
        exception = ServerException()
        exception.message = message_text
        exception.error_code = error_code_text
        return exception


def start_request(auth_string, device_id, locking=True):
    environment = get_environment(auth_string, False)
    if not environment:
        print("Authentication error - auth_string: {} device_id: {}".format(auth_string, device_id))
        raise ServerException.create("Authentication error!", 401)
    if device_id not in environment.devices:
        environment.nr_operations_failed += 1
        print("Device not found (FAILED) - auth_string: {} device_id: {}".format(auth_string, device_id))
        raise ServerException.create("Device not found!", 404)
    device = environment.devices[device_id]
    if not locking or device.lock.acquire(False):
        return environment
    else:
        print("Cannot lock device - auth_string: {} device_id: {}".format(auth_string, device_id))
        raise ServerException.create("Device is busy!", 503)


@app.route("/api/<string:auth_string>/start/")
def api_start(auth_string):
    if auth_string not in environments:
        if auth_string not in ctf_users:
            time.sleep(10)
            return make_response("Authentication error!", 401)
        create_environment(auth_string)
        time.sleep(1)
        return "Environment started!"
    else:
        (environment, lock) = environments[auth_string]
        lock.acquire(False)
        lock.release()
        environment.nr_operations_failed = 0
        for device_id in environment.devices:
            device = environment.devices[device_id]
            device.lock.acquire(False)
            device.lock.release()
        time.sleep(10)
        return "Environment already initialized!"


@app.route("/api/<string:auth_string>/restart/")
def api_restart(auth_string):
    if auth_string not in environments:
        if auth_string not in ctf_users:
            time.sleep(10)
            return make_response("Authentication error!", 401)
    else:
        time.sleep(30)
        del environments[auth_string]
        create_environment(auth_string)
        return "Environment restarted!"


class MyAppRoute:

    def __init__(self, *args, **kwargs):
        self.args = args
        self.kwargs = kwargs

    def __call__(self, func):

        time.sleep(1)
        def wrapper(auth_string, device_id, *args, **kwargs):
            environment_locked = False
            try:
                environment = start_request(auth_string, device_id)
                environment.nr_operations += 1

                if environment.nr_operations_failed > 1000:
                    environment.nr_operations_failed = 0
                    kill_device(auth_string, device_id,
                                "Device turned off due to large number of failed requests.")
                    raise ServerException.create("Device turned off due to due to "
                                                 "large number of failed requests!", 404)
                if environment.device_get_battery_level(device_id) <= 0:
                    kill_device(auth_string, device_id,
                                "Device turned off due to lack of electricity in the battery.")
                    raise ServerException.create("Device turned off due to due to "
                                                 "lack of electricity in the battery!", 404)
                environment_locked = True
                return func(auth_string, device_id, environment, *args, **kwargs)
            except ServerException as exception:
                time.sleep(5)
                return make_response(exception.message, exception.error_code)
            finally:
                if environment_locked and environment:
                    release_device(environment, device_id)

        wrapper.__name__ = func.__name__
        new_route = app.route(*self.args, **self.kwargs)
        return new_route(wrapper)


class MyAppRouteNonLocking:

    def __init__(self, *args, **kwargs):
        self.args = args
        self.kwargs = kwargs

    def __call__(self, func):

        def wrapper(auth_string, device_id, *args, **kwargs):
            try:
                environment = start_request(auth_string, device_id, locking=False)
                return func(auth_string, device_id, environment, *args, **kwargs)
            except ServerException as exception:
                print(exception)
                return make_response(exception.message, exception.error_code)

        wrapper.__name__ = func.__name__
        new_route = app.route(*self.args, **self.kwargs)
        return new_route(wrapper)


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/discover/")
def api_discover(auth_string, device_id, environment):
    list_devices = environment.device_discover(device_id)
    return yaml.dump(list_devices)


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/get_battery/")
def api_get_battery(auth_string, device_id, environment):
    return str(environment.device_get_battery_level(device_id))


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/get_brightness/")
def api_get_brightness(auth_string, device_id, environment):
    return environment.device_get_battery_level(device_id)


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/get_color/")
def api_get_color(auth_string, device_id, environment):
    return environment.device_get_battery_level(device_id)


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/get_config/")
def api_get_config(auth_string, device_id, environment):
    return environment.device_get_config(device_id)


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/set_config/", methods=['POST'])
def api_set_config(auth_string, device_id, environment):
    if request.method == "POST":
        config = request.files['file'].read().decode('ascii')
        if environment.device_set_config_self(device_id, config):
            return "True"
    print("set_config (FAILED) - auth_string: {} device_id: {}".format(auth_string, device_id))
    environment.nr_operations_failed += 1
    return "False"


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/read_message/")
def api_read_message(auth_string, device_id, environment):
    return environment.device_get_message(device_id)


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/send_message/<string:receiver_id>/",
            methods=['POST'])
def api_send_message(auth_string, device_id, environment, receiver_id):
    message = request.files['file'].read().decode('ascii')
    if environment.device_send_message(device_id, receiver_id, message):
        return "True"
    else:
        print("send_message (FAILED) - auth_string: {} device_id: {}".format(auth_string, device_id))
        environment.nr_operations_failed += 1
        return "False"


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/send_update/<string:receiver_id>/",
            methods=['POST'])
def api_send_update(auth_string, device_id, environment, receiver_id):
    update = request.files['file'].read()
    if environment.device_apply_update(auth_string, device_id, receiver_id, update):
        return "True"
    else:
        print("send_update (FAILED) - auth_string: {} device_id: {}".format(auth_string, device_id))
        environment.nr_operations_failed += 1
        return "False"


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/forward_update/<string:receiver_id>/",
            methods=['POST'])
def api_forward_update(auth_string, device_id, environment, receiver_id):
    if environment.device_apply_update(auth_string, device_id, receiver_id):
        return "True"
    else:
        print("forward_update (FAILED) - auth_string: {} device_id: {}".format(auth_string, device_id))
        environment.nr_operations_failed += 1
        return "False"


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/move/<string:direction>/")
def api_move(auth_string, device_id, environment, direction):
    if direction == "FORWARD":
        move_direction = MoveDirection.FORWARD
    elif direction == "BACKWARD":
        move_direction = MoveDirection.BACKWARD
    else:
        environment.nr_operations_failed += 1
        return make_response("move (FAILED) - direction error (must be FORWARD or BACKWARD)!", 400)
    if environment.device_move(device_id, move_direction):
        return "True"
    else:
        print("move (FAILED) - auth_string: {} device_id: {}".format(auth_string, device_id))
        environment.nr_operations_failed += 1
        return "False"


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/rotate/<string:direction>/")
def api_rotate(auth_string, device_id, environment, direction):
    if direction == "LEFT":
        rotate_direction = RotateDirection.LEFT
    elif direction == "RIGHT":
        rotate_direction = RotateDirection.RIGHT
    else:
        return make_response("Direction error (must be LEFT or RIGHT)!", 400)
    if environment.device_rotate(device_id, rotate_direction):
        return "True"
    else:
        print("rotate (FAILED) - auth_string: {} device_id: {}".format(auth_string, device_id))
        environment.nr_operations_failed += 1
        return "False"


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/get_surface/")
def api_get_surface(auth_string, device_id, environment):
    if environment.device_check_surface(device_id):
        # print("api_get_surface True - auth_string: {} device_id: {}".format(auth_string, device_id))
        return "True"
    else:
        # print("api_get_surface False - auth_string: {} device_id: {}".format(auth_string, device_id))
        return "False"


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/check_space/")
def api_check_space(auth_string, device_id, environment):
    if environment.device_check_pass(device_id):
        return "True"
    else:
        return "False"


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/return_dock/")
def api_return_dock(auth_string, device_id, environment):
    if environment.device_dock(device_id):
        return "True"
    else:
        print("return_dock (FAILED) - auth_string: {} device_id: {}".format(auth_string, device_id))
        environment.nr_operations_failed += 1
        return "False"


@MyAppRouteNonLocking("/api/<string:auth_string>/<string:device_id>/finish_work/")
def api_return_dock_fast(auth_string, device_id, environment):
    return str(environment.device_finish_work(device_id))


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/send_debug/", methods=['POST'])
def api_send_debug(auth_string, device_id, environment):
    message = request.files['file'].read().decode('ascii')
    message = truncate_message(message)
    url = "{}{}{}/".format("http://localhost:8888", DEBUG_MESSAGE_API, auth_string)
    if device_id in environment.devices:
        device = environment.devices[device_id]
        if environment.device_distance_dock(device) > 5:
            print("Message dropped because device is too far from dock! "
                  "auth_string: {} device_id: {} distance: {} message: {}"
                  .format(auth_string, device_id,
                          environment.device_distance_dock(device), message))
            environment.nr_messages_dropped += 1
            return "False"
        if device.update_debug_sending():
            send_message(url, auth_string, message)
            if environment.device_debug_message(device_id, message):
                return "True"
        else:
            print("Message dropped because sent too fast! "
                  "auth_string: {} device_id: {} message: {}"
                  .format(auth_string, device_id, message))
            environment.nr_messages_dropped += 1
    print("send_debug (FAILED) - auth_string: {} device_id: {}".format(auth_string, device_id))
    environment.nr_operations_failed += 1
    return "False"


@MyAppRoute("/api/<string:auth_string>/<string:device_id>/clean/")
def api_clean(auth_string, device_id, environment):
    if environment.device_clean_space(device_id):
        return "Surface clean!"
    else:
        print("clean (FAILED) - auth_string: {} device_id: {}".format(auth_string, device_id))
        environment.nr_operations_failed += 1
        return make_response("Cannot clean this space!", 405)


@MyAppRouteNonLocking(DEBUG_MESSAGE_API + "<string:auth_string>/<string:device_id>/stats/")
def api_stats(auth_string, device_id, environment):
    print("stats auth: {}, device: {}".format(auth_string, device_id))
    return "{} {} {} {} {} {}".format(environment.nr_messages_dropped, environment.nr_segments_cleaned,
                                      environment.nr_segments_visited, environment.nr_moves,
                                      environment.nr_operations, environment.nr_operations_failed)


# @MyAppRoute("/api/<string:auth_string>/<string:device_id>/state/")
# def api_state(auth_string, device_id, environment):
#     return environment.print_env(dump=True)


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8080)
