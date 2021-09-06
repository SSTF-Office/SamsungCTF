FROM ubuntu:16.04

ENV USER dvchat
ENV FLAG flag_802743cb7dfca891af0ed89056fa3c3d

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/kr.archive.ubuntu.com/g" /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -y libncurses-dev libseccomp-dev

#Adduser
RUN useradd -b /home/$USER $USER

#Copy Binary
ADD $USER /home/$USER/$USER
ADD libc-2.23.so /dvchat-libc.so

#Set Flag
ADD flag /home/$USER/$FLAG

#Set Priviledge
RUN chown -R root:$USER /home/$USER
RUN chmod 750 /home/$USER
RUN chmod 750 /home/$USER/$USER
RUN chmod 440 /home/$USER/$FLAG

#COPY start script
ADD ./super.pl /

WORKDIR /home/$USER
CMD ["perl", "/super.pl"]

EXPOSE 7779


