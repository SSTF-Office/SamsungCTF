FROM debian:bullseye

RUN apt update

ARG DEBIAN_FRONTEND=noninteractive
RUN apt -y install --no-install-recommends python3 python3-pip apache2 libapache2-mod-wsgi-py3
RUN apt clean

RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install Pillow==8.2.0
RUN python3 -m pip install Flask==2.1.2
RUN python3 -m pip cache purge

COPY 000-default.conf /etc/apache2/sites-available/

ENTRYPOINT apachectl -D FOREGROUND
