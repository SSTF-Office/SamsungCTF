from ubuntu:22.04
MAINTAINER matta <js00n.park@gmail.com>

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/ftp.daumkakao.com/g" /etc/apt/sources.list
RUN sed -i "s/http:\/\/security.ubuntu.com/http:\/\/ftp.daumkakao.com/g" /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -y xinetd

#Adduser
RUN useradd -s /sbin/nologin challenger

#Copy Binary
COPY src/challenge /home/challenger/challenge

#Set xinetd
COPY challenge.xinetd /etc/xinetd.d/challenge
RUN chmod 754 /etc/init.d/xinetd

#Set Priviledges
RUN chown -R root:challenger /home/challenger
RUN chmod 750 /home/challenger
RUN chmod 550 /tmp

#COPY start script
COPY start.sh /start.sh
RUN chmod 774 /start.sh

WORKDIR /home/challenger
CMD ["/start.sh"]

EXPOSE 1337
