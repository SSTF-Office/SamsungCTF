FROM ubuntu:22.04
RUN apt-get update
RUN apt-get -y install --no-install-recommends socat docker.io

COPY ["exec.sh","/exec.sh"]
ENTRYPOINT ["socat","TCP-LISTEN:31339,reuseaddr,fork","EXEC:/exec.sh"]

