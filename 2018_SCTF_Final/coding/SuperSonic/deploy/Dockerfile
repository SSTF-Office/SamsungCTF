FROM ubuntu:16.04
MAINTAINER Minjoon Park <dinggul@kaist.ac.kr>

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/ftp.daumkakao.com/g" /etc/apt/sources.list
RUN apt update && apt -y upgrade

RUN apt install -y tzdata
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt install -y socat
RUN apt install -y python
RUN apt install -y python-pip
RUN pip install requests

ENV PROB SuperSonic
RUN useradd -m $PROB

COPY ./start.sh /start.sh
ADD ./src/* /home/$PROB/

RUN chmod +x /start.sh
RUN chmod 400 /home/$PROB/flag

RUN mkdir -p /tmp/log/

WORKDIR /home/$PROB
CMD ["/start.sh"]
EXPOSE 22341
