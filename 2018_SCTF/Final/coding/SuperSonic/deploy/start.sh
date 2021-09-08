#!/bin/sh
export TERM=xterm
socat TCP-LISTEN:22341,pktinfo,reuseaddr,fork EXEC:"stdbuf -i 0 -o 0 python /home/SuperSonic/SuperSonic_server.py"
sleep infinity;
