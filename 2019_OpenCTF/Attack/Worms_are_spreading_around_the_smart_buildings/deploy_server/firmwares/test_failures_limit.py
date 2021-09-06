from api import VacuumCleaner, MoveDirection, RotateDirection

if __name__ == '__main__':

    cleaner = VacuumCleaner()
    cleaner.rotate(RotateDirection.RIGHT)
    while True:
        cleaner.move(MoveDirection.FORWARD)
