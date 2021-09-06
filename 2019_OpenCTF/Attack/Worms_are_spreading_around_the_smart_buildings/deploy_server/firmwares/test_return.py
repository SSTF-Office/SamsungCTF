import time
from api import VacuumCleaner, MoveDirection, RotateDirection

if __name__ == '__main__':
    cleaner = VacuumCleaner()
    cleaner.move(MoveDirection.FORWARD)
    cleaner.move(MoveDirection.BACKWARD)

    current_time = time.time()
    cleaner.return_dock()
    cleaner.send_debug_message("Time from dock = {}".format(time.time() - current_time))

    time.sleep(60)
    cleaner.move(MoveDirection.FORWARD)
    current_time = time.time()
    cleaner.return_dock()
    cleaner.send_debug_message("Time from 1 place  = {}".format(time.time() - current_time))

    time.sleep(60)
    cleaner.move(MoveDirection.FORWARD)
    cleaner.move(MoveDirection.FORWARD)
    current_time = time.time()
    cleaner.return_dock()
    cleaner.send_debug_message("Time from 2 place  = {}".format(time.time() - current_time))

    time.sleep(60)
    cleaner.move(MoveDirection.FORWARD)
    cleaner.move(MoveDirection.FORWARD)
    cleaner.move(MoveDirection.FORWARD)
    current_time = time.time()
    cleaner.return_dock()
    cleaner.send_debug_message("Time from 3 place  = {}".format(time.time() - current_time))

    time.sleep(60)
    cleaner.move(MoveDirection.FORWARD)
    cleaner.move(MoveDirection.FORWARD)
    cleaner.move(MoveDirection.FORWARD)
    cleaner.move(MoveDirection.FORWARD)
    current_time = time.time()
    cleaner.return_dock()
    cleaner.send_debug_message("Time from 4 place  = {}".format(time.time() - current_time))
