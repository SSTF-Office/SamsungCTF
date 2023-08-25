FROM python:3

RUN apt-get update -y
RUN apt-get install libpq-dev -y

RUN pip3 install django==4.0.5
RUN pip3 install psycopg2

COPY src /root/libreria
COPY init.json /root/libreria/init.json

WORKDIR /root/libreria

EXPOSE 8000
