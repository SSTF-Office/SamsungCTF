#!/bin/sh
cp -r ../prob ./ && cp -r ../../platform/requirements.txt ./ \
  && docker build -t abc .
rm -rf ./prob ./requirements.txt
