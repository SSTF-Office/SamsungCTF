FROM debian
RUN apt-get update
#RUN apt-get -y install --no-install-recommends socat docker.io
RUN apt-get -y install socat docker.io
COPY ["exec.sh","exec.sh"]

ENTRYPOINT ["socat","TCP-LISTEN:31340,reuseaddr,fork","EXEC:/exec.sh"]
