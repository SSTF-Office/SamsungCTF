#!/bin/sh
export TERM=xterm
socat TCP-LISTEN:14697,pktinfo,reuseaddr,fork EXEC:"stdbuf -i 0 -o 0 /home/CowBoy/CowBoy"
sleep infinity;
