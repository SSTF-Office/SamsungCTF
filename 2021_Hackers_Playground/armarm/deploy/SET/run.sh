#!/bin/sh
#chroot /home/guest/ ./qemu-arm-static ./prob
#/usr/bin/qemu-arm-static -strace -L /usr/arm-linux-gnueabihf/ /home/guest/prob 2>/tmp/result
/usr/bin/qemu-arm-static -L /usr/arm-linux-gnueabihf/ /home/guest/prob 2>/dev/null
