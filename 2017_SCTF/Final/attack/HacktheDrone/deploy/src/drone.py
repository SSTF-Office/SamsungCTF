#!/usr/bin/python3
import socket
import sys
import random
import struct
import binascii
import threading
import traceback
import logging, coloredlogs
import time

coloredlogs.install(level=logging.INFO)
logger = logging.getLogger("drone")

with open("/home/drone/flag", "r") as f:
    flag = f.read()


def uf32(a): return struct.unpack("<f", a)[0]
def f32(a): return struct.pack("<f", a)
def u32(a): return struct.unpack("<I", a)[0]
def u16(a): return struct.unpack("<H", a)[0]


def sendline(s):
    sys.stdout.buffer.write(binascii.hexlify(s.encode()) + b"\n")
    sys.stdout.flush()


def animate(start, end, coef=1, div=200):
    if start == end:
        return [start]

    start, end = start * coef, end * coef

    duration = int(abs(end - start)*1.0/div)
    if duration < 3:
        duration = 3

    data = []
    for i in range(duration+1):
        t = i/duration*1.0
        val = start + (end - start) * t
        data.append(val*1.0/coef)
    return data


class Map(object):
    def __init__(self, width, height, depth):
        pass


class Protocol(object):
    def __init__(self, headers, protocol):
        self.headers = headers
        self.protocol = protocol
        self.header_size = []
        for field in self.headers:
            self.header_size.append(self.protocol[field])

        logger.debug(self.headers)
        logger.debug(self.protocol)


    def check_length(self, packet):
        if len(packet) < sum(self.header_size):
            self.log_info("[-] Too short packet")
            return False

        if len(packet) > sum(self.protocol.values()):
            self.log_info("[-] Too long packet")
            return False

        offs = self.get_offs('length')
        size = self.get_size('length')
        data = packet[offs:offs + size]

        #logger.debug("idx: %d, offs: %d, %s", idx, offs, str(self.header_size))
        logger.debug(repr(packet))

        logger.debug(size)
        logger.debug(u32(data))


        if size == 4:
            length = u32(data)
        elif size == 2:
            length = u16(data)
        else:
            length = ord(data)

        if length != len(packet):
            self.log_info("[-] Packet length does not match with the header")
            return False
        return True


    def check_crc(self, packet):
        if len(packet) < sum(self.header_size) + self.protocol['crc']:
            self.log_info("[-] No crc in the packet")
            return False

        crc = u32(packet[-4:])
        target_crc = binascii.crc32(packet[:-4]) & 0xffffffff
        if crc != target_crc:
            logger.debug("%x %x", crc, target_crc)
            self.log_info("[-] Wrong crc32")
            return False
        return True


