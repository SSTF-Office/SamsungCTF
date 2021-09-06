# Installation of required packages

    sudo apt update
    sudo apt install python3-pip
    sudo pip3 install flask
    sudo apt install docker.io
    cd client_sandbox/python_container
    sudo docker build -t python_sandbox_1 .

# Configuration

Blocking access from docker containers to outside addresses (except {IP}:8080):

    sudo iptables -I DOCKER-USER -p tcp ! -d {IP} -j DROP
    sudo iptables -I DOCKER-USER -p tcp ! --destination-port 8080 -j DROP

WARNING: This will block all accesses from Docker containers, so it there are any other Dockers, that require access,
    you need to modify above rules!

Blocking access to environment server ({IP}:8080) except from Docker containers and dashboard server:

    sudo iptables <TBD> DROP
    sudo iptables <TBD> DROP

# Running servers

    cd ctf_worms
    screen -dm -S ctf_dashboard sudo python3 -m server.dashboard_server

    (runs Flask dashboard server on 0.0.0.0:8888)

    screen -dm -S ctf_rest sudo python3 -m server.rest_server

    (runs Flask environment server on 0.0.0.0:8080)

# Instruction for CTF participants

    If you want to participate in autonomous vacuum cleaning challenge, visit our website: http://{IP of server}:8888/

# Credentials for CTF participants

    Credentials for CTF participants and team names must be inserted into server/ctf_users.yaml file

# Contents

Description of folders:
- device - clients simulating IoT device API via access to the Flask server 
- server - Flask server simulating IoT environment 
- updates - tool used to generate IoT update packages (compressed and signed)
- firmwares - example firmware files used for testing
- Writeup.md - spoilers :-)

#Tests

##Unit tests

Running all unit tests:

    python3 -m unittest discover

##Firmware tests

To run multiple firmwares (different one firmware on each device):

    python3 -m tools.run_firmwares {SERVER_IP}:8888

To run one selected firmware on all user accounts:

    python3 -m tools.run_firmwares {SERVER_IP}:8888 name_firmware_from_firmwares_dir

For example:

    python3 -m tools.run_firmwares 127.0.0.1:8888 test_clean.py

To run selected firmware on one account multiple times:

    python3 -m tools.run_firmwares {SERVER_IP}:8888 name_firmware_from_firmwares_dir user_id nr_runs

For example:

    python3 -m tools.run_firmwares 127.0.0.1:8888 test_exception.py 9d0b8da1fb4afa7b98e0642caee45c94e4f6bf61 999999 


#Automaticaly generated files

##Update of website files

API documentation (available at: http://{URL}:8888/api-doc) is automatically generated using pdoc3 from device_api.py.
After changing API in device_api.py update packages using following commands:

    cd device
    pdoc3 --html --force device_api.py
    cd ..


##Update of firmware packages

Updated packages for lightbulb and vacuum cleaner available on the main website are generated based on files in /updates/ folder. 
After changing update packages please update packages using following commands:

    cd updates/lightbulb/
    python3 ../prepare.py
    cd ..
    cd vacuumcleaner/
    python3 ../prepare.py
    cd ../..

##Update of compiled Python API

To provide better obfuscation of API used by participants it is used in Docker environment only in Python bytecode (compiled) form -- as pyc file.
After changing device/device_api_implementation.py update pyc file using following commands:

    cd ctf_worms
    python3.5 -m compileall device/device_api_implementation.py

