FROM ubuntu:20.04
RUN groupadd -r prob && useradd -r -g prob prob
RUN apt update
RUN apt -y install python3 
WORKDIR /prob
COPY --chown=prob:prob deploy /prob/
RUN chown prob:prob /prob
USER prob
ENTRYPOINT ["python3","sprotocol.py"]
