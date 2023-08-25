#!/bin/bash
find ./ -type f -exec sed -i 's/{{BASE_HOST}}/srv1\.sstf\.site:13337/g' {} \;
find ./ -type f -exec sed -i 's/{{EXPOSE_PORT}}/13337/g' {} \;
