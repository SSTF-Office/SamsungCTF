#!/bin/sh
export TERM=xterm
socat TCP-LISTEN:22341,reuseaddr,fork EXEC:"stdbuf -i 0 -o 0 python /home/lights/3-Lights.py"
sleep infinity;
