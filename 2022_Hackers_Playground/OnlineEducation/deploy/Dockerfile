FROM ubuntu:20.04

RUN sed -i 's/archive.ubuntu.com/mirror.kakao.com/g' /etc/apt/sources.list
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -yqq wkhtmltopdf
RUN apt-get install -yqq python3-pip
RUN python3 -m pip install flask pdfkit uwsgi

RUN adduser --shell /sbin/nologin app

COPY . /home/app

RUN chown -R root:app /home/app
RUN chmod -R 750 /home/app

WORKDIR /home/app
RUN mv flag flag_`md5sum flag | cut -d' ' -f1`

USER app
CMD ["uwsgi", "uwsgi.ini"]

