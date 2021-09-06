#!/bin/sh
NAME="sctfheap"
sudo docker rm $NAME
sudo docker kill $NAME
sudo docker build --tag $NAME:1.0 ./

PORT="-p 20000:20000"

SHARED=
OPTION=" --rm "
#OPTION="--rm  --cap-add=SYS_PTRACE --ulimit core=-1 --security-opt seccomp=unconfined "
OPTION=" --read-only --rm -v `pwd`/tmp/:/tmp"

#sudo docker run -idt --name $NAME $PORT $SHRED $OPTION $NAME:1.0 /bin/bash
sudo docker run -d --name $NAME $PORT $SHRED $OPTION $NAME:1.0

