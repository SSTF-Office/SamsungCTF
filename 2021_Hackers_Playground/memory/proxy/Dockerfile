FROM ubuntu
RUN apt-get update \
        && apt-get -y install --no-install-recommends \
            socat \
            docker.io 
ENTRYPOINT ["socat","TCP-LISTEN:31339,reuseaddr,fork","EXEC:docker run --rm -i memory"]
#ENTRYPOINT ["/bin/sh"]
