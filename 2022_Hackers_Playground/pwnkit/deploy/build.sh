#!/usr/bin/env bash

(cd ../src; make)
docker build -t pwnkit ..
