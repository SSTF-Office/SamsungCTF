#! /bin/sh
#!/bin/bash
tail -F /var/log/nginx/* &
php-fpm7.2
nginx -g 'daemon off;'
