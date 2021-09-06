#!/bin/bash
docker build -t sctf/adb . 
docker run --rm -p 31555:6666 -e ADBKEY="$(cat android/adbkey)" --name adb --device /dev/kvm -d sctf/adb
