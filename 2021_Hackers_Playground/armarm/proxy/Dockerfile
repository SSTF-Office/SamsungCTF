FROM ubuntu:18.04
RUN apt-get update
RUN apt-get -y install --no-install-recommends socat docker.io 

ENTRYPOINT ["socat","TCP-LISTEN:31338,reuseaddr,fork","EXEC:docker run --rm -i \"armarm:1.0\""]
#ENTRYPOINT ["/bin/bash"]

