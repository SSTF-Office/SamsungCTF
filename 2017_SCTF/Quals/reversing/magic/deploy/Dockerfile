FROM ubuntu:16.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN apt-get update && apt-get -y dist-upgrade

RUN apt-get install -yy tzdata
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get install -yy socat

ENV PROB magic
RUN useradd -m $PROB

COPY ./start.sh /start.sh

ADD ./src/* /home/$PROB/

RUN chmod +x /home/magic/magic
RUN chmod +x /start.sh
RUN chmod 400 /home/$PROB/flag

RUN mkdir -p /tmp/log/

WORKDIR /home/$PROB
CMD ["/start.sh"]
EXPOSE 23972
