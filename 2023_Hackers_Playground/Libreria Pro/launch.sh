#!/bin/bash

docker-compose down --remove-orphans
docker-compose up -d --build
sleep 10
pushd deploy/server
./postsetup.sh
popd
