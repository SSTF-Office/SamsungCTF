from enum import Enum
import datetime
import base64
import socket
import subprocess
import binascii
import sys

ContentsLength = 511

if len(sys.argv) == 2:
    debug = True
else:
    debug = False


class Rule(Enum):
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
            
            self.content = base64.b64decode(self.base_content).decode() # decoded content 
            self.crcTarget = raw[0:15+self.content_length] # crc target 
            self.crc = int(raw[15+self.content_length:].decode("utf-8")) # crc 

            now = datetime.datetime.now()
            server_date = int(datetime.datetime.timestamp(now))

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

    def __init__(self):
        self.dbname=""
        self.DB=dict()
        self.selected = 0
        self.p = Packet()

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

    def crc_check(self,d):
        if debug:
            self.dprint("---------- CRC check ----------")
            self.dprint(f'raw data : {d}')
            self.dprint(f'data : {self.p.base_content}')
            self.dprint(f'crc : {self.p.crc}')
        if len(d) <= 4:
            return False
        gc = binascii.crc32(self.p.crcTarget)

        if gc == self.p.crc:
            return True
        else :
            return False

    def run_server(self):
        self.dprint("run_server")
        self.p.reset()
        self.dbname = ""
        try:
            while True:
                data = sys.stdin.buffer.readline()
                data = data[:-1]
            
                if len(data) < (15+4):
                    self.just_send(0,"[Exception] Header Error")
                    exit()

                result = self.p.parse(data)

                ## crc check ##################################################
                if self.crc_check(data) == False:
                    self.just_send(0,"crc32_check_failed")
                    exit()
                
                result = self.processing()
        except Exception as e:
            #self.dprint(e)
            self.just_send(0,f'error with {e}')
            exit()



    def processing(self):
        result=""
        cmd = self.p.cmd

        if cmd == Cmd.Admin.value:
            if self.p.content == "turnon!":
                self.just_send(1,"[Success]")
            else:
                self.just_send(0,"[Fail]")
            return
        


        ## Select 
        elif cmd == Cmd.Select.value:
            self.dprint(f'[Select] : {self.p.content}')
            if self.p.content in self.DB:
                self.dbname = self.p.content
                self.just_send(1,f"Selected {self.dbname}")
            else:
                self.just_send(0,f"no {self.p.content} category ")
            return


        ## Create
        elif cmd == Cmd.Create.value:
            self.dprint(f'[Select] : {self.p.content}')
            if self.p.content in self.DB:
                self.just_send(0,"Duplicated name")
            else:
                self.DB[self.p.content]=list()
                self.dbname = self.p.content
                self.just_send(1,f"Created and selected {self.dbname}")
            return 

        ## Delete 
        elif cmd == Cmd.Delete.value:
            if self.dbname == "" or self.dbname not in self.DB:
                self.just_send(0,"none category")
                return 
            
            self.dprint(f'[Delete] : {self.p.content}')
            try:
                index = int(self.p.content)
            except Exception as e:
             #   self.dprint(e)
                self.just_send(0,f"can't convert {self.p.content} to integer")
                return

            if len(self.DB[self.dbname]) <= index or int(self.p.content) < 0:
                self.just_send(0,"Fail - out of bound over")
                return 
            else :
                content = self.DB[self.dbname][index]
                self.DB[self.dbname].remove(self.DB[self.dbname][index])
                self.just_send(1,f'Success -  delete {index}:{content}')
                return 


        ## hidden 
        elif cmd == Cmd.Hidden.value:
            self.dprint(self.p.admin)
            if self.p.admin == 1:
                 self.just_send(1,f'[Contents]\n{self.hidden()}')
            else :
                 self.just_send(0,"Denied")


        ## Get info
        elif cmd == Cmd.Get.value:
            keys = ""
            result=""
            if self.p.content == "Category":
                keys = self.DB.keys()
            elif self.p.content == "Contents":
                if self.dbname == "" or self.dbname not in self.DB:
                    self.just_send(0,"select the category")
                    return 
                keys = self.DB[self.dbname]
            elif self.p.content == "Selected":
                self.just_send(1,f'{self.dbname}')
                return
            else:
                self.just_send(0,"select Category or Contents")
                return 

            for key in keys:
                result = result + key + "\n"

            self.dprint(f'send data : {result}')
            if result == "":
                self.just_send(0,"Empty")
                return "Fail - Empty"
            else:
                self.just_send(1,result)
            return


        ## write 
        elif cmd == Cmd.Write.value:
            if self.dbname == "":
                self.just_send(0,"select a category")
            else:
                self.DB[self.dbname].append(f'[{datetime.datetime.fromtimestamp(self.p.date)}]:{self.p.content}')
                self.just_send(1,"Success")
            return


        ## show 
        elif cmd == Cmd.Show.value:
            if self.dbname == "" or self.dbname not in self.DB:
                self.just_send(0,"Select a Category")
            else:
                result = ""
                for item in self.DB[self.dbname]:
                    result = result + item + "\n"
                if result == "":
                    self.just_send(0,f"No Data in {self.dbname}")
                else:
                    self.just_send(1,result)
            return

        else:
            return


    def hidden(self):
        result = subprocess.run(self.p.content.split(" ")
                , stdout=subprocess.PIPE, text=True)
        return result.stdout[0:ContentsLength-len("[Contents]\n")]


if __name__ == "__main__":
    server = Protocol()
    server.run_server()
