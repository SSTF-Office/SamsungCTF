from api import VacuumCleaner, MoveDirection, RotateDirection

if __name__ == '__main__':
    cleaner = VacuumCleaner()

    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    surface = cleaner.check_surface()
    if not surface:
        cleaner.clean_surface()
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    cleaner.rotate(RotateDirection.LEFT)
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    cleaner.rotate(RotateDirection.LEFT)
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    cleaner.rotate(RotateDirection.RIGHT)
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    cleaner.rotate(RotateDirection.RIGHT)
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
    if cleaner.check_passability():
        cleaner.move(MoveDirection.FORWARD)
    if not cleaner.check_surface():
        cleaner.clean_surface()
