FROM ubuntu:16.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/ftp.daumkakao.com/g" /etc/apt/sources.list
RUN sed -Ei 's/^# deb-src/deb-src/' /etc/apt/sources.list
RUN apt-get update && apt-get -y dist-upgrade
RUN apt-get build-dep -yy qemu
RUN apt-get install -yy xinetd linux-libc-dev-arm64-cross libc6-arm64-cross 2.19

ENV PROB DungeonQuest

RUN useradd -m $PROB
COPY ./$PROB.xinetd /etc/xinetd.d/$PROB
COPY ./start.sh /start.sh

ADD ./bin/* /home/$PROB/

RUN chmod +x /start.sh
RUN chown -R root:$PROB /home/$PROB
RUN chmod -R 750 /home/$PROB
RUN chmod 740 /home/$PROB/flag

WORKDIR /home/$PROB
CMD ["/start.sh"]
EXPOSE 9999
