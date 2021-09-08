#!/bin/sh

docker build -t readflag .
docker rm -f readflag
docker run -d --name readflag -p 55402:55402 readflag
