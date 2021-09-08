#!/bin/sh

docker build -t lcg .
docker rm -f lcg
docker run -d --name lcg \
    -p 12345:12345 \
    lcg
