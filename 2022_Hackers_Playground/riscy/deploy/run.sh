#!/usr/bin/env bash
ROOT=$(dirname "$0")
PORT=$(cat $ROOT/../PORT)

docker run -p $PORT:$PORT riscy
