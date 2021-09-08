#!/bin/sh
docker run -p 55555:55555 --net=host -d --name abc -t abc
