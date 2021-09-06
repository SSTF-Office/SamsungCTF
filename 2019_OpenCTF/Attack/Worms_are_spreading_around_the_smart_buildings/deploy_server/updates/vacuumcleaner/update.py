import random
import time
from api import ConnectedThing, VacuumCleaner, MoveDirection, RotateDirection

if __name__ == '__main__':

    cleaner = VacuumCleaner()
    cleaner.rotate(RotateDirection.RIGHT)
    cleaner.rotate(RotateDirection.RIGHT)
    cleaner.rotate(RotateDirection.RIGHT)
    cleaner.rotate(RotateDirection.RIGHT)
    cleaner.send_debug_message("VC: STARTED")

    while True:
        time.sleep(30)
        if not cleaner.check_surface():
            cleaner.clean_surface()
        choice = random.randint(0, 2)
        if choice == 0:
            cleaner.send_debug_message("VC: GOING RIGHT")
            cleaner.rotate(RotateDirection.RIGHT)
        elif choice == 1:
            cleaner.rotate(RotateDirection.LEFT)
            cleaner.send_debug_message("VC: GOING LEFT")
        if cleaner.check_passability():
            cleaner.move(MoveDirection.FORWARD)
            cleaner.send_debug_message("VC: GOING FORWARD")
        if cleaner.get_battery_level() < 50:
            cleaner.return_dock()
            cleaner.send_debug_message("VC: CHARGING")
            while cleaner.get_battery_level() < 50:
                time.sleep(60)
