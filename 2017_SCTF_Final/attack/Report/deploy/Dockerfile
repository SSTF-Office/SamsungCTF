FROM ubuntu:16.04

#Install xinetd
RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN apt-get update && apt-get -y dist-upgrade
RUN apt-get install -y xinetd

# setup timezone
RUN apt-get install -yy tzdata
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

#Adduser
RUN useradd -b /home/report -p $(perl -e 'print crypt("Def4ultP4ssw0rd", "password")' $password) report

#Copy Binary
ADD Report /home/report/Report

#Set xinetd
RUN echo "service Report\n{\n        type = UNLISTED\n        disable = no\n        socket_type = stream\n        protocol = tcp\n        user = report" > /etc/xinetd.d/Report
RUN echo "        wait = no\n        server = /home/report/Report\n        port = 55555\n        env = HOME=/home/report/\n}\n" >> /etc/xinetd.d/Report


#Set Flag
RUN echo "SCTF{Ch41n_oF_C0unT3rFe1t_oBj3cts}" > /home/report/flag

#Set Priviledge
RUN chown -R root:report /home/report
RUN chmod 750 /home/report 
RUN chown root:report /home/report/flag
RUN chmod 440 /home/report/flag


#COPY start script
ADD ./start.sh /start.sh

WORKDIR /home/report
CMD ["/start.sh"]

EXPOSE 55555
