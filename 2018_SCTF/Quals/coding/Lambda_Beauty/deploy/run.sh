#!/bin/sh
docker build -t beauty .
docker run --name beauty -p42:42 -p4242:4242 --rm -it beauty
