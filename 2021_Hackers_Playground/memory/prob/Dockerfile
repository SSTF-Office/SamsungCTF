FROM ubuntu:18.04
COPY ./libcrypto.so.1.1 /usr/lib/x86_64-linux-gnu/
RUN groupadd -r prob && useradd -r -g prob prob
RUN apt update
RUN apt -y install bsdmainutils
RUN apt -y install socat
WORKDIR /prob
COPY --chown=prob:prob deploy /prob/
RUN chown prob:prob /prob
USER prob
#USER root
#RUN ls -al 
#RUN pwd
ENTRYPOINT ["/prob/prob"]
#ENTRYPOINT ["socat","TCP-LISTEN:31339,reuseaddr,fork","EXEC:/prob/prob"]
#ENTRYPOINT ["/bin/sh"]

