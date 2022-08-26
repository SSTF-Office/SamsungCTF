#!/bin/sh
socat TCP-LISTEN:9999,reuseaddr,fork,su=app,max-children=100 'EXEC:python powdle.py'
