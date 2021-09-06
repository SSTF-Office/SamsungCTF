FROM ubuntu:16.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -y python3
RUN apt-get install -y python3-pip

# setup timezone
RUN apt-get install -yy tzdata
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

ADD requirements.txt /tmp/r.txt
RUN pip3 install -r /tmp/r.txt && rm /tmp/r.txt

RUN mkdir -p /app
ADD src/ /app/

RUN useradd -m waas

RUN rm -rf /var/lib/apt/lists/*
RUN chmod 555 /app/*
RUN chmod 755 /app/run.sh
RUN touch /app/frontend.log
RUN touch /app/backend.log
RUN touch /app/admin.log
RUN chown root.waas /app/*.log
RUN chmod 660 /app/*.log

EXPOSE 8080
USER waas
CMD cd /app; ./run.sh
