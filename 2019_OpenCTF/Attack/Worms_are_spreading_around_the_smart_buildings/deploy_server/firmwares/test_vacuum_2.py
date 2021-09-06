from api import VacuumCleaner, MoveDirection, RotateDirection

if __name__ == '__main__':

    cleaner = VacuumCleaner()

    print("Discovered things (1): {}".format(cleaner.discover_things()))

    print("Trying to go back - success : {}".format(cleaner.move(MoveDirection.BACKWARD)))
    # cleaner.print_env()

    if cleaner.check_passability():
        print("There is free space in front of device!")
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    else:
        print("There is NO free space in front of device!")
    cleaner.print_env()
    print("Discovered things (2): {}".format(cleaner.discover_things()))

