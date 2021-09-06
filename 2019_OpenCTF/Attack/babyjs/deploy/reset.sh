#!/bin/bash
rm -rf ../src/db
mkdir ../src/db

docker-compose rm -f
docker-compose build
docker-compose up
