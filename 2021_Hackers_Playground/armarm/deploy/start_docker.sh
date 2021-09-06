#!/bin/sh
NAME="armarm"

SHARED=""
OPTION="--rm -d"
OPTION="-it --rm -v `pwd`/SET/:/SET/ --cap-add=SYS_PTRACE --security-opt=apparmor:unconfined"
#OPTION="-i -t --rm "
sudo docker run --name $NAME $PORT $OPTION $SHRED $NAME:1.0

#sudo docker start $NAME
