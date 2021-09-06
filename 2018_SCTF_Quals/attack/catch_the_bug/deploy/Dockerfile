FROM ubuntu:17.10

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -y xinetd

RUN useradd -ms /bin/bash bug
WORKDIR /home/bug

COPY ./xinetd_file /etc/xinetd.d/bug
COPY ./flag /home/bug/flag
COPY ./bug /home/bug/bug
COPY ./start.sh /home/bug/start.sh
COPY ./libc-2.26.so /lib/x86_64-linux-gnu/libc-2.26.so
RUN echo "bug     55555/tcp" >> /etc/services

RUN chown -R root:bug /home/bug
RUN chmod 750 /home/bug
RUN chown root:bug /home/bug/flag
RUN chmod 440 /home/bug/flag

USER bug
CMD ["./start.sh"]

EXPOSE 55555
