#!/bin/bash

docker rm -f frontend
docker rm -f backend
docker rm -f mongo

# app
cd app
docker build -t front_build .
docker run --name frontend -p 3000:3000 -it -d front_build
cd ..

# mongo
docker pull mongo:latest
docker run --name mongo \
           -p 127.0.0.1:27017:27017\
           -e MONGO_INITDB_ROOT_USERNAME=root\
           -e MONGO_INITDB_ROOT_PASSWORD=root_password_cannot_guess\
           -it -d mongo:latest

sleep 3
# create new user
docker exec mongo mongo admin --username root --password root_password_cannot_guess --eval 'db = db.getSiblingDB("rdiary");db.createUser({user: "rdiary", pwd: "rdiary_password_cannot_guess", roles: [{role: "readWrite", db: "rdiary"}]})'

# backend
cd backend
docker build -t backend_build .
docker run --name backend --link=mongo:mongo -p 3001:3001 -it -d backend_build
cd ..
