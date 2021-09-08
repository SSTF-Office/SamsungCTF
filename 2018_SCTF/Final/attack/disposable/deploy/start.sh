#!/bin/sh
export TERM=xterm
socat TCP-LISTEN:30010,pktinfo,reuseaddr,fork EXEC:"stdbuf -i 0 -o 0 /home/disposable/disposable",stderr
sleep infinity;
