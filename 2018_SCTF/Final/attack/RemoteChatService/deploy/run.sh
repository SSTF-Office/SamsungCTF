#!/bin/bash

docker build -t chat_service .
docker rm -f chat_service
docker run -d --name chat_service -p 13137:13137 chat_service
