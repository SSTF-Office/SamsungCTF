FROM ubuntu
WORKDIR /app
ADD EchoFrag /app/
RUN apt-get update \
    && apt-get -y install --no-install-recommends \
        binutils-aarch64-linux-gnu \
        libc6-arm64-cross \
        qemu-user-static \
        socat \
    && rm -rf /var/lib/apt/lists/* \
    && useradd prob \
    && chown -R prob:prob /app


ENTRYPOINT ["runuser", "prob","-c", "socat TCP-LISTEN:31337,reuseaddr,fork EXEC:\"/usr/bin/qemu-aarch64-static -L /usr/aarch64-linux-gnu/ /app/EchoFrag\""]
# ENTRYPOINT [ "/bin/sh" ]
