# Hack the Drone

## Description
Hack the Drone!

Enemy's drone has been spotted. This drone contains secret information, called
'flag'. However, there is nothing known except for the drone's address. You need
to figure out the protocol yourself.

Hurry up! You need to hack the drone and steal it to the base.

nc [Server IP] [port Number]


## Comment
Only IP/port will be given.

Players should figure out the protocol specification with the response debugging
messages.

1) Get the packet length

2) Length field in the header

3) crc32 at last 4 bytes

4) uid information (will be given, random 2 bytes)

5) cmd (2 bytes, need to bruteforce)

6) payload for each cmd (need to figure out by hand, may bruteforce 1 byte)

7) calibrate the drone, change altitude, move the drone to the base (need to
bypass the 'danger zone', land the drone, and get the flag.


## How to Run
```/bin/sh
# Install docker..
sudo pip install docker-compose
sudo docker-compose build
sudo docker-compose up -d
# Wait for about 10 seconds..
```


## Requirements
python3

