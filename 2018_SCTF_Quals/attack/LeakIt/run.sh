#!/bin/bash
docker-compose stop
docker-compose build
cd deploy
./setup.sh
cd ..
docker-compose up -d
docker-compose exec --user root leakit umount /proc
