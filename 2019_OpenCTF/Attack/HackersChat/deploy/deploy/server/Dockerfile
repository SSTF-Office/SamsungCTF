FROM debian:stable

RUN useradd -ms /bin/bash ctf
RUN mkdir /hacker_chat/
RUN chmod 755 /hacker_chat

RUN  apt-get -y update &&\
     apt-get -y install socat &&\
     apt-get -y install file &&\
     rm -rf /var/lib/apt/lists/*

EXPOSE 1337


COPY ./hackerChat /hacker_chat/hackerChat
COPY ./libssl.so.1.1 /hacker_chat/libssl.so.1.1
COPY ./libcrypto.so.1.1 /hacker_chat/libcrypto.so.1.1
COPY ./libc.so.6 /hacker_chat/libc.so.6
COPY ./ld-linux-x86-64.so.2 /hacker_chat/ld-linux-x86-64.so.2
COPY ./libpthread.so.0 /hacker_chat/libpthread.so.0
COPY ./libpthread.so /hacker_chat/libpthread.so
COPY ./libdl.so.2 /hacker_chat/libdl.so.2

COPY ./k@q.k /hacker_chat/k@q.k
COPY ./cred_file.psw /hacker_chat/cred_file.psw
COPY ./message.s /hacker_chat/message.s

RUN chmod 755 /hacker_chat/*

USER ctf

CMD cd hacker_chat; pwd; LD_LIBRARY_PATH=. socat tcp-l:1337,reuseaddr,fork exec:"./hackerChat",pty,setsid,setpgid,stderr,ctty,raw,echo=0
