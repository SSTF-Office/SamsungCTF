FROM ubuntu:16.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/ftp.daumkakao.com/g" /etc/apt/sources.list
RUN apt update && apt -y upgrade

RUN apt install -y tzdata
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt install -y python3
RUN apt install -y python3-pip
RUN pip3 install pycryptodomex

RUN apt-get install -y xinetd

ENV PROB SaltyDSA
RUN useradd -m $PROB
WORKDIR /home/$PROB

COPY ./start.sh /start.sh
ADD ./server1.xinetd /etc/xinetd.d/server1
ADD ./server2.xinetd /etc/xinetd.d/server2
ADD ./src/* /home/$PROB/
RUN chmod +x /start.sh

RUN chown -R $PROB:$PROB /home/$PROB
USER $PROB

CMD ["/start.sh"]
EXPOSE 12345 54321
