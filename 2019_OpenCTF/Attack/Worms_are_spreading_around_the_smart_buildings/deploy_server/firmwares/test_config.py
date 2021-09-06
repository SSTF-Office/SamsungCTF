import api
from api import VacuumCleaner, MoveDirection, RotateDirection
from sys import modules as sys_modules
from os import environ as os_environ

os_environ["PYTHONPATH"] = "."
sys_modules['os'] = None
sys_modules['socket'] = None
# __builtins__.__dict__['__import__'] = None


if __name__ == '__main__':

    cleaner = VacuumCleaner()
    config = cleaner.get_config()
    print(config)
    if cleaner.send_debug_message(config):
        print("Debug message sent!")
    else:
        print("Cannot send debug message")


    print(sys_modules.keys())
