#!/bin/sh

docker build -t magic .
docker rm -f magic
docker run -d --name magic -p 23972:23972 magic
