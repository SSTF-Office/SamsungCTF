#!/bin/bash

pushd ../.. > /dev/null

PROJECT_DIR=`pwd`
PROJECT_NAME=${PROJECT_DIR##*/}

popd > /dev/null

docker-compose -p ${PROJECT_NAME} "$@"
