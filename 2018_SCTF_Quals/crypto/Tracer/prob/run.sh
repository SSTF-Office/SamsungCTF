#!/bin/bash

PIN_ROOT=../pin-3.6-97554-g31f0a167d-gcc-linux

if [ $# -gt 0 ]
then
    $PIN_ROOT/pin -t $PIN_ROOT/source/tools/EIPLogger/obj-intel64/EIPLogger.so -- $@
else
    echo "Few arguments"
fi
