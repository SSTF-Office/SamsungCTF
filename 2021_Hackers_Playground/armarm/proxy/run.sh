#!/bin/bash
sudo docker run -v /var/run/docker.sock:/var/run/docker.sock --privileged -p 31338:31338 -it armarm_proxy 
