class LightBulb(ConnectedThing):
    """Interface for lightbulbs."""
    def __init__(self):
        super().__init__()

    def set_brightness(self, brightness: float) -> bool:
        """Sets brightness level.
        Args:
            brightness (float): Brightness.

        Returns:
            bool: True for success, False otherwise.
        """
        pass

    def get_brightness(self) -> float:
        """Returns current brightness level

        Returns:
            float: Brightness.
        """
        pass

    def set_color(self, color: float) -> bool:
        """Sets color value
        Args:
            color (float): Color.

        Returns:
            bool: True for success, False otherwise.
        """
        pass

    def get_color(self) -> float:
        """Returns current color value"""
        pass
