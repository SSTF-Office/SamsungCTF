FROM ubuntu:20.04

#Install xinetd
USER root
RUN apt-get update
RUN apt-get install -y xinetd

RUN useradd -b /home/lostark  lostark
WORKDIR /home/lostark

ADD  ./a.out /home/lostark/a.out
ADD lostark.xinetd /etc/xinetd.d/lostark
ADD flag /home/lostark/flag
ADD flag /flag
ADD ./start.sh /start.sh

RUN chown -R root:lostark /home/lostark
RUN chmod 750 /home/lostark
RUN chmod 550 /home/lostark/a.out
RUN chmod 440 /home/lostark/flag
RUN chown root:lostark /flag
RUN chmod 440 /flag
RUN chmod +x /start.sh

USER lostark
CMD ["/start.sh"]

EXPOSE 31678
