#!/bin/sh
docker rmi `docker images |grep pipe | awk '{print $3}'` 2> /dev/null 1>/dev/null
value=`cat /connection_count`
value=$((value+1))
value=$((value%10))
echo $value > /connection_count
#docker rmi `docker images |grep ubuntu | awk '{print $3}'`
docker load -i pipe$value.tar 2> /dev/null 1>/dev/null
#aocker load -i ubuntu22.tar

docker run --rm -i "pipe$value"
