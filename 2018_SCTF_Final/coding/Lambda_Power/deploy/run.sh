#!/bin/sh
docker build -t ghost .
docker run -p 9999:9999 --rm -it ghost
