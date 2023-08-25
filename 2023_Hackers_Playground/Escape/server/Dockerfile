FROM ubuntu:22.04

RUN apt update && apt install -y socat
RUN set -e -x ;\
    groupadd -g 1337 escape ;\
    useradd -g 1337 -u 1337 escape

ADD escape /escape
ADD flag /flag

RUN set -e -x ;\
    chmod a+r /flag ;\
    chmod a+rx /escape

USER escape

EXPOSE 5051/tcp

CMD socat TCP-LISTEN:5051,reuseaddr,fork EXEC:/escape
