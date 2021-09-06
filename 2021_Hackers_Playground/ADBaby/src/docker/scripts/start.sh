#!/bin/bash
screen -S emulator  -d -m /android/sdk/launch-emulator.sh

screen -S adbd      -d -m su sctf -c "/home/sctf/scripts/start_adbd.sh"
screen -S socat     -d -m su sctf -c "socat -d tcp-listen:6666,reuseaddr,fork tcp:127.0.0.1:5558"

# screen -r adbd
# /bin/bash
tail -f /dev/null
