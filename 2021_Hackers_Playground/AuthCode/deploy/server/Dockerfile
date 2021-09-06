from ubuntu:20.04
MAINTAINER matta <js00n.park@gmail.com>

RUN apt-get update
RUN apt-get install -y xinetd libssl-dev

#Adduser
RUN useradd -s /sbin/nologin challenger

#Copy Binary
COPY src/challenge /home/challenger/challenge

#Set xinetd
COPY challenge.xinetd /etc/xinetd.d/challenge
RUN chmod 754 /etc/init.d/xinetd

#Set flag
COPY src/flag /home/challenger/flag
RUN ln -s /home/challenger/flag /flag

#Set Priviledges
RUN chown -R root:challenger /home/challenger
RUN chmod 750 /home/challenger
RUN chown root:challenger /home/challenger/flag
RUN chmod 440 /home/challenger/flag
RUN chown root:challenger /flag
RUN chmod 440 /flag
RUN chmod 550 /tmp

#COPY start script
COPY start.sh /start.sh
RUN chmod 774 /start.sh

WORKDIR /home/challenger
CMD ["/start.sh"]

EXPOSE 1337
