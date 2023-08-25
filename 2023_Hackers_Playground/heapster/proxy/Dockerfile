FROM ubuntu:22.04

RUN apt update
RUN apt install -y --no-install-recommends socat docker.io
COPY ["./exec.sh", "/exec.sh"]
RUN chmod +x /exec.sh

ENTRYPOINT ["socat","TCP-LISTEN:31339,reuseaddr,fork","EXEC:/exec.sh"]


