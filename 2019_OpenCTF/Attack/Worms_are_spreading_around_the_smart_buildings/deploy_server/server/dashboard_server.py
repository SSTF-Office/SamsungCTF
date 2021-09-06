import copy
from enum import Enum
import multiprocessing
from threading import Thread
from flask import Flask, render_template, request, Response, send_file
import epicbox
import io
import random
import requests
import socket
import subprocess
import time
import traceback
import yaml
from Crypto.Hash import SHA256
from device.device_api import ConnectedThing


app = Flask(__name__)

PROFILES = {
    'python': {
        'docker_image': 'python_sandbox_1',
        'network_disabled': False,
    }
}

DOCKER_PS_COMMAND = "sudo docker ps --no-trunc".split()

epicbox.configure(profiles=PROFILES)
EPICBOX_LIMITS = {'cputime': 100000, 'memory': 64}
DEVICE_VC_000 = "1"
DEBUG_MESSAGE_API = '/fb68dc2d8b869785b680d6e6ff150f30bca3361b7b938385cb26fffd6d6a200d/'


DEVICE_ID_MAP_KEY = "khjasdvfakblaewfrgflnjegewngkljgfasg"


def secure_device_id(device_id):
    return SHA256.new(str.encode(device_id + DEVICE_ID_MAP_KEY)).hexdigest()


class DeviceStatus(Enum):
    ACTIVE = 0
    RETURNING_DOCK = 1
    WAITING_DOCK = 2


class UserSession(object):

    MIN_TIME_BETWEEN_REQUESTS = 10

    def __init__(self):
        self.last_request = time.time()
        self.state = DeviceStatus.WAITING_DOCK
        self.nr_firmware_uploads = 0
        self.nr_firmware_crashes = 0
        self.nr_firmware_correct_finishes = 0
        self.nr_firmware_out_of_battery = 0
        self.nr_debug_messages_delivered = 0
        self.debug_messages = []
        self.firmware_process = None
        self.wakeup_time = None
        self.last_execution_result = "No new messages."
        self.last_execution_exception = "No new exceptions."


def get_local_ip():
    """Returns IP address of local node."""
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.connect(("1.255.255.255", 80))
    local_ip = sock.getsockname()[0]
    sock.close()
    return local_ip


def active_firmware(user_id, dev_id):
    docker_ps_result = subprocess.run(DOCKER_PS_COMMAND,
                                      stdout=subprocess.PIPE).stdout.decode('utf-8')
    if docker_ps_result:
        for process in docker_ps_result.splitlines():
            if (" " + user_id + " ") in process and (" " + dev_id + " ") in process:
                return True
    return False


def device_finish_work(user_id, dev_id):
    """Returns device to docking station, when firmware finished execution.

    Returns:
        int: -1 for error, value of time that device should sleep.
    """
    try:
        url = "http://{}:8080/api/{}/{}/finish_work/".format(get_local_ip(), user_id, dev_id)
        response = requests.get(url)
        request_nr = 0
        while response.status_code != 200 and request_nr < 5:
            time.sleep(1)
            response = requests.get(url)
            request_nr += 1
        if response.status_code == 200:
            sleep_time = response.content.decode('ascii')
            print("User: {} Dev: {} | Finished work: {}".format(user_id, dev_id, sleep_time))
            return int(sleep_time)

    except requests.exceptions.RequestException as exc:
        print(exc)
    return False


def docker_kill(process_id):
    subprocess.run("sudo docker kill {}".format(process_id).split())
    subprocess.run("sudo docker rm {}".format(process_id).split())


def kill_device(user_id, dev_id=None):
    docker_ps_result = subprocess.run(DOCKER_PS_COMMAND,
                                      stdout=subprocess.PIPE).stdout.decode('utf-8')
    print("Result of ps: {}".format(docker_ps_result))
    if docker_ps_result:
        for process in docker_ps_result.splitlines():
            if not dev_id and (" " + user_id + " ") in process:
                docker_kill(process.split()[0])
            elif (" " + user_id + " ") in process and (" " + dev_id + " ") in process:
                docker_kill(process.split()[0])


