# MazeAdventure Solution


## Reversen Engineering Electron App
Checking provided elf binary with strings command, you can guess the binary is built with electron builder.
Electron binary for linux distribution is packed with appImage.
You can extract the archive with command `./maze-adventure.bin --appimage-extract`.
Unpacking app.asar with `npx asar extract app.asar folder` command make you analyze electron based application.

## Tampering database
Earned cash data in game is persistent even if restart or delete and re-download the app. Then you can guess there is external persistent data storage to store game related data.
Tracking with strace command and check carefully, you can finally find out the data is stored at `~/.config/maze-adventure/Local Storage/leveldb`. Change data stored that directory lead you get the flag.
Database data tampering attack code is enclosed in `ex.py`.