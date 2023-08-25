#!/bin/bash
if [ ! -e "src" ] ; then
    echo "I think.. it's wrong cwd"
    exit 0
fi

if [ ! -e "dist" ] ; then
    mkdir dist
fi
rm -rf dist/src
rm -rf dist/src.tar.gz
cp -rf src/ dist/src

cd dist
echo SCTF{something} > src/flag
find src -type f -exec sed -i 's/{{BASE_HOST}}/localhost:13337/g' {} \;
find src -type f -exec sed -i 's/{{EXPOSE_PORT}}/13337/g' {} \;
find ./ -type f -exec sed -i 's/{{ADMIN_PASSWORD}}/admin/g' {} \;
tar zcvf src.tar.gz ./src
rm -r ./src
cd -
