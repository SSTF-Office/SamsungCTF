#!/bin/sh
socat -T 30 -t 10 TCP-LISTEN:12345,reuseaddr,pktinfo,fork EXEC:"stdbuf -i 0 -o 0 -e 0 python3.6 -u eth_chall.py Gamble.sol Gamble" &
sleep infinity
