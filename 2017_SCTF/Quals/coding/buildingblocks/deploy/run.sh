#!/bin/sh

docker build -t buildingblocks .
docker rm -f buildingblocks
docker run -d --name buildingblocks -p 46115:46115 buildingblocks
