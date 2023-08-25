#! /bin/bash

PORT=5051
NAME=escape
case "$1" in
  "--build")
    docker build --tag $NAME:latest .
    ;;
  "--run")
    docker run --name $NAME -d -p $PORT:5051 $NAME:latest
    ;;
  "--start")
    docker start $NAME
    ;;
  "--stop")
    docker stop $NAME
    ;;
  "--rm")
    docker rm -f $NAME &&
    docker rmi -f `docker images | grep $NAME | awk {'print $3'}`
    ;;
  "--sh")
    docker exec -it $NAME bash
    ;;
  *)
    echo "USE --build, --run, --start, --stop, --rm, --sh"
    docker images | grep $NAME
    ;;
esac
