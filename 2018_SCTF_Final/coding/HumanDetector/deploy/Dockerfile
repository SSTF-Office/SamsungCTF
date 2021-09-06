FROM ubuntu:18.04
MAINTAINER Dongkwan Kim <0xdkay@gmail.com>

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/ap-northeast-2.ec2.archive.ubuntu.com/g" /etc/apt/sources.list
RUN apt-get update && apt-get -y dist-upgrade

# setup timezone
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get install -yy tzdata
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN dpkg-reconfigure --frontend noninteractive tzdata

RUN apt-get install -yy xinetd
RUN apt-get install -yy python-opencv

# python3
RUN apt-get install -yy python3
RUN apt-get install -yy python3-pip
RUN pip3 install -U pip
ADD requirements.txt /tmp/r.txt
RUN pip3 install -r /tmp/r.txt && rm /tmp/r.txt

ENV PROB humandetector
ENV PYTHONIOENCODING="UTF-8"

#Set xinetd
COPY ./$PROB.xinetd /etc/xinetd.d/humandetector
COPY ./start.sh /start.sh
RUN chmod +x /start.sh

RUN useradd -m $PROB
WORKDIR /home/$PROB

CMD ["/start.sh"]
EXPOSE 42424
