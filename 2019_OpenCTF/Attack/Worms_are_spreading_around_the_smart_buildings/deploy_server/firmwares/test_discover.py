from api import VacuumCleaner, MoveDirection
from time import sleep

if __name__ == '__main__':

    cleaner = VacuumCleaner()
    cleaner.send_debug_message("My device ID: " + cleaner.get_dev_id())
    sleep(61)
    cleaner.send_debug_message("Discovered things (1): {}".format(cleaner.discover_things()))
    cleaner.move(MoveDirection.FORWARD)
    sleep(61)
    cleaner.send_debug_message("Discovered things (2): {}".format(cleaner.discover_things()))
