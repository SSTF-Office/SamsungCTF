#!/bin/sh

docker build -t cowboy .
docker rm -f cowboy
docker run -d --name cowboy \
    -p 14697:14697 \
    cowboy
