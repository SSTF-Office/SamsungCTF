 1. Get PMKID and MAC addresses of both sides from packets in the file.

 2. For each guess for the password, derive PMKID using SSID and MAC addresses and then check the equality of them.

[This challenge should have higher points according to its cryptographic complexity, but adjusted to 150pt because attack can be done by using John the Ripper, without writing own exploits.]