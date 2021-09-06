#!/bin/sh

/etc/init.d/nginx restart
uwsgi --ini /tmp/uwsgi.ini
