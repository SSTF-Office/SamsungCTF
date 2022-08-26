#!/bin/sh
sudo docker run -v /var/run/docker.sock:/var/run/docker.sock --rm --privileged -p 31339:31339 -it protocol_proxy
