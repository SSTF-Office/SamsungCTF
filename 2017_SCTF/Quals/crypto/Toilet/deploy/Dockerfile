FROM ubuntu:16.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -y python
RUN apt-get install -y python-pip
RUN apt-get install -y nginx-full

ADD requirements.txt /tmp/r.txt
RUN pip install -r /tmp/r.txt && rm /tmp/r.txt
RUN ln -s /usr/local/bin/uwsgi /usr/bin/uwsgi

RUN mkdir -p /app
ADD src/ /app/

ADD uwsgi.ini /tmp
RUN rm /etc/nginx/sites-available/default
ADD nginx.conf /etc/nginx/sites-available/default

RUN rm -rf /var/lib/apt/lists/*
ADD start.sh /start.sh
RUN chmod 755 /*.sh

RUN chmod 555 /app/*


EXPOSE 80
CMD ["/bin/sh", "/start.sh"]
