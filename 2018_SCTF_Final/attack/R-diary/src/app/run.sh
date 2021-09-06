#!/bin/sh
forever -o /var/log/out.log --uid "rdiary" -c /bin/sh /forever.sh
sleep infinity;
