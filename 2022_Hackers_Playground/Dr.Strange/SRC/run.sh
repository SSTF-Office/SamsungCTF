#!/bin/bash
runuser prob -c "taskset -c 1 /AttackMe &"
echo `date` > /tmp/prob
sleep 1
chmod 777 /dev/shm/test_shm

runuser --pty guest -c "/bin/bash"

