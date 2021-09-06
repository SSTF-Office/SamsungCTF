from api import VacuumCleaner, MoveDirection

if __name__ == '__main__':

    cleaner = VacuumCleaner()

    print("Trying to go back - success : {}".format(cleaner.move(MoveDirection.BACKWARD)))

    some_name_that_should_trigger_exception
