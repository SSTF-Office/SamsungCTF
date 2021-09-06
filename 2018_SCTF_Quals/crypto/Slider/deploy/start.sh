#!/bin/sh
export TERM=xterm
socat TCP-LISTEN:6884,pktinfo,reuseaddr,fork EXEC:"stdbuf -i 0 -o 0 python3 /home/Slider/slider.py"
sleep infinity;
