from device.device_api_implementation import VacuumCleaner

if __name__ == '__main__':

    cleaner = VacuumCleaner()
    print("Start!")
    print("Update application: START")
    devices = cleaner.discover_things()
    with open("updates/update.zip", 'rb') as file:
        update_binary = file.read()
        print("Loaded binary")
        result = cleaner.send_update(devices.pop(0), update_binary)
        print("Update result = {}".format(result))
    print("Update application: END")
