#!/bin/sh
sudo docker run --rm -v /var/run/docker.sock:/var/run/docker.sock -p 31340:31340 -idt cache_proxy
