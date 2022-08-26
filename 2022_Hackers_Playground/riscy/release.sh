#!/usr/bin/env bash

set -ex

ROOT=$(dirname "$0")
DIR=$ROOT/release

rm -rf $DIR
mkdir $DIR

cp -rf deploy $DIR/
cp -rf src $DIR/
cp -rf PORT $DIR/
cp -rf README.md $DIR/

rm -f $DIR/deploy/check.py
rm -f $DIR/src/exploit.py
echo "SCTF{THIS-IS-NOT-A-FLAG}" > $DIR/deploy/flag

tar czf riscy.tar.gz release/
