#!/bin/sh

docker build -t lights .
docker rm -f lights
docker run -d --name lights -p 22341:22341 lights
