FROM ubuntu:22.04
RUN apt-get update
RUN apt install -y wget
RUN useradd -d /home/guest guest

COPY ./mario /home/guest/mario
COPY ./info.sh /home/guest/info.sh
RUN chmod +x /home/guest/info.sh

COPY ./flag /home/guest/flag
RUN chmod 600 /home/guest/flag

ENTRYPOINT ["timeout","60","runuser", "guest", "-c", "/home/guest/mario"]