def run_device(user_id, dev_id, firmware_content, q_messages=None, q_exceptions=None):
    input_files = []
    try:
        docker_ps_result = subprocess.run(DOCKER_PS_COMMAND,
                                          stdout=subprocess.PIPE).stdout.decode('utf-8')
        print("Result of ps: {}".format(docker_ps_result))
        if docker_ps_result:
            for process in docker_ps_result.splitlines():
                if (" " + user_id + " ") in process and (" " + dev_id + " ") in process:
                    docker_kill(process.split()[0])

        input_files.append({'name': 'main.py', 'content': firmware_content})
        # with open("device/device_api_implementation.py", 'rb') as file:
        with open("device/__pycache__/device_api_implementation.cpython-35.pyc", 'rb') as file:
            api_content = file.read()
            # input_files.append({'name': 'api.py', 'content': api_content})
            input_files.append({'name': 'api.pyc', 'content': api_content})
            print("File device_api_implementation.py loaded! size = {}"
                  .format(len(api_content)))

        if len(input_files) != 2:
            print(input_files)
            print("Files were not loaded!")
            exit(-1)

        result = epicbox.run('python',
                             'python3.5 main.py {} {} {} {}'
                             .format(user_id, dev_id, secure_device_id(dev_id), get_local_ip()),
                             files=input_files, limits=EPICBOX_LIMITS)
        print(result)
        if q_messages and q_exceptions:
            if result['exit_code'] != 0:
                q_messages.put((user_id, "Firmware finished with following error:\n\t{}"
                                .format(result['stderr'].decode('ascii'))))
                q_exceptions.put((user_id, str(result)))
            else:
                q_messages.put((user_id, "Firmware finished work without errors!"))
                q_exceptions.put((user_id, str(result)))

    except Exception as exc:
        exc_buffer = io.StringIO()
        traceback.print_exc(file=exc_buffer)
        if q_messages and q_exceptions:
            q_messages.put((user_id, "Firmware crashed with following exception:\n\t{}"
                            .format(exc.__class__.__name__)))
            q_exceptions.put((user_id, exc_buffer.getvalue()))
        raise exc
    finally:
        pass


def truncate_message(message):
    message = message[:ConnectedThing.MESSAGE_MAX_SIZE]
    message_list = list(message)
    i = random.randint(0, 25)
    while i < len(message):
        message_list[i] = " "
        i += random.randint(0, 25)
    message = ''.join(message_list)
    return message


@app.route('/')
def main_form():
    return render_template('upload.html')


ROBOTS_MESSAGE = """
# Please don't try to hack or DDoS the environment! 
There are no flags in this server! 
Your actions are monitored and you will be expelled from CTF!"""


@app.route('/robots.txt')
def robots_message():
    return ROBOTS_MESSAGE


@app.route('/upload')
def upload_form():
    return render_template('upload.html')


@app.route('/update_lightbulb')
def update_lightbulb_form():
    return send_file("../updates/lightbulb/update.zip", as_attachment=True)


@app.route('/update_vacuumcleaner')
def update_vacuumcleaner_form():
    return send_file("../updates/vacuumcleaner/update.zip", as_attachment=True)


@app.route('/vendor_public_key.pem')
def update_public_key():
    return send_file("../updates/vendor_public.pem", as_attachment=True)


@app.route('/vendor_private_key.pem')
def update_private_key():
    return send_file("../updates/vendor_private.pem", as_attachment=True)


