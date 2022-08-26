FROM ubuntu:22.04
RUN apt-get update
RUN apt-get -y install --no-install-recommends python3-minimal netcat socat vim
RUN useradd -d /home/prob prob
RUN useradd -d /home/guest guest

COPY ./AttackMe.py /AttackMe.py
COPY ./flag /flag
COPY ./init.sh /init.sh

RUN chmod +x /init.sh
RUN chmod +x /AttackMe.py
RUN chown prob:prob ./flag
RUN chmod 700 ./flag

ENTRYPOINT ["timeout","60","/init.sh"]

