#!/bin/sh
export TERM=xterm
socat TCP-LISTEN:46115,reuseaddr,fork EXEC:"stdbuf -i 0 -o 0 python /home/buildingblocks/buildingblocks.py"
sleep infinity;
