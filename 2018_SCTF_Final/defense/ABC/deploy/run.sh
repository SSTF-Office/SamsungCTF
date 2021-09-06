#!/bin/sh
PORT=$@
./run.py $PORT
while [ $? -eq 123 ];
do
  ./run.py $PORT
done