@app.route('/reset', methods=['POST'])
def reset_environment():
    user = request.form['user_id']
    if user not in ctf_users:
        return "You are not allowed to run your code in our environment!"
    if user in active_sessions:
        kill_device(user, DEVICE_VC_000)
        try:
            url = "http://{}:8080/api/{}/restart/" \
                .format(get_local_ip(), user)
            response = requests.get(url)
            if response.status_code == 200:
                session = UserSession()
                active_sessions[user] = session
                return response.content.decode('ascii')
        except requests.exceptions.RequestException as exc:
            print(exc)
        return "I could not reset environment!"
    else:
        return "Environment was not started (no firmware has been started yet)!"


@app.route('/api-doc')
def display_api_doc():
    # return render_template('device_api.html')
    with open("device/html/device_api.html", 'r') as file:
        content = file.read()
        return Response(content, mimetype="text/html")


def receive_stats(auth_string: str):
    try:
        url = "http://{}:8080{}{}/{}/stats/" \
            .format(get_local_ip(), DEBUG_MESSAGE_API,
                    auth_string, DEVICE_VC_000)
        response = requests.get(url)
        if response.status_code == 200:
            return response.content.decode('ascii')
    except requests.exceptions.RequestException as exc:
        print(exc)
    return ""


@app.route('/stats')
def show_stats():
    tab_results = ("<html><b>Scoreboard of Autonomous Vacuum Cleaning Competition</b>"
                   "<head><style> td { height: 50px; width: 50px; } "
                   "#cssTable td { text-align: center; vertical-align: middle; } </style>"
                   "</head><table border='1' id='cssTable''><tr>"
                   "<th>Team</th>"
                   # "<th>Device status</th>"
                   "<th>Firmware uploads</th><th>Firmware finished without errors</th>"
                   "<th>Firmware crashes</th>"
                   "<th>Debug messages delivered</th><th>Debug messages dropped</th>"
                   "<th>Segments cleaned</th><th>Segments visited</th>"
                   "<th>Moves made</th>"
                   "<th>Total operations made</th><th>Operations failed (this run)</th>"                   
                   "</tr>")

    copy_active_sessions = copy.copy(active_sessions)

    record_cleaned = 0
    record_team = ""
    record_moves = 0
    record_ex_aequo = []

    for user in copy_active_sessions:
        session = copy_active_sessions[user]
        tab_results += "<tr><td>{}</td><td>{}</td>"\
            .format(ctf_users[user], session.nr_firmware_uploads)
        # tab_results += "<tr><td>{}</td><td>{}</td><td>{}</td>"\
        #     .format(ctf_users[user], session.state.name, session.nr_firmware_uploads)
        tab_results += "<td>{}</td><td>{}</td>" \
            .format(session.nr_firmware_correct_finishes,
                    session.nr_firmware_crashes)
        remote_stats = receive_stats(user)
        print("Received stats: {}".format(remote_stats))
        if remote_stats:
            (messages_dropped, segments_cleaned, segments_visited, moves,
             operations, operations_failed) = remote_stats.split()
            tab_results += "<td>{}</td><td>{}</td>".format(session.nr_debug_messages_delivered,
                                                           messages_dropped)
            tab_results += "<td>{}</td><td>{}</td>".format(segments_cleaned,
                                                           segments_visited)
            tab_results += "<td>{}</td>".format(moves)
            tab_results += "<td>{}</td><td>{}</td>".format(operations,
                                                           operations_failed)

            if int(segments_cleaned) > record_cleaned:
                record_cleaned = int(segments_cleaned)
                record_team = ctf_users[user]
                record_ex_aequo = []
                record_moves = int(moves)
            elif int(segments_cleaned) == record_cleaned and record_moves > int(moves):
                record_cleaned = int(segments_cleaned)
                record_team = ctf_users[user]
                record_ex_aequo = []
                record_moves = int(moves)
            elif int(segments_cleaned) == record_cleaned and record_moves == int(moves):
                record_ex_aequo.append(ctf_users[user])

        else:
            tab_results += "<td>{}</td><td>{}</td>".format(session.nr_debug_messages_delivered,
                                                           "?")
            tab_results += "<td>{}</td><td>{}</td>".format("?",
                                                           "?")
            tab_results += "<td>{}</td><td>{}</td>".format("?",
                                                           "?")
            tab_results += "<td>{}</td>".format("?")
        tab_results += "</tr>"

    tab_results += "</table>"

    if record_cleaned > 0:
        ex_equo = ""
        for team in record_ex_aequo:
            ex_equo += team + ", "
        if record_ex_aequo:
            record_team += " (in equal place with {})".format(ex_equo.strip(" ,"))

        record_results = "<br>Current winner of Autonomous Vacuum Cleaning Competition "\
                         "is <b>{}</b> with {} cleaned segments in {} moves!!!"\
            .format(record_team, record_cleaned, record_moves)
        tab_results += record_results
    tab_results += "</html>"
    return Response(tab_results, mimetype="text/html")


