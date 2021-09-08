#!/bin/sh

redis-server &
/etc/init.d/nginx restart
uwsgi --ini /tmp/uwsgi.ini
