import unittest
from server.environment import *


class TestEnvironmentMethods(unittest.TestCase):

    def test_new_position_up(self):
        (x, y) = new_position(10, 20, MazeDirection.WEST, MoveDirection.FORWARD)
        self.assertEqual((x, y), (9, 20))

        (x, y) = new_position(5, 1, MazeDirection.EAST, MoveDirection.FORWARD)
        self.assertEqual((x, y), (6, 1))

        (x, y) = new_position(5, 8, MazeDirection.NORTH, MoveDirection.FORWARD)
        self.assertEqual((x, y), (5, 7))

        (x, y) = new_position(20, 4, MazeDirection.SOUTH, MoveDirection.FORWARD)
        self.assertEqual((x, y), (20, 5))

        (x, y) = new_position(10, 20, MazeDirection.EAST, MoveDirection.BACKWARD)
        self.assertEqual((x, y), (9, 20))

        (x, y) = new_position(5, 1, MazeDirection.WEST, MoveDirection.BACKWARD)
        self.assertEqual((x, y), (6, 1))

        (x, y) = new_position(5, 8, MazeDirection.SOUTH, MoveDirection.BACKWARD)
        self.assertEqual((x, y), (5, 7))

        (x, y) = new_position(20, 4, MazeDirection.NORTH, MoveDirection.BACKWARD)
        self.assertEqual((x, y), (20, 5))

    def test_discovery(self):
        environment = create_sample_environment()

        discovered = environment.device_discover(DEVICE_VC_000)
        self.assertEqual([secure_device_id(DEVICE_ID_000)], discovered)
        self.assertEqual(len(discovered), 1)

        discovered = environment.device_discover(DEVICE_ID_000)
        list = [secure_device_id(DEVICE_ID_001)]
        list.append(secure_device_id(DEVICE_VC_000))
        self.assertEqual(list, discovered)
        self.assertEqual(len(discovered), 2)

        discovered = environment.device_discover(DEVICE_ID_001)
        self.assertEqual(len(discovered), 1)

        discovered = environment.device_discover(DEVICE_ID_002)
        self.assertEqual(len(discovered), 0)

        discovered = environment.device_discover(DEVICE_ID_003)
        self.assertEqual([secure_device_id(DEVICE_ID_004)], discovered)
        self.assertEqual(len(discovered), 1)

        discovered = environment.device_discover(DEVICE_ID_004)
        self.assertEqual([secure_device_id(DEVICE_ID_003)], discovered)
        self.assertEqual(len(discovered), 1)

        environment.kill()

    def test_environment(self):
        environment = create_sample_environment()
        environment.print_env()
        cleaner = environment.devices[DEVICE_VC_000]
        self.assertEqual(cleaner.location_x, 1)
        self.assertEqual(cleaner.location_y, 1)
        environment.device_move(DEVICE_VC_000, MoveDirection.FORWARD, operation_time=0)
        discovered = environment.device_discover(DEVICE_VC_000)
        self.assertEqual(len(discovered), 1)
        segment = environment.maze.get_segment(1, 2)
        self.assertFalse(segment.surface_clean)
        environment.device_clean_space(DEVICE_VC_000, operation_time=0)
        self.assertTrue(segment.surface_clean)
        environment.device_move(DEVICE_VC_000, MoveDirection.FORWARD, operation_time=0)
        environment.device_move(DEVICE_VC_000, MoveDirection.FORWARD, operation_time=0)
        environment.device_move(DEVICE_VC_000, MoveDirection.FORWARD, operation_time=0)
        self.assertEqual(cleaner.location_x, 1)
        self.assertEqual(cleaner.location_y, 5)
        environment.device_rotate(DEVICE_VC_000, RotateDirection.LEFT, operation_time=0)
        for i in range(0, 14):
            environment.device_move(DEVICE_VC_000, MoveDirection.FORWARD, operation_time=0)
        self.assertEqual(cleaner.location_x, 15)
        self.assertEqual(cleaner.location_y, 5)
        discovered = environment.device_discover(DEVICE_VC_000)
        self.assertEqual([], discovered)
        self.assertEqual(len(discovered), 0)
        environment.device_move(DEVICE_VC_000, MoveDirection.FORWARD, operation_time=0)
        environment.device_move(DEVICE_VC_000, MoveDirection.FORWARD, operation_time=0)
        environment.device_move(DEVICE_VC_000, MoveDirection.FORWARD, operation_time=0)
        # environment.print_env()

        environment.device_clean_space("1", operation_time=0)
        # environment.print_env()

        environment.device_dock("1", operation_time=0)
        # environment.print_env()
        environment.kill()


if __name__ == '__main__':
    unittest.main()
