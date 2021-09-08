#!/bin/sh

sudo docker build -t disposable .
sudo docker rm -f disposable
sudo docker run -d --name disposable \
    -p 30010:30010 \
    disposable
