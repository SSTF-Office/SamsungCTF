#!/bin/bash
if [ ! -e "src" ] ; then
    echo "I think.. it's wrong cwd"
    exit 0
fi

if [ ! -e "deploy" ]; then
    mkdir deploy
fi

rm -rf deploy/src
rm -rf deploy/exploit
cp -rf src/ deploy/src
cp -rf exploit/ deploy/exploit

cd deploy
find ./ -type f -exec sed -i 's/{{BASE_HOST}}/flagstore2\.sstf\.site:13337/g' {} \;
find ./ -type f -exec sed -i 's/{{EXPOSE_PORT}}/13337/g' {} \;
find ./ -type f -exec sed -i 's/{{ADMIN_PASSWORD}}/dnflwlqdpdhodhkTsl/g' {} \;
cd -
