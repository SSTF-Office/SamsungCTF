
!WARNING! SPOILERS AHEAD! <br> 
. <br>
. <br>
. <br>
. <br>
. <br>
. <br>
. <br>
. <br>
. <br>
. <br>
. <br>
. <br>
. <br>
. <br>
. <br>
. <br>

## Description

Participants receive access to an IoT testing environment.
They can run their own software (firmware) on vacuum cleaners placed in a simulated environment.
Software needs to be written in Python 3.5 using provided API.
Information for developers include:
- documentation of ConnectedThings API,
- example update packages.

Dashboard allows following actions:
- starting firmware,
- obtaining limited results of execution (one-line traceback in case of exception)
- reading debug messages sent from device (limited to 1 message per minute of max length 100 chars and partially tampered to limit possibility of downloading data)
- showing statistics of number of performed mooves and failures and result for best cleaning algorithm,
- resetting environment in case of any problems (e.g. overwriting config files with flags).

Participants need to program their vacuum cleaners to explore whole environment and identify all devices (using device discovery).
Sensitive information is placed in configuration files of all devices.
In configuration of vacuum cleaner that participant is controlling at the beginning, there is a hint "Config files can sometimes contain sensitive information".
Parts of flags are placed in configuration files of other devices (simulating lightbulbs) situated in different locations in the environment.
Configuration files are only accessible for firmware run on this device, so participants need to update firmware of other devices using their vacuum cleaner.
To create their own update package participants need to prepare valid signature of unknown algorithm.
Update packages are in fact protected using RSA 512b over MD4 hash - this can be guessed by observing file "digest.der" included in the example digest package as a hint.
To create correct signature participant needs to find private key of the vendor, which can be found under path "/vendor_private_key.pem" while "/vendor_public_key.pem" is linked as a hint on the main page in section ("For clients"). There is also hint as HTML comment with link to vendor_private_key.pem.
One of minor problems participants need to investigate, is that configuration file in update package will overwrite device configuration file, but device can be updated with empty configuration file, but cannot be updated without configuration file.

Final part of flag (P5) is located in region that cannot be discovered by vacuum cleaner, so it must be updated using forward_update from lightbulb holding P4 fragment.
To allow to test worm spreading update two bulbs located close to dock can be used (they can discover and update each other).

To sum up: participants need to prepare firmware that will:
1. identify all 4 lightbulbs in the environment,
2. update all of them with tampered update package,
3. prepare "worm-like" updated and infect last one that is available only for another bulb,
3. download all 5 parts of flag from their configuration files,
4. send flag using debug message to participant.

## Security:

Firmware files prepared by participants are executed in sandboxed created as separated Docker containers using epicbox (https://github.com/StepicOrg/epicbox) library.
Implementation of API is acting as HTTP client.
Environments (separated for each participant) are simulated using HTTP server, common for all participants.
Participants can potentially disclose implementation of API by reading and downloading Python file, but this should not allow to bypass the logic of environment.
