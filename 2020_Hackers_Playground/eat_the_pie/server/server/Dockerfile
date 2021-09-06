from ubuntu:18.04
MAINTAINER matta <js00n.park@gmail.com>

RUN dpkg --add-architecture i386
RUN apt-get update
RUN apt-get install -y xinetd libc6:i386

#Adduser
RUN useradd -s /sbin/nologin challenger

#Copy Binary
COPY challenge /home/challenger/challenge

#Set xinetd
COPY challenge.xinetd /etc/xinetd.d/challenge
RUN chmod 754 /etc/init.d/xinetd

#Set flag
COPY flag /home/challenger/flag
RUN ln -s /home/challenger/flag /flag

#Set Priviledges
RUN chown -R root:challenger /home/challenger
RUN chmod 750 /home/challenger
RUN chmod 750 /home/challenger/challenge
RUN chown root:challenger /home/challenger/flag
RUN chmod 440 /home/challenger/flag
RUN chown root:challenger /flag
RUN chmod 440 /flag

#COPY start script
COPY start.sh /start.sh
RUN chmod 774 /start.sh

WORKDIR /home/challenger
CMD ["/start.sh"]

EXPOSE 1337
