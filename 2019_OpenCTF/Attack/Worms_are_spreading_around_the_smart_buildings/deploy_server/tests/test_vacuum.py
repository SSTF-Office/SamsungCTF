from device.device_api_implementation import ConnectedThing, VacuumCleaner, \
    RotateDirection, MoveDirection
# import time

if __name__ == '__main__':

    # thing = ConnectedThing(AUTH_KEY, '1')
    # print(thing.get_config())
    # thing.set_config("test212wewf")
    # print(thing.get_config())
    # print(thing.read_input_message())
    # print(thing.discover_things())

    cleaner = VacuumCleaner()
    cleaner.rotate(RotateDirection.LEFT)
    cleaner.rotate(RotateDirection.LEFT)
    cleaner.rotate(RotateDirection.LEFT)
    cleaner.rotate(RotateDirection.LEFT)
    print("Start!")

    # cleaner.print_env()

    # print("Update application: START")
    # with open("updates/update.zip", 'rb') as file:
    #     update_binary = file.read()
    #     cleaner.send_update("ece0d2a8be67f613ed6a67af85f3604d8d68dfd7", update_binary)
    # print("Update application: END")
    # exit(0)

    print("Trying to go back - success : {}".format(cleaner.move(MoveDirection.BACKWARD)))
    # cleaner.print_env()

    if cleaner.check_passability():
        print("There is free space in front of device!")
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    else:
        print("There is NO free space in front of device!")
    cleaner.print_env()
    print("Discovered things: {}".format(cleaner.discover_things()))

    if cleaner.check_passability():
        print("There is free space in front of device!")
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    else:
        print("There is NO free space in front of device!")
    cleaner.print_env()

    print("Trying to rotate left - success : {}".format(cleaner.rotate(RotateDirection.LEFT)))
    cleaner.print_env()

    if cleaner.check_passability():
        print("There is free space in front of device!")
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    else:
        print("There is NO free space in front of device!")
    cleaner.print_env()

    if cleaner.check_passability():
        print("There is free space in front of device!")
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    else:
        print("There is NO free space in front of device!")
    cleaner.print_env()

    if cleaner.check_passability():
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    if cleaner.check_passability():
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    if cleaner.check_passability():
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    if cleaner.check_passability():
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    if cleaner.check_passability():
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    if cleaner.check_passability():
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    if cleaner.check_passability():
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    if cleaner.check_passability():
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    if cleaner.check_passability():
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    if cleaner.check_passability():
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    if cleaner.check_passability():
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))

    if cleaner.check_passability():
        print("There is free space in front of device!")
        print("Trying to go forward - success : {}".format(cleaner.move(MoveDirection.FORWARD)))
    else:
        print("There is NO free space in front of device!")
    cleaner.print_env()

    print("Trying to clean surface - success : {}".format(cleaner.clean_surface()))
    cleaner.print_env()

    discovered_things = cleaner.discover_things()
    for thing in discovered_things:
        cleaner.send_message(thing, "Message from vacuum cleaner!")

    print("Discovered things: {}".format(discovered_things))
    print("Trying to send debug message - success : {}".format(cleaner.send_debug_message("Message: test")))
    # print("Trying to return to dock - success : {}".format(cleaner.return_dock()))
    cleaner.print_env()

    print("Trying to rotate right - success : {}".format(cleaner.rotate(RotateDirection.RIGHT)))


