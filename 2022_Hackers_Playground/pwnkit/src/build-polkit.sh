#!/usr/bin/env bash

ROOT=$(realpath $(dirname "$0"))
REPO=https://github.com/freedesktop/polkit.git
COMMIT=4ff1abe4a4c1f8c8378b9eaddb0346ac6448abd8

cd $ROOT

if [ ! -e $ROOT/polkit ]; then
    git clone $REPO polkit
    (cd polkit && git checkout $COMMIT)
    (cd polkit && patch -p1 <$ROOT/CVE-2021-4034.patch)
fi

if [ ! -e $ROOT/polkit/build ]; then
    (cd polkit && meson build)
fi

if [ -e $ROOT/polkit/build ]; then
    (cd $ROOT/polkit/build && ninja)
fi

cp -f $ROOT/polkit/build/src/programs/pkexec $ROOT/pkexec

