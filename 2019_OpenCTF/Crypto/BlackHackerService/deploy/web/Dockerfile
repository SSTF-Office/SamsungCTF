FROM ubuntu:latest

RUN apt-get update && apt-get install -y python python-pip nginx netcat mysql-client python-mysqldb
COPY task.tgz /var/run/

RUN  cd /var/run && tar -zvxf task.tgz
RUN pip install -r /var/run/task/requirements.txt

EXPOSE 8080

ENTRYPOINT ["/var/run/task/run.sh"]
