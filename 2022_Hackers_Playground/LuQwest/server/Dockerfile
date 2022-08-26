FROM ubuntu:18.04

# Install xinetd
RUN apt-get update
RUN apt-get install -y xinetd

# Add user
RUN useradd -b /home/player player
WORKDIR /home/player

# Copy stuff
ADD luqwest /home/player/luqwest
ADD script.lua /home/player/script.lua
ADD luqwest.xinetd /etc/xinetd.d/luqwest
ADD flag /home/player/flag
ADD flag /flag
ADD start.sh /start.sh

# Set permissions
RUN chown root:player /flag
RUN chmod 440 /flag

RUN chown -R root:player /home/player/
RUN chmod 750 /home/player
RUN chmod 550 /home/player/luqwest
RUN chmod 440 /home/player/script.lua
RUN chmod 440 /home/player/flag
RUN chmod 550 /start.sh

CMD ["/start.sh"]

EXPOSE 1337
