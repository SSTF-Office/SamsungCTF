from ubuntu:20.04
MAINTAINER matta <js00n.park@gmail.com>

RUN apt update
RUN dpkg --add-architecture i386
RUN apt install -y xinetd
RUN apt install -y libc6-i386

#Adduser
RUN useradd -s /sbin/nologin challenger

#Copy Binary and flag
COPY src/chal /home/challenger/challenge
COPY src/flag.txt /flag.txt
COPY src/setunbuffered32 /usr/bin/setunbuffered32

#Set xinetd
COPY challenge.xinetd /etc/xinetd.d/challenge
RUN chmod 754 /etc/init.d/xinetd

#Set Priviledges
RUN chown -R root:challenger /home/challenger
RUN chmod 750 /home/challenger
RUN chown root:challenger /flag.txt
RUN chmod 440 /flag.txt

#COPY start script
COPY start.sh /start.sh
RUN chmod 774 /start.sh

CMD ["/start.sh"]

EXPOSE 1337
