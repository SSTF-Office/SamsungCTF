FROM node:lts-alpine3.15

LABEL maintainer=Einstrasse

RUN mkdir -p /f14g9g9g9g9g9g9g9g9g9g9g/p/a/t/h/hhhhhhhhhhhhhheer3232r23r2
COPY ./server /server
COPY ./flag.txt /f14g9g9g9g9g9g9g9g9g9g9g/p/a/t/h/hhhhhhhhhhhhhheer3232r23r2/flag.txt

EXPOSE 3000

USER root
WORKDIR /server

RUN npm install && npm install --location=global forever
RUN chmod 555 -R /server && chmod 444 /f14g9g9g9g9g9g9g9g9g9g9g/p/a/t/h/hhhhhhhhhhhhhheer3232r23r2/flag.txt


ENTRYPOINT ["su", "node", "-c", "forever app.js"]