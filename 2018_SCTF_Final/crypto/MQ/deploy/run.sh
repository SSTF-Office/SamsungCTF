#!/bin/sh

docker build -t mq .
docker run -d --name mq \
    -p 12345:12345 \
    mq
