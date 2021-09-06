import subprocess

DOCKER_PS_COMMAND = "sudo docker ps --no-trunc".split()


def kill_device(user_id, dev_id=None):
    docker_ps_result = subprocess.run(DOCKER_PS_COMMAND,
                                      stdout=subprocess.PIPE).stdout.decode('utf-8')
    print("Result of ps: {}".format(docker_ps_result))
    if docker_ps_result:
        for process in docker_ps_result.splitlines():
            if not dev_id and (" " + user_id + " ") in process:
                print("Killed 1 process (based on user_id)")
                subprocess.run("sudo docker kill {}".format(process.split()[0]).split())
            elif not user_id and (" " + dev_id + " ") in process:
                print("Killed 1 process (based on dev_id)")
                subprocess.run("sudo docker kill {}".format(process.split()[0]).split())
            # elif (" " + user_id + " ") in process and (" " + dev_id + " ") in process:
            #     print("Killed 1 process (based on user_id and dev_id)")
            #     subprocess.run("sudo docker kill {}".format(process.split()[0]).split())


kill_device(None, "e92f3eb0ad1c70371e177a4d7d741f90af3f902c")
