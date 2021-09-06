FROM ubuntu:16.04

#RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/ftp.daumkakao.com\/ubuntu/g" /etc/apt/sources.list
RUN apt-get update && apt-get -y dist-upgrade
RUN apt-get install -yy xinetd

# setup timezone
RUN apt-get install -yy tzdata
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# python3
RUN apt-get install -yy python3
RUN apt-get install -yy python3-pip
RUN pip3 install -U pip
RUN pip3 install coloredlogs

ENV PROB drone

#Set xinetd
COPY ./$PROB.xinetd /etc/xinetd.d/drone
COPY ./start.sh /start.sh

RUN useradd -m $PROB
#COPY ./$PROB.xinetd /etc/xinetd.d/$PROB

#ADD ./src/* /home/$PROB/

ENV PYTHONIOENCODING="UTF-8"

RUN chmod +x /start.sh
#RUN chown -R root:$PROB /home/$PROB
#RUN chmod -R 750 /home/$PROB
#RUN chmod 700 /home/$PROB/drone.py
#RUN chmod 700 /home/$PROB/flag

WORKDIR /home/$PROB
CMD ["/start.sh"]
EXPOSE 42424

