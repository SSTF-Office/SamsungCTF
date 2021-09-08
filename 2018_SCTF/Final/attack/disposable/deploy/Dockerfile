FROM ubuntu:16.04
MAINTAINER Chihyun Song <ian0371@kaist.ac.kr>

RUN apt update && apt -y upgrade

RUN apt install -y tzdata
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt install -y socat

ENV PROB disposable
RUN useradd -m $PROB
WORKDIR /home/$PROB

COPY ./start.sh /start.sh
ADD ./home/* /home/$PROB/
RUN chmod +x /start.sh

RUN chown -R root:$PROB /home/$PROB
RUN chmod 750 /home/$PROB
RUN chmod 440 /home/$PROB/flag

USER $PROB
CMD ["/start.sh"]

EXPOSE 30010
