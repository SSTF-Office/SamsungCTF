#!/bin/sh

case "$1" in
  "--build")
    docker build --tag luqwest:latest .
    ;;
  "--run")
    docker run --name luqwest -p 37714:1337 luqwest:latest
    ;;
  "--start")
    docker start luqwest
    ;;
  "--stop")
    docker stop luqwest
    ;;
  "--rm")
    docker rm -f luqwest &&
    docker rmi -f `docker images | grep luqwest | awk {'print $3'}`
    ;;
  "--sh")
    docker exec -it luqwest bash
    ;;
  *)
    echo "USE --build, --run, --start, --stop, --rm, --sh"
    docker images | grep luqwest
    ;;
esac
