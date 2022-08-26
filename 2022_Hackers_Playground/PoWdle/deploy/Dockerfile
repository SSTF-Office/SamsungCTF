FROM python:3

RUN apt-get update
RUN apt-get install -y socat

RUN pip install requests timeout-decorator

RUN adduser --shell /sbin/nologin app

COPY . /home/app

RUN chown -R root:app /home/app
RUN chmod -R 750 /home/app
RUN chmod 770 /home/app/HOF_TODO_LIST

WORKDIR /home/app

CMD ["/bin/sh", "start.sh"]
