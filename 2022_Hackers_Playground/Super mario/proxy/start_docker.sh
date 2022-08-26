#!/bin/sh
sudo docker run --rm -v /var/run/docker.sock:/var/run/docker.sock -p 31338:31338 -idt pipe_proxy
