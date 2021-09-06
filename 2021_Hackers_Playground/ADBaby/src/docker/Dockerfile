FROM us-docker.pkg.dev/android-emulator-268719/images/30-google-x64:30.1.2

RUN apt update &&  apt install -y procps net-tools screen vim
RUN useradd -ms /bin/bash sctf
 
COPY scripts /home/sctf/scripts
COPY android /home/sctf/.android
COPY adbd_sctf /home/sctf/adbd_sctf
COPY flag /home/sctf/flag

RUN chmod 555 /home/sctf/adbd_sctf
RUN chmod 444 /home/sctf/flag
RUN chown -R sctf /home/sctf/.android

CMD ["/home/sctf/scripts/start.sh"]
