#!/bin/bash

cd deploy
rm web/task.tgz
tar -zcvf web/task.tgz ../task/

docker-compose build
docker-compose up
