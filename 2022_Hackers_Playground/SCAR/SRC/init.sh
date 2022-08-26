#!/bin/bash
#/usr/sbin/service xinetd restart
runuser prob -c "socat TCP-LISTEN:31337,reuseaddr,fork EXEC:/AttackMe.py &"
runuser --pty guest -c "/bin/bash -i"
#/bin/bash -i
