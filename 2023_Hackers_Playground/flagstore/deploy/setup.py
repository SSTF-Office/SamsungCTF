#!/usr/bin/python3
import shutil

DST="flagstore"
try:
    shutil.rmtree(f'./{DST}')
except:
    pass
shutil.copytree('../src', f'./{DST}', ignore=shutil.ignore_patterns(
    'node_modules',
    'package-lock.json',
    'docker-compose.dev.yml'
    'docker-compose.dist.yml'))
shutil.move(f'./{DST}/gateway/package_prod.json', f'./{DST}/gateway/package.json')
shutil.move(f'./{DST}/ms-order/package_prod.json', f'./{DST}/ms-order/package.json')
shutil.move(f'./{DST}/ms-secret/package_prod.json', f'./{DST}/ms-secret/package.json')
shutil.move(f'./{DST}/ms-user/package_prod.json', f'./{DST}/ms-user/package.json')
