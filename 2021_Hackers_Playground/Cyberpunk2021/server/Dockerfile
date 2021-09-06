FROM ubuntu:20.04

# Install xinetd
RUN apt-get update
RUN apt-get install -y xinetd

# Add user
RUN useradd -b /home/samurai samurai
WORKDIR /home/samurai

# Copy stuff
ADD cyberpunk /home/samurai/cyberpunk
ADD cyberpunk.xinetd /etc/xinetd.d/cyberpunk
ADD flag /home/samurai/flag
ADD flag /flag
ADD start.sh /start.sh

# Set permissions
RUN chown root:samurai /flag
RUN chmod 440 /flag

RUN chown -R root:samurai /home/samurai/
RUN chmod 750 /home/samurai
RUN chmod 550 /home/samurai/cyberpunk
RUN chmod 440 /home/samurai/flag
RUN chmod 550 /start.sh

CMD ["/start.sh"]

EXPOSE 1337
