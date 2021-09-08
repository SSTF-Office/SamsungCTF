FROM ubuntu:17.10
MAINTAINER Gyeongwon Kim<sutt69@kaist.ac.kr>

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/ftp.daumkakao.com/g" /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -y python python-pip
RUN dpkg --add-architecture i386
RUN apt-get update
RUN apt-get install -y libc6 libc6-dev libc6-dev:i386 gcc gcc-multilib libseccomp-dev libseccomp-dev:i386

RUN pip install --upgrade pip

RUN useradd -ms /bin/bash abc
WORKDIR /home/abc

COPY ./requirements.txt /home/abc/requirements.txt

RUN pip install -r requirements.txt

COPY ./flag /home/abc/flag
COPY ./prob /home/abc/prob
COPY ./run.py /home/abc/run.py
COPY ./sleep.sh /home/abc/sleep.sh
COPY ./run.sh /home/abc/run.sh
COPY ./tester /home/abc/tester
COPY ./abc_default /home/abc/abc_default

RUN chown -R root:abc /home/abc
RUN chmod 750 /home/abc
RUN chown root:abc /home/abc/flag
RUN chmod 440 /home/abc/flag

CMD ["./sleep.sh"]

EXPOSE 55555
