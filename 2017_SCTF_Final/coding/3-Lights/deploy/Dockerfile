FROM ubuntu:16.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN dpkg --add-architecture i386
RUN apt-get update

RUN apt-get install -yy tzdata
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get install -yy libc6:i386
RUN apt-get install -yy socat
RUN apt-get install -yy python
RUN apt-get install -yy python-pip
RUN apt-get install -yy gcc
RUN apt-get install -y libssl-dev
RUN apt-get install -yy libc6 libc6-dev
RUN apt-get install -yy  g++-multilib
RUN apt-get install -yy unzip wget
RUN apt-get install -yy libffi-dev

RUN pip install --upgrade pip

RUN pip install PyQRCode
RUN pip install requests

RUN mkdir -p /tmp/log

ENV PROB lights
RUN useradd -m $PROB

COPY ./start.sh /start.sh
ADD ./src/* /home/$PROB/
RUN chmod +x /start.sh

WORKDIR /home/$PROB
CMD ["/start.sh"]
EXPOSE 22341
