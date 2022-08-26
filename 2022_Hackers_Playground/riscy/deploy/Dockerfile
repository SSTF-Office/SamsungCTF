FROM ubuntu:20.04

ARG PORT
ARG DEBIAN_FRONTEND=noninteractive
RUN apt update -y
RUN apt install -y xinetd qemu-user

RUN useradd -M -s /bin/bash riscy
RUN mkdir -m 0755 /home/riscy
WORKDIR /home/riscy

COPY riscy.xinetd /etc/xinetd.d/riscy
RUN sed -i "s/PORT/$PORT/" /etc/xinetd.d/riscy
COPY flag /flag

COPY target target
COPY run-xinetd.sh run-xinetd.sh
COPY run-riscy.sh run-riscy.sh
COPY qemu-riscv64 /usr/bin/qemu-riscv64

CMD ["/home/riscy/run-xinetd.sh"]

EXPOSE $PORT
