#!/bin/sh

docker build -t saltydsa .
docker run -d --name saltydsa \
    -p 12345:12345 \
    -p 54321:54321 \
    saltydsa
