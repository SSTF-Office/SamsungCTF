FROM ubuntu:18.10
MAINTAINER Mingeun Kim <pr0v3rbs@kaist.ac.kr>

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN apt-get update && apt-get -y upgrade
RUN apt-get install -y xinetd

# chat_server
ENV PROB chat_server

RUN useradd -m $PROB
WORKDIR /home/$PROB

COPY $PROB /home/$PROB/$PROB
COPY ./flag /home/$PROB/flag
RUN chown -R root:$PROB /home/$PROB
RUN chmod 750 /home/$PROB
RUN chmod 750 /home/$PROB/$PROB

# chat_client
ENV PROB chat_client

RUN useradd -m $PROB
WORKDIR /home/$PROB

COPY $PROB /home/$PROB/$PROB
COPY ./client_flag /home/$PROB/flag
RUN chown -R root:$PROB /home/$PROB
RUN chmod 750 /home/$PROB
RUN chmod 750 /home/$PROB/$PROB

# Run setup
COPY ./$PROB.xinetd /etc/xinetd.d/$PROB
COPY ./start.sh /start.sh
RUN chmod +x /start.sh

CMD ["/start.sh"]

# Port for client
EXPOSE 13137
