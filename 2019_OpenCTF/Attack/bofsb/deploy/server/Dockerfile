FROM ubuntu:18.04
MAINTAINER matta <js00n.park@gmail.com>

#Install xinetd
#Install xinetd
RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN dpkg --add-architecture i386
RUN apt-get update
RUN apt-get install -y xinetd 
RUN apt-get install -y libc6:i386

#Adduser
RUN useradd -b /home/bofsb bofsb -s /bin/bash

#Copy Binary
ADD bofsb /home/bofsb/bofsb

#Set xinetd
ADD bofsb.xinetd /etc/xinetd.d/bofsb

#Set flag
ADD flag /home/bofsb/flag
RUN ln -s /home/bofsb/flag /flag

#Set Priviledges
RUN chown -R root:bofsb /home/bofsb
RUN chmod 750 /home/bofsb
RUN chown root:bofsb /home/bofsb/flag
RUN chmod 440 /home/bofsb/flag
RUN chown root:bofsb /flag
RUN chmod 440 /flag

#COPY start script
ADD start.sh /start.sh
RUN chmod +x /start.sh

WORKDIR /home/bofsb
CMD ["/start.sh"]

EXPOSE 1337

