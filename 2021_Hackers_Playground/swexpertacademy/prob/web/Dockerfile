FROM node:14-alpine3.11

RUN mkdir -p /server

ADD ./ /server/

WORKDIR /server
RUN npm install

EXPOSE 3000

ENV EXECURL="http://executor:3000"

ENTRYPOINT ["npm", "start"]