#!/bin/sh

docker build -t supersonic .
docker rm -f supersonic
docker run -d --name supersonic \
    -v $(pwd)/access_log:/tmp/log \
    -p 22341:22341 \
    supersonic
