#!/bin/sh
sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
apt-get update && apt-get -y dist-upgrade
apt-get install -yy python3
apt-get install -yy python3-pip
pip3 install -U pip
pip3 install -r requirements

python3 prob/gen_data.py --data_dir deploy/src/dataset --char_num 10000
cd deploy/src
find dataset -type f > image_list.txt
