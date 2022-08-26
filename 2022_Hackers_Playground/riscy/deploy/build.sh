#!/usr/bin/env bash
ROOT=$(dirname "$0")
PORT=$(cat $ROOT/../PORT)

docker build --build-arg PORT=$PORT -t riscy .
