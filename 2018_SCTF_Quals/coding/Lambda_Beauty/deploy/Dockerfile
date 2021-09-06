FROM ubuntu:16.04
FROM microsoft/dotnet:latest

RUN DIST=xenial && \
    sed -i 's/deb.debian.org/ftp.daumkakao.com/' /etc/apt/sources.list && \
    apt-get update
RUN apt-get install -y xinetd

COPY Lambda /Lambda
COPY chal /chal
COPY repl /repl

RUN cd /repl && \
      dotnet build -c repl

RUN cd /chal && \
      dotnet build -c release


ADD repl.xinetd /etc/xinetd.d/repl
ADD chal.xinetd /etc/xinetd.d/chal
ADD repl.sh /
ADD chal.sh /
ADD start.sh /

RUN chmod +x /repl.sh
RUN chmod +x /chal.sh
RUN chmod +x /start.sh


EXPOSE 42
EXPOSE 4242
CMD ["/start.sh"]
