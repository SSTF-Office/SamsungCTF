FROM ubuntu:20.04

RUN apt update
RUN apt install -y openssh-server
RUN apt install -y policykit-1
RUN mkdir /var/run/sshd

RUN useradd -M -s /bin/bash pwnkit
RUN mkdir -m 0755 /home/pwnkit

RUN useradd -M -s /bin/bash -d /home/pwnkit sstf
RUN echo "sstf:sstf" | chpasswd

COPY src/libglib-2.0.so.0.7200.0 /usr/lib/x86_64-linux-gnu/libglib-2.0.so.0
COPY src/pkexec /home/pwnkit/pkexec
COPY deploy/flag /home/pwnkit/flag
COPY README /home/pwnkit/README

RUN chown pwnkit:pwnkit /home/pwnkit/flag
RUN chown pwnkit:pwnkit /home/pwnkit/pkexec

RUN chmod 0400 /home/pwnkit/flag
RUN chmod +s /home/pwnkit/pkexec
RUN chmod 0703 /tmp

RUN chattr +i /home/pwnkit/flag
RUN chattr +i /home/pwnkit/pkexec

# run sshd
EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]