#!/bin/sh
socat TCP-LISTEN:23972,reuseaddr,fork EXEC:"stdbuf -i 0 -o 0 /home/magic/magic"
sleep infinity;
