FROM ubuntu:16.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN apt-get update && apt-get -y dist-upgrade

RUN apt-get install -yy tzdata
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get install -yy socat
RUN apt-get install -yy python
RUN apt-get install -yy git
RUN apt-get install -yy cython
RUN apt-get install -yy wget
RUN apt-get install -yy unzip
RUN apt-get install -yy make
RUN apt-get install -yy automake
RUN apt-get install -yy libtool

ENV PROB readflag
RUN useradd -m $PROB

WORKDIR /tmp
RUN wget https://github.com/seccomp/libseccomp/archive/master.zip
RUN unzip master.zip
WORKDIR /tmp/libseccomp-master/
RUN ./autogen.sh
RUN ./configure
RUN make

WORKDIR /tmp/libseccomp-master/src/python
RUN make build
RUN make install
RUN cp /usr/local/lib/python2.7/site-packages/seccomp.so /usr/local/lib/python2.7/dist-packages/

COPY ./start.sh /start.sh
ADD ./src/* /home/$PROB/
RUN chmod +x /start.sh

WORKDIR /home/$PROB
CMD ["/start.sh"]
EXPOSE 55402
