#!/bin/sh
socat TCP-LISTEN:55402,reuseaddr,fork EXEC:"stdbuf -i 0 -o 0 python /home/readflag/test_wrapper.py"
sleep infinity;
