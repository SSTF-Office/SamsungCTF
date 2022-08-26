#!/bin/sh
NAME="pipe$1"
docker build --tag $NAME ./
