# -*- coding: utf-8 -*-
"""Interface for hardware of Connected Devices."""

from enum import Enum


class MoveDirection(Enum):
    """Enum for selecting direction of move."""
    FORWARD = 1
    BACKWARD = 2


class RotateDirection(Enum):
    """Enum for selecting direction of rotation."""
    LEFT = 1
    RIGHT = 2


class ConnectedThing(object):
    """Common interface for all types of devices."""
    def __init__(self):
        """Creates ConnectedThing device."""
        pass

    MESSAGE_MAX_SIZE = 150

    MESSAGE_MIN_TIME_SEC = 60

    MAX_DISTANCE_DEVICE_DISCOVER = 5

    def get_dev_id(self) -> str:
        """Returns unique identifier of this device.

        Returns:
            str: String containing unique identifier of this device.
        """
        pass

    def get_config(self) -> str:
        """Returns current configuration of device in YAML format.

        Returns:
            str: String containing YAML formatted configuration.
        """
        pass

    def set_config(self, config: str) -> bool:
        """Sets current configuration of device.

        Args:
            config (str): String with YAML configuration.

        Returns:
            bool: True for success, False otherwise.
        """
        pass

    def discover_things(self) -> list:
        """Performs discovery of nearby devices and returns list of them.
        Returns:
            list: List of device identifiers of discovered devices.
        """
        pass

    def send_message(self, thing_id: str, message: str) -> bool:
        """Sends text message to nearby device. <br>
            Messages can be sent only nearby destination device (where device can be discovered). <br>
            Messages can be tampered because of low reliability of our narrowband link. <br>
            Messages must be ASCII encoded with no binary characters! <br>
            Maximal size of message is 150 characters!

        Args:
            thing_id (str): Identifier of device.
            message (str): Content of message to another device.

        Returns:
            bool: True for success, False otherwise.
        """
        pass

    def send_update(self, thing_id: str, update_package: bytes) -> bool:
        """Sends update package to nearby device.

        Args:
            thing_id (str): Identifier of device.
            update_package (bytes): Content of update package for another device.

        Returns:
            bool: True for success, False otherwise.
        """
        pass

    def forward_update(self, thing_id: str) -> bool:
        """Sends update package used by this device to nearby device.<br>
            This can be used only by devices of the same type!

        Args:
            thing_id (str): Identifier of device.

        Returns:
            bool: True for success, False otherwise.
        """
        pass

    def read_input_message(self) -> str:
        """Reads incoming messages from other devices or system sent by docking station.

        Returns:
            str: First incoming message or empty string, if no messages are available.
        """
        pass

    def get_battery_level(self) -> int:
        """Returns current level of battery charge.
        Returns:
            int: Current level of battery charge in percentage (0-100).
        """
        pass


class VacuumCleaner(ConnectedThing):
    """Interface for vacuum cleaners."""
    def __init__(self):
        """Creates VacuumCleaner device."""
        super().__init__()

    def check_passability(self) -> bool:
        """Checks whether space in front of device is unoccupied.

        Returns:
            bool: True if space in front of device is passable, False otherwise.
        """
        pass

    def check_surface(self) -> bool:
        """Checks whether surface under device is clean.

        Returns:
            bool: True if surface under device is clean, False otherwise.
        """
        pass

    def clean_surface(self) -> bool:
        """Cleans surface near device.
        Cleaning of each block takes around 10 seconds and uses around 1% of battery power.

        Returns:
            bool: True for success, False otherwise.
        """
        pass

    def rotate(self, direction: RotateDirection) -> bool:
        """Rotates device in desirable direction.

        Args:
            direction (RotateDirection): Direction of rotation.

        Returns:
            bool: True for success, False otherwise.
        """
        pass

    def move(self, direction: MoveDirection) -> bool:
        """Moves device in desirable direction.
        Moving takes around 1 second and uses around 0.3% of battery power.

        Args:
            direction (MoveDirection): Direction of move.

        Returns:
            bool: True for success, False otherwise.
        """
        pass

    def return_dock(self) -> bool:
        """Returns device to docking station.

        Returns:
            bool: True for success, False otherwise.
        """
        pass

    def send_debug_message(self, message: str) -> bool:
        """Sends debug message.

        Restrictions:

        * Messages can be sent only nearby docking stations. <br>
        * Messages must be ASCII encoded with no binary characters! <br>
        * Maximal size of a single message is 150 characters. <br>
        * Each device can send only one message per minute. <br>
        * Messages can be tampered because of low reliability of our narrowband link.

        Args:
            message (str): Content of debug message to be sent.

        Returns:
            bool: True for success, False otherwise.
        """
        pass
