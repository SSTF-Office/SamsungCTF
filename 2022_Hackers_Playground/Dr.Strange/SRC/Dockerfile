FROM debian
RUN apt-get update
RUN apt-get -y install --no-install-recommends netcat procps wget 
RUN apt-get -y install gcc
RUN useradd -d /home/guest guest
RUN useradd -d /home/prob prob

COPY ./AttackMe /
COPY ./libpd.so /lib/
COPY ./run.sh /run.sh
COPY ./flag /flag
RUN chown prob:prob /flag
RUN chmod 600 /flag
RUN chmod +x /AttackMe
RUN chmod +x /run.sh

#ENTRYPOINT ["/run.sh"]
ENTRYPOINT ["timeout","300","/run.sh"]
