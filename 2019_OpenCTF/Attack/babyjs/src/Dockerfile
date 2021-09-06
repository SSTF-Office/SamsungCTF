FROM node:11
RUN useradd -ms /bin/bash babyjs
WORKDIR /home/babyjs

COPY ./flag /flag
COPY ./web .

RUN chmod -R 555 .
RUN chmod -R 777 ./sessions
RUN chmod -R 500 ./bin 
ENV PATH /home/babyjs/bin:$PATH

EXPOSE 30002

CMD ["node", "/home/babyjs/app.js"]
