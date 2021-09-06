import random
import time
from api import VacuumCleaner, MoveDirection, RotateDirection

if __name__ == '__main__':

    cleaner = VacuumCleaner()

    while True:
        if not cleaner.check_surface():
            cleaner.clean_surface()
        choice = random.randint(0, 20)
        if choice == 0:
            result = cleaner.rotate(RotateDirection.RIGHT)
        elif choice == 1:
            cleaner.rotate(RotateDirection.LEFT)
        while cleaner.check_passability() and choice > 2:
            choice -= 1
            cleaner.move(MoveDirection.FORWARD)
            if not cleaner.check_surface():
                cleaner.clean_surface()
        if cleaner.get_battery_level() < 50:
            cleaner.return_dock()
            cleaner.send_debug_message("VC: RETURN DOCK")
            while cleaner.get_battery_level() < 50:
                time.sleep(60)
                cleaner.send_debug_message("VC: CHARGING - battery: {}".format(cleaner.get_battery_level()))
