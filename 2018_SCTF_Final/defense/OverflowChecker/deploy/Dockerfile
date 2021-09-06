FROM ubuntu:16.04
MAINTAINER JIHEE PARK <j31d0@kaist.ac.kr>

#Install xinetd
RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN echo "deb http://dl.bintray.com/sbt/debian /" | tee -a /etc/apt/sources.list.d/sbt.list
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 2EE0EA64E40A89B84B2DF73499E82A75642AC823
RUN apt-get update
RUN apt-get install -y xinetd 
RUN apt-get install -y curl
RUN curl -O https://www.scala-lang.org/files/archive/scala-2.11.6.deb 
RUN apt-get install -y software-properties-common
RUN add-apt-repository ppa:openjdk-r/ppa
RUN apt-get update
RUN apt-get install -y openjdk-7-jdk
RUN dpkg -i scala-2.11.6.deb

#Adduser
RUN useradd -b /home/ofd  ofd
#Copy Binary

ADD ./prob.jar /home/ofd/

#Set xinetd
ADD ofd.xinetd /etc/xinetd.d/ofd

#Set Flag
ADD run.sh /home/ofd/run.sh

#Set Priviledge
RUN chown -R root:ofd /home/ofd
RUN chown root:ofd /home/ofd/run.sh
RUN chmod 755 /home/ofd/run.sh

#COPY start script
ADD ./start.sh /start.sh

WORKDIR /home/ofd
CMD ["/start.sh"]

EXPOSE 8888
