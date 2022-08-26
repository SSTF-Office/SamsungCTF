#!/bin/sh
cd /home/riscy
env -i PATH=/bin:/usr/bin:/usr/local/bin qemu-riscv64 ./target
