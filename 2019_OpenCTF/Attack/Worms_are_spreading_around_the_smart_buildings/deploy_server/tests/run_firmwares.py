from os import walk
import requests
import sys
import time
import yaml


def upload_firmware(auth_string, firmware_file, server_url):
    upload_url = "http://{}/uploader".format(server_url)
    with open("firmwares/" + firmware_file, 'r') as file:
        firmware_content = file.read()
    data = dict(user_id=auth_string)
    attachment = {'file': firmware_content}
    response = requests.post(upload_url, data=data, files=attachment)
    response_content = response.content.decode('ascii')
    if response.status_code != 200:
        print("Upload error auth_string: {} firmware: {} - Server returned status: {}"
              .format(auth_string, firmware_file, response.status_code))
        return False
    elif "Device firmware started!" in response_content:
        print("Firmware uploaded auth_string: {} firmware: {}"
              .format(auth_string, firmware_file))
    elif "You can run new firmware after" in response_content:
        sub_content = response_content[response_content.index("You can run new firmware after"):]
        timeout = int(sub_content.split()[6]) + 1
        print("Sleeping {} seconds and trying to resend".format(timeout))
        time.sleep(timeout)
        upload_firmware(auth_string, firmware_file, server_url)
    else:
        print(response_content)


if __name__ == '__main__':
    with open("server/ctf_users.yaml", 'r') as file:
        ctf_users_raw = file.read()
    ctf_users = list(yaml.safe_load(ctf_users_raw).keys())
    print("Available users: {}".format(ctf_users))

    (_, _, firmwares) = next(walk("firmwares"))
    print("Available firmwares: {}".format(firmwares))

    if len(sys.argv) == 2:
        print("Using SNIPER mode - one firmware for one user")
        number = 0
        while number < len(ctf_users) and number < len(firmwares):
            upload_firmware(ctf_users[number], firmwares[number], sys.argv[1])
            number += 1

    elif len(sys.argv) == 3:
        print("Using BATTERING RAM mode - one firmware {} on all users".format(sys.argv[2]))
        number = 0
        while number < len(ctf_users):
            upload_firmware(ctf_users[number], sys.argv[2], sys.argv[1])
            number += 1
    else:
        print("Bad number of input params!\n")
