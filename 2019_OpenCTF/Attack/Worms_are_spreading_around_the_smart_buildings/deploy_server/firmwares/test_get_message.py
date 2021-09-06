from api import VacuumCleaner
import time

if __name__ == '__main__':

    cleaner = VacuumCleaner()
    i = 0
    while True:
        message = cleaner.read_input_message()
        if message:
            cleaner.send_debug_message("Received message: {}".format(message))
        else:
            cleaner.send_debug_message("No input message nr: {}".format(i))
        time.sleep(61)
        i += 1
