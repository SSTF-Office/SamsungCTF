#!/bin/bash
rm -rf pipe$1.tar
#rm -rf ubuntu22.tar
sudo docker save -o pipe$1.tar pipe$1
#sudo docker save -o ubuntu22.tar ubuntu
sudo chmod 766 ./pipe$1.tar
sudo docker rmi `docker images |grep pipe |awk {'print $3'}`
