FROM ubuntu:16.04
MAINTAINER HYUNGSEOK HAN <hyungseok.han@kaist.ac.kr>

#Install xinetd
RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN dpkg --add-architecture i386
RUN apt-get update
RUN apt-get install -y xinetd 
RUN apt-get install -y libc6:i386

#Adduser
RUN useradd -b /home/memo  memo
#Copy Binary
ADD memo /home/memo/memo

#Set xinetd
ADD memo.xinetd /etc/xinetd.d/memo

#Set Flag
ADD flag /home/memo/flag_b99b46cb80323cc01a97179b1be6e6d7


#Set Priviledge
RUN chown -R root:memo /home/memo
RUN chmod 750 /home/memo
RUN chown root:memo /home/memo/flag_b99b46cb80323cc01a97179b1be6e6d7
RUN chmod 440 /home/memo/flag_b99b46cb80323cc01a97179b1be6e6d7
RUN mkdir -p /home/memo/db
RUN chown root:memo /home/memo/db
RUN chmod 770 /home/memo/db

#COPY start script
ADD ./start.sh /start.sh

WORKDIR /home/memo
CMD ["/start.sh"]

EXPOSE 8888
