from api import LightBulb
from time import sleep

if __name__ == '__main__':
    bulb = LightBulb()
    config = bulb.get_config()
    bulb.set_brightness = 1
    bulb.set_brightness = 0
    if config and "ALWAYS_TURN_ON" in config:
        bulb.set_brightness = 1
        exit()
    while True:
        sleep(60)
        message = bulb.read_input_message()
        if message:
            if "TURN_ON" in message:
                bulb.set_brightness = 1
            elif "TURN_OFF" in message:
                bulb.set_brightness = 0

