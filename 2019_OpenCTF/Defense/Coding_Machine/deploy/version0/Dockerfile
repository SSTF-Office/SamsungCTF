FROM ubuntu:18.04
MAINTAINER matta <js00n.park@gmail.com>

#Install xinetd
#Install xinetd
RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN dpkg --add-architecture i386
RUN apt-get update
RUN apt-get install -y xinetd 
RUN apt-get install -y python
RUN apt-get install -y libc6:i386
RUN apt-get install -y net-tools
RUN apt-get install -y vim


#Adduser
RUN useradd -b /home/code code -s /bin/bash

#Copy Binary
ADD server.py /home/code/server.py
ADD loader /home/code/loader
ADD check.sh /home/code/check.sh
ADD test_input1.txt /home/code/test_input1.txt

#Set xinetd
ADD code.xinetd /etc/xinetd.d/code

#Set Priviledges
RUN chown -R code:code /home/code
RUN chmod 750 /home/code

#COPY start script
ADD start.sh /start.sh
RUN chmod +x /start.sh

WORKDIR /home/code
CMD ["/start.sh"]

EXPOSE 9000
