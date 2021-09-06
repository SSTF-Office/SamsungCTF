#!/bin/sh
cd src
make
cd ..
docker build . -t memory
