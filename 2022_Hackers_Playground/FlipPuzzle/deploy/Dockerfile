FROM ubuntu:18.04

RUN apt-get update && apt-get install -y xinetd python3\
    && useradd -b /home/guest -s /bin/bash guest

ADD app.py /app.py
ADD prob.xinetd /etc/xinetd.d/prob
ADD start.sh /start.sh

RUN chmod +x /app.py && chmod +x /start.sh

CMD ["/start.sh"]

EXPOSE 8080

