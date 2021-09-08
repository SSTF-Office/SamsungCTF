#!/bin/sh
cd ../prob && make && cp bin/bug ../deploy/bug && cd ../deploy && \
  docker build -t bug .
