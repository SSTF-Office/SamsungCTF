#!/bin/sh
NAME="timming"

SHARED=""
OPTION="--rm -it"

docker run --name $NAME $PORT $OPTION $SHARED $NAME:1.0

