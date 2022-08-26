#!/bin/sh
sudo docker run --name timming_proxy --rm -v /var/run/docker.sock:/var/run/docker.sock -p 31339:31339 -idt timming_proxy