@app.route('/result', methods=['POST'])
def show_result():
    user = request.form['user_id']
    print("User: " + user)
    if user not in ctf_users:
        return "You are not allowed to run your code in our environment!"
    if user in active_sessions:
        session = active_sessions[user]
        if session.state == DeviceStatus.ACTIVE:
            return "Device is active (firmware is running)."
        elif session.state == DeviceStatus.RETURNING_DOCK:
            if session.wakeup_time < time.time():
                session.state = DeviceStatus.WAITING_DOCK
            else:
                return ("Device is being restarted or returned to dock "
                        "(firmware finished execution). "
                        "Result will be available after {} seconds.")\
                    .format(1 + int(session.wakeup_time - time.time()))

        last_message = session.last_execution_result
        session.last_execution_result = "Device is inactive (waiting in docking station)."
        if "stdout" in last_message:
            return truncate_message(last_message["stdout"])
        else:
            return truncate_message(str(last_message))
    else:
        return "Device is inactive (waiting in docking station)."


@app.route('/debug_message', methods=['POST'])
def show_debug_messages():
    user = request.form['user_id']
    print("User: " + user)
    if user not in ctf_users:
        return "You are not allowed to run your code in our environment!"
    if user in active_sessions and active_sessions[user].debug_messages:
        result_messages = ""
        message_nr = 1
        while active_sessions[user].debug_messages:
            message = active_sessions[user].debug_messages.pop(0)
            result_messages += "Message {} : {} <br> ".format(message_nr, message)
            message_nr += 1
        return Response("<html><body>{}</body></html>".format(result_messages), mimetype="text/html")
    else:
        return "No debug messages from your device!"


@app.route('/result_errors', methods=['POST'])
def show_result_errors():
    user = request.form['user_id']
    print("User: " + user)
    if user not in ctf_users:
        return "You are not allowed to run your code in our environment!"
    if user in active_sessions:
        session = active_sessions[user]
        last_message = session.last_execution_exception
        session.last_execution_exception = "No new errors."
        return Response(last_message, mimetype="text/plain")
    else:
        return "No errors from previous tests!"


@app.route(DEBUG_MESSAGE_API + "<string:auth_string>/", methods=['POST'])
def process_debug_message(auth_string):
    message = request.files['file'].read().decode('ascii')
    print(auth_string)
    print(message)
    if auth_string in active_sessions:
        session = active_sessions[auth_string]
        session.debug_messages.append(message)
        session.nr_debug_messages_delivered += 1
        return "True"
    else:
        return "False"


@app.route(DEBUG_MESSAGE_API + "<string:auth_string>/<string:dev_id>/kill/", methods=['POST'])
def kill_device_firmware(auth_string, dev_id):
    message = request.files['file'].read().decode('ascii')
    print("Received kill message: auth: {} dev_id: {} message: {}"
          .format(auth_string, dev_id, message))
    if auth_string in active_sessions:
        session = active_sessions[auth_string]
        kill_device(auth_string, dev_id)
        time.sleep(3)
        session.last_execution_result = message
        return "True"
    else:
        return "False"


