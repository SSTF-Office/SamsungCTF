FROM ubuntu:16.04

ENV PROB MultiLingual

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/ftp.daumkakao.com/g" /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -y xinetd python python-pip

RUN useradd -m $PROB
RUN pip install unicorn
COPY ./$PROB.xinetd /etc/xinetd.d/$PROB
COPY ./start.sh /start.sh

ADD ./bin/* /home/$PROB/

RUN chmod +x /start.sh
RUN chown -R root:$PROB /home/$PROB
RUN chmod -R 750 /home/$PROB

WORKDIR /home/$PROB
CMD ["/start.sh"]
EXPOSE 9999
