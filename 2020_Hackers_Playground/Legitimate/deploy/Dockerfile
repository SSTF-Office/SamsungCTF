FROM nginx:alpine

RUN apk add --no-cache git

WORKDIR /usr/share/nginx/html/

COPY index.html index.html
COPY style.css style.css
RUN rm /etc/nginx/conf.d/default.conf
COPY default.conf /etc/nginx/conf.d/default.conf

COPY legit-init.sh legit-init.sh
RUN chmod u+x legit-init.sh
RUN ./legit-init.sh
RUN rm legit-init.sh
