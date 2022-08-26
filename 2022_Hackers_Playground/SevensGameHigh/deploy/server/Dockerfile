from ubuntu:20.04

RUN apt-get update
RUN apt-get install -y xinetd language-pack-en

#Adduser
RUN useradd -s /sbin/nologin challenger

#Copy Binary and flag
COPY src/challenge /home/challenger/challenge
COPY src/flag.txt /flag.txt

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

EXPOSE 7777