class Drone(Protocol):
    DISARMED = 0
    ARMED = 1
    CALIBRATING = 2
    MODE_LIST = [DISARMED, ARMED, CALIBRATING]
    MODE_STR = ["DISARMED", "ARMED", "CALIBRATING"]

    def __init__(self):
        self.log_info("[+] creating a drone")

        self.protocol = {
            #header
            'length' : 4,
            'uid' : 2,
            'cmd' : 2,

            #body
            'payload' : 8,
            'crc' : 4,
        }

        self.uid = random.randint(0x1000, 0xffff)
        logger.debug("uid: %x", self.uid)

        self.headers = ['length', 'uid', 'cmd']

        super(Drone, self).__init__(self.headers, self.protocol)

        self.actions = {
            0x1212: self.help,
            0x3030: self.print_location,
            0x4040: self.control_rotor,
            0x6666: self.change_altitude,
            0x7878: self.moveto,
            0xfefe: self.change_mode,
        }

        self.rotors = [0x11, 0x12, 0x13, 0x14]
        self.rotor_status = [-1, -1, -1, -1]
        self.status = 0
        self.x = 20
        self.y = 50
        self.altitude = 0
        self.calibrated = [False, False, False, False]
        self.timer = 0
        self.flag_alt = False
        self.flag_move = False
        self.prev = False
        self.crash = False

        self.goal = (round(25, 4), round(16, 4))
        self.map_boundary = ((1, 1), (80, 54))
        self.danger_zone = ((5, 22), (52, 44))
        self.map_size = (80, 54)

        self.map = list('''                                                                                
                                                                                
                                                                                
                                                                                
                                                                                
                                                                                
                                                                                
                                                                                
                                                                                
                                                                                
                                               (52, 44)                         
    *----------------------------------------------*                            
    |  Dangerzone                                  |                            
    |                                     {'{      |                            
    |                                    /   '.    |                            
    |                                  _/     _}   |                            
    |                            __ __}      /     |                            
    |                           (  `        (      |                            
    |                     /'\____\          /      |                            
    |                    :                  |      |                            
    |                    /                  |      |                            
    |                   /                 _/       |                            
    |             __.-='                 /         |                            
    |         .-='                   .-='          |                            
    |     .-='                 _.--='              |                            
    |    (                   .'                    |                            
    |     \=._               }                     |                            
    |         '-._          (                      |                            
    |            (           '=-.                  |                            
    |            /               \                 |                            
    |           (_                \                |                            
    |        _.='\                 \               |                            
    |       ;                       }              |       EAST                 
    *----------------------------------------------*       SEA                  
  (5, 22)    " \_ _ . -      - . -   \                                          
                \                     \            __,=-.                       
              ,=.\                     \          {__.--'                       
 WEST        {_.                        }                      __. Dokdo        
 SEA          :`{    (25, 16)           |                     {__|              
                /       Y Base          /                                       
                }                      .                                        
                \                      |                                        
               _]                     <_.                                       
               ',                       /                                       
               /                       /                                        
               \                _.____/                                         
                }  _____.-.--=\{ /}                                             
                \/" ,_}   \,      `                                             
                                                                                
                                                                                
               __,=-.                                                           
              {__.--'  Jeju                                                     
                                                                                
                                                                                
                                                                                
''')


    def log_info(self, msg):
        sendline(msg)
        logger.debug(msg)


    def help(self, payload):
        self.log_info("[+] this is desecription page")
        self.log_info('''
__________                __           ________                              
\______   \_______  _____/  |_  ____   \______ \_______  ____   ____   ____  
 |     ___/\_  __ \/  _ \   __\/  _ \   |    |  \_  __ \/  _ \ /    \_/ __ \ 
 |    |     |  | \(  <_> )  | (  <_> )  |    `   \  | \(  <_> )   |  \  ___/ 
 |____|     |__|   \____/|__|  \____/  /_______  /__|   \____/|___|  /\___  >
                                               \/                  \/     \/ 

''')
        actions_str = str(list(map(lambda x: x[1].__name__,
                                   sorted(self.actions.items(), key=lambda x:
                                          x[0]))))
        self.log_info("[+] actions: %s" % actions_str)
        self.log_info("[+] this drone is supported by the SuperSafe company")
        self.log_info("[+] to fly, calibration should be preceded")


    def print_location(self, payload):
        self.log_info("[+] current location")
        self.log_info("[+] altitude: %fm" % (self.altitude))
        self.log_info("[+] location (x: %f, y: %f)" % (self.x, self.y))
        self.log_info("[+] You need to move the drone to the base")
        self.log_info("[+] map boundary: %s" %(str(self.map_boundary)))


        x = int(self.x) - 1
        y = int(self.y)

        idx = (55 - y)*81 + x
        if self.prev:
            self.map[self.prev] = self.prev_char
        self.prev_char = self.map[idx]
        self.map[idx] = "X"
        self.prev = idx

        self.log_info("".join(self.map))
        time.sleep(1)


    def check_armed(self):
        if self.status != 1:
            self.log_info("[-] Drone should be armed first")
            return False
        return True


    def control_rotor(self, payload):
        self.log_info("[+] rotor control mode")
        if self.status != Drone.CALIBRATING:
            self.log_info("[-] rotor control can be only allowed in calibrated mode.")
            self.log_info("[-] otherwise, the drone will be crashed if you are not an expert")
            return
        #self.log_info("[+] to calibrate drone, please control the rotor first, minimum value (0) and then, maximum value (ffff)")

        if sum(self.calibrated) == 4:
            self.log_info("[+] all rotors are already calibrated")
            return

        if len(payload) != 3:
            self.log_info("[-] Invalid payload size")
            return

        rotor_id = payload[0]
        if not rotor_id in self.rotors:
            self.log_info("[-] Unknown rotor id: %d" % rotor_id)
            return

        rotor_no = self.rotors.index(rotor_id)
        self.log_info("[+] controlling rotor[%d] %d" % (rotor_no + 1, rotor_id))

        speed = u16(payload[1:])
        self.log_info("[+] the speed of rotor %d is being set to %x" % (rotor_id, speed))
        time.sleep(1)

        if self.rotor_status[rotor_no] == 0 and speed == 0xffff:
            self.calibrated[rotor_no] = True
            self.log_info("[+] rotor %d's calibration is done." % (rotor_id))

        if sum(self.calibrated) == 4:
            self.log_info("[+] calibration of all rotors is done. now you can fly the drone")

        self.rotor_status[rotor_no] = speed


    def check_move(self):
        if self.flag_alt or self.flag_move:
            self.log_info("[-] drone is already moving...")
            self.print_location("")
            return False

        return True


    def change_altitude(self, payload):
        self.log_info("[+] change altitude of drone")
        if not self.check_armed():
            return

        if not self.check_move():
            return

        if len(payload) != 4:
            self.log_info("[-] Invalid payload size")
            return

        altitude = uf32(payload)
        if not 0 <= altitude:
            self.log_info("[-] Invalid altitude: %f" % altitude)
            return

        self.log_info("[+] altitude is moving to: %f m" % altitude)
        self.flag_alt = True
        threading.Thread(target=self.change_altitude_helper, args=(altitude,)).start()


    def change_altitude_helper(self, altitude):
        l = animate(self.altitude, altitude)
        self.log_info("[+] it may take %ds, use 'print_location' to check current altitude" % len(l))
        for x in l:
            time.sleep(1)
            self.altitude = x

            logger.debug("[+] drone is now %f m high in the sky" % (self.altitude))
            if self.altitude > 300 and sum(self.calibrated) != 4:
                self.log_info("[-] drone suddenly crashed due to the lack of calibration")
                self.crash = True
                return

            if self.altitude > 1000:
                self.log_info("[-] wind blew and drone became out of control. maybe its too high")
                self.crash = True
                return

            if (round(self.x, 4), round(self.y, 4)) == self.goal and round(self.altitude) == 0:
                self.log_info("[+] Congratulations! the flag is: %s" % flag)
                #logger.info("%s got the flag" % self.addr)
                self.crash = True

        self.flag_alt = False


    def moveto(self, payload):
        self.log_info("[+] set the waypoint of drone")
        if not self.check_armed():
            return

        if not self.check_move():
            return

        if self.altitude < 800:
            self.log_info("[-] Drone should be up high enough (>= 800 m) before moving to the waypoint")
            return

        if len(payload) != 8:
            self.log_info("[-] Invalid payload size")
            return

        x = uf32(payload[:4])
        y = uf32(payload[4:])

        self.log_info("[+] moving drone to (x: %f, y: %f)" % (x, y))

        self.flag_move = True
        threading.Thread(target=self.moveto_helper, args=(x, y)).start()


    def moveto_helper(self, x, y):
        x_list = animate(self.x, x, coef=100, div=400)
        y_list = animate(self.y, y, coef=100, div=300)

        x_len = len(x_list)
        y_len = len(y_list)

        if x_len < y_len:
            x_list.extend([x_list[-1]] * (y_len - x_len))
        else:
            y_list.extend([y_list[-1]] * (x_len - y_len))

        self.log_info("[+] it may take %ds, use 'print_location' to check current location" % len(x_list))
        for x, y in zip(x_list, y_list):
            time.sleep(1)
            self.x = x
            self.y = y
            logger.debug("[+] drone is now at (x: %f, y: %f)" % (self.x, self.y))
            if self.altitude > 300 and sum(self.calibrated) != 4:
                self.log_info("[-] drone suddenly crashed due to the lack of calibration")
                self.crash = True
                return

            if self.altitude > 1000:
                self.log_info("[-] wind blew and drone became out of control. maybe its too high")
                self.crash = True
                return

            if not (self.map_boundary[0][0] < self.x < self.map_boundary[1][0] and \
                    self.map_boundary[0][1] < self.y < self.map_boundary[1][1]):
                self.log_info("[-] drone got out of control, outside of the map: %f, %f" % (self.x, self.y))
                self.crash = True
                return

            if self.danger_zone[0][0] < self.x < self.danger_zone[1][0] and \
                    self.danger_zone[0][1] < self.y < self.danger_zone[1][1]:
                self.log_info("[-] drone was exploded by NK. You need to take a detour")
                self.crash = True
                return

            if (round(self.x, 4), round(self.y, 4)) == self.goal:
                self.log_info("[+] Congratulations! you just moved to the right place!!")
                self.log_info("[+] land the drone for the flag!")
        self.flag_move = False


    def change_mode(self, payload):
        self.log_info("[+] change mode: DISARMED (0) / ARMED (1) / CALIBRATING (2)")
        self.log_info("[+] current mode: %s" % Drone.MODE_STR[self.status])

        if len(payload) !=  2 :
            self.log_info("[-] Invalid payload size")
            return

        mode = u16(payload)
        if not mode in Drone.MODE_LIST:
            self.log_info("[-] Unknown mode: %d" % mode)
            return

        time.sleep(1)

        if mode == self.status:
            self.log_info("[+] already in mode: %s" % Drone.MODE_STR[self.status])
        else:
            self.status = mode
            self.log_info("[+] drone is now in %s mode" % Drone.MODE_STR[self.status])

        if self.status == Drone.CALIBRATING:
            self.log_info("[+] to calibrate drone, please control the target rotor first, minimum value (0) and then, maximum value (0xffff)")


    def get_offs(self, field):
        idx = self.headers.index(field)
        offs = sum(self.header_size[:idx])
        return offs


    def get_size(self, field):
        return self.protocol[field]


    def check_uid(self, packet):
        offs = self.get_offs('uid')
        size = self.get_size('uid')
        uid = u16(packet[offs:offs + size])
        logger.debug("%x %x", uid, self.uid)

        if uid != self.uid:
            self.log_info("[-] That is not my UID, my uid is %d" % self.uid)
            return False
        return True


    def check_time(self):
        t0 = time.time()
        if (t0 - self.timer) < 10:
            self.log_info("[-] You're sending commands too fast ... (10 seconds)")
            return False
        self.timer = t0
        return True


    def parse(self, packet):
        if self.crash:
            return True
        if not self.check_length(packet):
            return
        if not self.check_crc(packet):
            return
        if not self.check_uid(packet):
            return

        offs = self.get_offs('cmd')
        size = self.get_size('cmd')
        cmd = packet[offs:offs+size]
        cmd = u16(cmd)

        logger.debug(repr(packet))
        logger.debug(repr(cmd))
        logger.debug("cmd: %x", cmd)

        payload = packet[offs+size:-4]
        logger.debug("payload: %s", repr(payload))

        # first command
        if not cmd in self.actions:
            self.log_info("[-] Unknown command: %d" % cmd)
            return

        return self.actions[cmd](payload)


try:
    d = Drone()
    t0 = time.time()
    while True:
        data = sys.stdin.buffer.readline().rstrip()
        if not data:
            break

        data = binascii.unhexlify(data)

        logger.debug(repr(data))
        if d.parse(data):
            break

except:
    #traceback.print_exc()
    pass

