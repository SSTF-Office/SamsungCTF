#!/bin/sh
rabbitmq-server -detached
sleep 1;
nodejs /usr/src/app/init.js;
forever -o /var/log/backend.log --uid "rdiary" -c /bin/sh /forever.sh;
#cd / && celery -A bot worker --loglevel=info
