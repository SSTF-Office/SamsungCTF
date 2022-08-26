#!/bin/sh
NAME="cache"

SHARED=""
OPTION="--rm -it"

sudo docker run --name $NAME $PORT $OPTION $SHARED $NAME:1.0 /bin/bash
