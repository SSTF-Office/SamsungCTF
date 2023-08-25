FROM node:16

WORKDIR /app

COPY package.json .
RUN npm install

COPY public /app/public
COPY plugins /app/plugins
COPY routes /app/routes
COPY app.js .


CMD [ "npm", "run", "dev" ]