#!/bin/sh
export TERM=xterm
socat TCP-LISTEN:12345,pktinfo,reuseaddr,fork EXEC:"stdbuf -i 0 -o 0 python3 /home/mq/MQ.py"
sleep infinity;
