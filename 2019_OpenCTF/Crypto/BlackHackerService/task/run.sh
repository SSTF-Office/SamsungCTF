#!/bin/bash

host=dbhost
port=3306

echo -n "Waiting for sql..."

while ! nc -w 1 $host $port 2>/dev/null
do
	echo -n .
	sleep 30
done

cd /var/run/task/
python init_db.py
gunicorn --workers 4 --max-requests 32 --name app -b 0.0.0.0:8080 main:app
