FROM ubuntu:20.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN dpkg --add-architecture i386
RUN apt-get update
RUN apt-get install -y xinetd
RUN apt-get install -y libc6:i386

#Adduser
RUN useradd -b /home/texpress texpress -s /bin/bash

#Copy Binary
ADD t_express /home/texpress/t_express

#Set xinetd
ADD texpress.xinetd /etc/xinetd.d/texpress

#Set flag
ADD flag.txt /home/texpress/flag.txt
RUN ln -s /home/texpress/flag.txt /flag.txt

#Set Priviledges
RUN chown -R root:texpress /home/texpress
RUN chmod 750 /home/texpress
RUN chmod 750 /home/texpress/t_express
RUN chown root:texpress /home/texpress/flag.txt
RUN chmod 440 /home/texpress/flag.txt
RUN chown root:texpress /flag.txt
RUN chmod 440 /flag.txt

#COPY start script
ADD start.sh /start.sh
RUN chmod +x /start.sh

WORKDIR /home/texpress
CMD ["/start.sh"]
# CMD ["/bin/bash"]

EXPOSE 1337
