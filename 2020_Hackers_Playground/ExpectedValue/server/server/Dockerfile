from ubuntu:18.04
MAINTAINER matta <js00n.park@gmail.com>

RUN apt-get update
RUN apt-get install -y xinetd python3

#Adduser
RUN useradd -s /sbin/nologin challenger

#Copy Binary
COPY . /root/

#Set xinetd
COPY challenge.xinetd /etc/xinetd.d/challenge
RUN chmod 750 /etc/init.d/xinetd

#COPY start script
COPY start.sh /start.sh
RUN chmod 774 /start.sh

WORKDIR /home/challenger
CMD ["/start.sh"]

EXPOSE 1337
