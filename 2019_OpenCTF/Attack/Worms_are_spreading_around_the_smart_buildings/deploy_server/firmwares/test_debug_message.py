from api import VacuumCleaner, MoveDirection, RotateDirection
import time

if __name__ == '__main__':

    cleaner = VacuumCleaner()
    cleaner.send_debug_message("Very long debug message: " + 50 * "xyz")
    time.sleep(10)
    cleaner.send_debug_message("Very long debug message: " + 50 * "abc")
    time.sleep(60)
    cleaner.send_debug_message("Very long debug message: " + 50 * "123")
    time.sleep(10)
    cleaner.send_debug_message("Very long debug message: " + 50 * "456")
