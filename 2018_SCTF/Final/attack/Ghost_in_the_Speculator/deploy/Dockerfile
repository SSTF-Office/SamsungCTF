FROM ubuntu:16.04

ENV PROB Ghost_in_the_Speculator

RUN apt-get update
RUN apt-get install -y xinetd python

RUN useradd -m $PROB
COPY ./$PROB.xinetd /etc/xinetd.d/$PROB
COPY ./start.sh /start.sh

ADD ./bin/* /home/$PROB/

RUN chmod +x /start.sh
RUN chown -R root:$PROB /home/$PROB
RUN chmod -R 750 /home/$PROB
RUN chmod 740 /home/$PROB/flag

WORKDIR /home/$PROB
CMD ["/start.sh"]
EXPOSE 9999
