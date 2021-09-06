#!/bin/sh

case "$1" in
  "--build")
    docker build --tag cyberpunk:latest .
    ;;
  "--run")
    docker run --name cyberpunk -p 31477:1337 cyberpunk:latest
    ;;
  "--start")
    docker start cyberpunk
    ;;
  "--stop")
    docker stop cyberpunk
    ;;
  "--rm")
    docker rm -f cyberpunk &&
    docker rmi -f `docker images | grep cyberpunk | awk {'print $3'}`
    ;;
  *)
    echo "USE --build, --run, --start, --stop, --rm"
    ;;
esac
