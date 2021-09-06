FROM ubuntu:16.04
FROM microsoft/dotnet:latest

RUN DIST=xenial && \
    sed -i 's/deb.debian.org/ftp.daumkakao.com/' /etc/apt/sources.list && \
    apt-get update
RUN apt-get install -y xinetd

COPY Lambda /Lambda
COPY chal /chal

RUN cd /chal && \
      dotnet build -c release


ADD chal.xinetd /etc/xinetd.d/chal
ADD chal.sh /
ADD start.sh /

RUN chmod +x /chal.sh
RUN chmod +x /start.sh


EXPOSE 42
CMD ["/start.sh"]
