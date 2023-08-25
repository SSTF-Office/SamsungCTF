#!/bin/sh
chown -R root:challenger /home/challenger
chown -R root:challenger /flag.txt
/etc/init.d/xinetd restart
sleep infinity;

