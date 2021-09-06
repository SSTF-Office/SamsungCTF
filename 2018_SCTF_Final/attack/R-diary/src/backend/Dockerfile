FROM node:8.10

RUN sed -i "s/http:\/\/deb.debian.org/http:\/\/ftp.daumkakao.com/g" /etc/apt/sources.list

RUN apt update
RUN apt -y install netcat
WORKDIR /usr/src/app
ADD src/package-lock.json ./
ADD src/package.json ./
RUN npm install -g
RUN npm install -g forever

# rabbitmq
RUN apt-get install -yy rabbitmq-server python python-pip
#RUN apt-get install -yy iceweasel Xvfb wget tar

WORKDIR /usr/src/app

RUN mkdir -p /var/log
RUN adduser --disabled-password --gecos "" rdiary
RUN chown -R root:rdiary /usr/src/
RUN chmod -R 750 /usr/src/
ADD run.sh /run.sh
ADD forever.sh /forever.sh
RUN chmod +x /run.sh

# Add source
COPY src/ ./
#ADD bot/* /

EXPOSE 3001
CMD ["/run.sh"]
