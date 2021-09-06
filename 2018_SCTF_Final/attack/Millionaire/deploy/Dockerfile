FROM ubuntu:16.04
MAINTAINER Eunsoo Kim <hahah.kim@gmail.com>

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
#RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/ftp.daumkakao.com\/ubuntu/g" /etc/apt/sources.list

RUN apt-get update && apt-get -y dist-upgrade
RUN apt-get install -y software-properties-common
RUN add-apt-repository ppa:deadsnakes/ppa
RUN add-apt-repository ppa:ethereum/ethereum
RUN apt-get update
RUN apt-get install -y build-essential python3.6 python3.6-dev python3-pip python3.6-venv libpython3.6-dev
RUN apt-get install -y solc socat

# setup timezone
RUN apt-get install -yy tzdata
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# setup web3
COPY requirements.txt /tmp/r.txt
RUN python3.6 -m pip install -U pip
RUN python3.6 -m pip install -r /tmp/r.txt && rm /tmp/r.txt

ENV PROB millionaire 

RUN useradd -m $PROB

COPY ./src/* /home/$PROB/ 
RUN mkdir /home/$PROB/log
RUN chmod 777 /home/$PROB/log
RUN chmod +x /home/$PROB/socat.sh
ENV LC_ALL C.UTF-8

EXPOSE 12345
USER $PROB
WORKDIR /home/$PROB
CMD ["./socat.sh"]

