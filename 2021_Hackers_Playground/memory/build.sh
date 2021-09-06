#!/bin/bash
cd prob
./build.sh
cd src
make
cd ../../proxy
./build.sh
