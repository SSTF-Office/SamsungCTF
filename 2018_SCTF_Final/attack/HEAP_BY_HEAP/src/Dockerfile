FROM ubuntu:16.04
MAINTAINER k1rh4 <k1rh4.lee@gmail.com>

###### SERVER SETTING ########
#RUN apt-get install -y openssh-server
#RUN apt-get install -y vim

RUN sed -i 's/security.ubuntu.com/ftp.daumkakao.com/g' /etc/apt/sources.list
RUN sed -i 's/archive.ubuntu.com/ftp.daumkakao.com/g' /etc/apt/sources.list
RUN apt-get update

#RUN apt-get install -y python
#RUN apt-get update
#RUN apt-get install -y python-pip
#RUN pip install --upgrade pip
#RUN apt-get install -y vim
#RUN apt-get install -y gdb
#RUN apt-get install -y strace
#RUN apt-get install cron -y
#RUN apt-get install netcat -y
#RUN apt-get install net-tools -y


RUN apt-get install -y libssl1.0.0
RUN apt-get install -y xinetd
RUN apt-get install -y gcc 
RUN apt-get install -y make
RUN apt-get install -y libssl-dev
########### USER CREATE #############
RUN useradd -d /home/heap heap -s /bin/bash
RUN mkdir /home/heap

###### PROB  SETUP #####
ADD ./heap.c /home/heap/heap.c
ADD ./heap.h /home/heap/heap.h
ADD ./md5.c /home/heap/md5.c
ADD ./md5.h /home/heap/md5.h
ADD ./makefile /home/heap/makefile

ADD ./flag /flag
ADD ./banner.txt ./banner.txt
########## HOME DIR SETTING #############
RUN chown -R root:heap /home/heap
RUN chmod 750 /home/heap

RUN chown root:heap /flag
RUN chmod 440 /flag

####### XINETD SETTING
ADD ./load.xinetd /etc/xinetd.d/load
ADD ./start.sh /start.sh


RUN cd /home/heap && make && make clean && make remove

WORKDIR /home/heap
ENTRYPOINT /start.sh

