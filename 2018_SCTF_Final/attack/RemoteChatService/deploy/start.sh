#!/bin/bash

# setup permission
chmod 551 /proc
chmod 1773 /tmp
chmod 1773 /var/tmp

# client xinetd
/etc/init.d/xinetd start;
# server
while ( true ); do
    su chat_server -c /home/chat_server/chat_server
done