@app.route('/uploader', methods=['POST'])
def upload_file():
    user = request.form['user_id']
    print("User: " + user)
    if user not in ctf_users:
        return "You are not allowed to run your code in our environment!"

    if user in active_sessions:
        session = active_sessions[user]
        current_time = time.time()
        if current_time - session.last_request <= UserSession.MIN_TIME_BETWEEN_REQUESTS:
            return "Slow down!!! (you can start new firmware in {} sec)"\
                .format(1 + int(UserSession.MIN_TIME_BETWEEN_REQUESTS -
                                (current_time - session.last_request)))

        if session.state == DeviceStatus.ACTIVE:
            if session.firmware_process:
                session.firmware_process.terminate()
                session.firmware_process.join()
                print('Previous process stopped!')
            kill_device(user, "1")
            time.sleep(1)

            sleep_time = device_finish_work(user, "1")
            print("User = {} | Calculated sleep time = {}".format(user, sleep_time))
            if sleep_time > 0:
                session.wakeup_time = time.time() + sleep_time
                session.state = DeviceStatus.RETURNING_DOCK
                return "Stopped execution of previous firmware. "\
                       "Device is reinitialized and returned to dock (firmware finished execution). " \
                       "You can run new firmware after {} seconds." \
                    .format(1 + int(sleep_time))
            else:
                print("Device is in dock, so it can start immediately.")
        elif session.state == DeviceStatus.RETURNING_DOCK:
            if session.wakeup_time > time.time():
                return "Device is reinitialized and returned to dock (firmware finished execution). " \
                       "You can run new firmware after {} seconds." \
                    .format(1 + int(session.wakeup_time - time.time()))

    else:
        session = UserSession()
        active_sessions[user] = session

    firmware_content = request.files['file'].read()
    if not firmware_content:
        return 'Firmware file is empty!'

    process = multiprocessing.Process(target=run_device,
                                      args=(user, "1", firmware_content,
                                            queue_messages, queue_exceptions,))
    process.start()
    session.firmware_process = process
    session.nr_firmware_uploads += 1
    session.last_request = time.time()
    session.state = DeviceStatus.ACTIVE
    return 'Device firmware started!'


def process_queue(sessions, q_messages, q_exceptions):
    while True:
        time.sleep(1)
        while not q_messages.empty():
            (user, message) = q_messages.get()
            print("Message in queue : user: {} message: {}".format(user, message))
            sleep_time = device_finish_work(user, "1")
            print("User = {} | Calculated sleep time = {}".format(user, sleep_time))
            session = sessions[user]
            session.last_execution_result = message
            if sleep_time > 0:
                session.wakeup_time = time.time() + sleep_time
                session.state = DeviceStatus.RETURNING_DOCK
            else:
                session.state = DeviceStatus.WAITING_DOCK
            if "Firmware crashed" in message:
                session.nr_firmware_crashes += 1
            elif "without errors" in message:
                session.nr_firmware_correct_finishes += 1
            elif "Firmware finished with following error" in message:
                session.nr_firmware_crashes += 1
        while not q_exceptions.empty():
            (user, exception) = q_exceptions.get()
            session = sessions[user]
            session.last_execution_exception = exception


def load_users(filename):
    try:
        with open(filename, 'r') as file:
            ctf_users_raw = file.read()
        users_list = yaml.safe_load(ctf_users_raw)
        print(users_list)
        return users_list
    except yaml.YAMLError as exc:
        print("Cannot load users")
        exit(-1)


if __name__ == '__main__':
    ctf_users = load_users("server/ctf_users.yaml")
    active_sessions = {}
    # results_processes = {}
    # exceptions_processes = {}
    queue_messages = multiprocessing.Queue()
    queue_exceptions = multiprocessing.Queue()

    Thread(target=process_queue, args=(active_sessions,
                                       queue_messages, queue_exceptions)).start()
    app.run(host='0.0.0.0', port=8888)
    # app.run(debug=True)
