#!/bin/sh

docker build -t slider .
docker rm -f slider
docker run -d --name slider \
    -p 6884:6884 \
    slider
