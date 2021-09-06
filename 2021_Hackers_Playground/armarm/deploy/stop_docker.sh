#!/bin/sh
sudo docker kill armarm

sudo docker rmi -f `sudo docker images |grep armarm | awk {'print $3'}`
