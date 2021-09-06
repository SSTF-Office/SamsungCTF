FROM ubuntu:16.04

ENV USER crashcollector
ENV FLAG flag

#Adduser
RUN useradd -b /home/$USER $USER

#Copy Binary
ADD $USER /home/$USER/$USER
ADD libc-2.23.so /crash-libc.so

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

EXPOSE 7778


