FROM node:8.10

RUN sed -i "s/http:\/\/deb.debian.org/http:\/\/ftp.daumkakao.com/g" /etc/apt/sources.list

WORKDIR /usr/src/app
RUN apt update && apt install -yy xsel
ADD package*.json ./
RUN npm install -g
RUN npm install -g forever
ADD package-lock.json package.json /usr/src/app/
RUN mkdir -p /var/log
RUN adduser --disabled-password --gecos "" rdiary
RUN chown -R root:rdiary /usr/src/
RUN chmod -R 750 /usr/src/
ADD ./serve ./serve
RUN cd serve && npm link
ADD . .
RUN npm run build
RUN rm -rf package-lock.json package.json Dockerfile src public node_modules/
RUN sed -i 's/\"static/\"\/static/g' /usr/src/app/build/index.html

ADD run.sh /run.sh
ADD forever.sh /forever.sh
RUN chmod +x /run.sh /forever.sh

EXPOSE 3000
CMD ["/run.sh"]
