FROM node:14-alpine3.11

RUN mkdir -p /server

COPY flag.txt /
COPY *.txt /server/
COPY app.js /server
COPY package.json /server

WORKDIR /server
RUN addgroup -S einegroup && adduser -S eineuser -G einegroup &&\
    apk add --no-cache build-base bash &&\
    npm install &&\
    chmod 444 /flag.txt && chmod 555 -R /server
USER eineuser

EXPOSE 3000

ENTRYPOINT ["node", "app.js"]