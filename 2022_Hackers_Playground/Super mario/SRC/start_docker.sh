#!/bin/sh
NAME="pipe"

SHARED=""
OPTION="--rm -it"

sudo docker run --name $NAME $PORT $OPTION $SHARED $NAME

