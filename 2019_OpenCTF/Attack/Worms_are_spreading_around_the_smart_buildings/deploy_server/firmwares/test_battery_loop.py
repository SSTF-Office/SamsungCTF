from api import VacuumCleaner, MoveDirection, RotateDirection

if __name__ == '__main__':
    cleaner = VacuumCleaner()
    cleaner.move(MoveDirection.FORWARD)

    while True:
        cleaner.move(MoveDirection.FORWARD)
        cleaner.rotate(RotateDirection.LEFT)
        cleaner.move(MoveDirection.FORWARD)
        cleaner.rotate(RotateDirection.LEFT)
        cleaner.move(MoveDirection.FORWARD)
        cleaner.rotate(RotateDirection.LEFT)
        cleaner.move(MoveDirection.FORWARD)
        cleaner.rotate(RotateDirection.LEFT)
        cleaner.move(MoveDirection.FORWARD)
        cleaner.rotate(RotateDirection.LEFT)
        cleaner.move(MoveDirection.FORWARD)
        cleaner.rotate(RotateDirection.LEFT)
        cleaner.move(MoveDirection.FORWARD)
        cleaner.rotate(RotateDirection.LEFT)
        cleaner.move(MoveDirection.FORWARD)
        cleaner.rotate(RotateDirection.LEFT)
        cleaner.send_debug_message("Battery level = {}".format(cleaner.get_battery_level()))
