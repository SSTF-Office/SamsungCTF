#!/bin/sh
secret_dir=`mktemp -d --tmpdir=/tmp`
mount -n -o size=50m -t tmpfs tmpfs $secret_dir
cp /shellfuzz `echo $secret_dir`
cp /shellfuzz.c `echo $secret_dir`
chown -R shellfuzz_pwn:shellfuzz_pwn `echo $secret_dir`
chmod 111 /tmp
chmod 777 `echo $secret_dir`
chmod 4755 `echo $secret_dir`/shellfuzz
cd `echo $secret_dir`
su shellfuzz -c /bin/sh

