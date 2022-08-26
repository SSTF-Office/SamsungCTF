FROM ubuntu:22.04
RUN apt-get update
#RUN apt-get -y install --no-install-recommends socat docker.io
RUN apt-get -y install socat docker.io
#COPY ["ubuntu22.tar","ubuntu22.tar"]
COPY ["pipe0.tar","pipe0.tar"]
COPY ["pipe1.tar","pipe1.tar"]
COPY ["pipe2.tar","pipe2.tar"]
COPY ["pipe3.tar","pipe3.tar"]
COPY ["pipe4.tar","pipe4.tar"]
COPY ["pipe5.tar","pipe5.tar"]
COPY ["pipe6.tar","pipe6.tar"]
COPY ["pipe7.tar","pipe7.tar"]
COPY ["pipe8.tar","pipe8.tar"]
COPY ["pipe9.tar","pipe9.tar"]
COPY ["connection_count","connection_count"]
COPY ["exec.sh","exec.sh"]

ENTRYPOINT ["socat","TCP-LISTEN:31338,reuseaddr,fork","EXEC:/exec.sh"]

