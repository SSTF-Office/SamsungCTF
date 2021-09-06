FROM ubuntu:16.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
#RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/ftp.daumkakao.com\/ubuntu/g" /etc/apt/sources.list
RUN apt-get update && apt-get -y dist-upgrade
RUN apt-get install rng-tools -y
RUN apt-get install gnupg -y

# setup timezone
RUN apt-get install -yy tzdata
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

ENV PROB leakit

RUN useradd -m $PROB

COPY ./start.sh /start.sh
RUN chmod +x /start.sh
RUN chmod 733 /tmp

#RUN umount /proc
#RUN mkdir -pm 0555 /tmp/aselifhasliehflaise/proc
#RUN mount --bind /proc /tmp/aselifhasliehflaise/proc

USER $PROB
WORKDIR /home/$PROB
CMD ["/start.sh"]

