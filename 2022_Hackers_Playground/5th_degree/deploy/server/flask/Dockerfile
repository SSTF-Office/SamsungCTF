FROM python:3

RUN pip install flask uwsgi pytz Flask-Session

RUN useradd -s /sbin/nologin app

USER app

WORKDIR /home/app
