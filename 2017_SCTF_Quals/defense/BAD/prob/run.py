import sys,random,subprocess
import tester
import socket,threading,telnetlib
import binascii,base64,os
BIN_DIR = "./bin/"
USER="bad"
LIMIT = {1:2,2:4,3:6}
MAX_LISTEN = 1024
FLAG="FLAG{THIS IS FLAG}"

class Server:
  def __init__(self,host,port):
    sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    sock.bind((host,port))
    self.sock = sock
    self.clients = {}

  def listen(self):
    self.sock.listen(MAX_LISTEN)
    while True:
      client,addr = self.sock.accept()
      t = telnetlib.Telnet()
      t.sock = client
      print addr
      threading.Thread(target=self.prob , args=(t,addr)).start()

  def prob(self,client,addr):
    ip = addr[0]
    try:
      if self.allow(addr):
        for stage in range(1,4):
          client.write("STAGE : %d\n"%stage)
          for i in range(30):
            p = Problem(stage,ip)
            if not p.go(client):
              client.write("Fail!\n")
              client.close()
              del self.clients[ip] 
              return False
            else:
              client.write("Success!\n")
        client.write(FLAG+"\n")
        client.close()
        del self.clients[ip] 
      else:
        client.write("You are already connected\n")
      client.close()
    except IOError:
      if ip in self.clients:
        del self.clients[ip]

  def allow(self,addr):
    return True

class Problem:
  """
  STAGE 1 : BOF[stack] at 'get_int'
    - 0x10 < GET_INT_BUF < GET_INT_BUF_OVER-0x20 < 0x1000
  STAGE 2 : BOF[stack] at 'get_file'
    - 0x40 < VNAME_SIZE < GET_FILE_BUF_OVER-0x20 < 0x1000
  STAGE 3 : BOF[heap]  at 'modify_file'
    - 0x40 < VNAME_SIZE < VNAME_SIZE_OVER-0x12 <0x1000 , 4byte align
  """
  def __init__(self,stage,ip):
    self.__stage = stage
    self.__ip = ip
    self.__opt = self.__setting(stage)
    self.__bin = self.__compile(stage)
  def __setting(self,stage):
    ret = {}
    ret["GET_INT_BUF"] = (random.randint(0x10,0x100) >>2) <<2
    #ret["GET_INT_BUF"] = 0x20
    ret["GET_INT_BUF_OVER"] = ret["GET_INT_BUF"]
    ret["VNAME_SIZE"] = (random.randint(0x40,0x200) >>2) <<2
    #ret["VNAME_SIZE"] = 0x120
    ret["VNAME_SIZE_OVER"] = ret["VNAME_SIZE"]
    ret["GET_FILE_BUF_OVER"] = ret["VNAME_SIZE"]

    if stage >= 1:
      #ret["GET_INT_BUF_OVER"] += 0x20
      ret["GET_INT_BUF_OVER"] += (random.randint(0x20,0x100) >>2) <<2
    if stage >= 2 :
      #ret["GET_FILE_BUF_OVER"] += 0x20
      ret["GET_FILE_BUF_OVER"] += (random.randint(0x20,0x100) >>2) <<2
    if stage >= 3 :
      #ret["VNAME_SIZE_OVER"] += 0x20
      ret["VNAME_SIZE_OVER"] += (random.randint(0x20,0x100)>>2)<<2
    return ret

  def __compile(self,stage):
    name = BIN_DIR+self.__ip
    param = ["/usr/bin/gcc","./template.c", "-o", name,"-fno-stack-protector","-m32"]
    for key in self.__opt:
      param.append("-D%s=%d"%(key,self.__opt[key]))
    p = subprocess.Popen(param)
    p.wait()
    with open(name,"rb") as f:
      ret  = f.read()
    return ret

  def check(self,ans):
    if self.__check_brief(ans):
      patch = BIN_DIR+self.__ip +".ans"
      with open(patch,"wb") as f:
        f.write(ans)
      os.chmod(patch,750)
      return tester.Test(gen_SLA_test(self.__opt)+gen_PATCH_test(self.__opt)[:self.__stage],USER,patch)
    return False

  def __check_brief(self,ans):
    if len(self.__bin) != len(ans):
      print "NOT EQUAL %d, %d"%(len(self.__bin),len(ans))
      return False
    idx, diff = 0,0
    while idx<len(self.__bin):
      if self.__bin[idx] != ans[idx]:
        diff +=1
        if diff > LIMIT[self.__stage]:
          return False
      idx+=1
    return True

  def go(self,client):
    client.write(base64.b64encode(self.__bin))
    client.write("Send your file encode as base64.(Allow only %d byte to change) \n"%LIMIT[self.__stage])
    try:
      ans = base64.b64decode(readline(client))
    except binascii.Error:
      client.write("Wrong Input\n")
      return False
    return self.check(ans)
def check(p,data):
  ret = p.recvuntil(data)
  return ret == data

def create_file(p,name,size,content):
  ret = []
  p.recvuntil("6. Exit\n")
  for send,recv in [ ("1","File name :\n"),
                     (name,"File size :\n"),
                     ("%d"%size,"File Contents :\n"),
                     (content,"DONE\n") ]:
    p.sendline(send)
    ret.append(check(p,recv))
  return all(ret)

def modify_file(p,target,name,size,content,v=False):
  ret = []
  p.recvuntil("6. Exit\n")
  for send,recv in [ ("2","Name to modify\n"),
                     (target,"New name :\n"),
                     (name,"New file size :\n"),
                     ("%d"%size,"New file contents :\n"),
                     (content,"DONE\n") ]:
    if v and send =="%d"%size:
      break
    p.sendline(send)
    ret.append(check(p,recv))
  return all(ret)

def read_file(p,target,content):
  ret = []
  p.recvuntil("6. Exit\n")
  p.sendline("3")
  ret.append(check(p,"Name to read\n"))
  p.sendline(target)
  ret.append(check(p,content+"DONE\n"))
  return all(ret)

def list_file(p,name_list):
  ret = []
  p.recvuntil("6. Exit\n")
  p.sendline("4")
  for name in name_list:
    ret.append(check(p,name+"\n"))
  return all(ret)

def delete_file(p,name):
  ret = []
  p.recvuntil("6. Exit\n")
  p.sendline("5")
  ret.append(check(p,"Name to delete\n"))
  p.sendline(name)
  ret.append(check(p,"DONE\n"))
  return all(ret)

def gen_SLA_test(opt):
  vname_size = opt["VNAME_SIZE"]
  def SLA_test(p):
    ret = []
    name_list= ["A"*(vname_size-1),"B"*(vname_size-1),"C"*(vname_size-1)]
    for name in name_list:
      ret.append(create_file(p,name,vname_size,name))
      ret.append(read_file(p,name,name+"\n"))
    ret.append(list_file(p,name_list))
    new_list = []
    for name in name_list:
      new_name = "M"+name[:-1]
      new_list.append(new_name)
      ret.append(modify_file(p,name,new_name,0x10,"D"*0xf))
      ret.append(read_file(p,new_name,"D"*0xf+"\n"))
    ret.append(list_file(p,new_list))

    ret.append(delete_file(p,new_list[1]))
    ret.append(delete_file(p,new_list[2]))
    ret.append(delete_file(p,new_list[0]))
    
    p.recvuntil("6. Exit\n")
    p.sendline("6\n")
    return all(ret)

  return [SLA_test]

def gen_PATCH_test(opt):
  vname_size = opt["VNAME_SIZE"]
  vname_size_over = opt["VNAME_SIZE_OVER"]
  get_file_buf_over = opt["GET_FILE_BUF_OVER"]
  get_int_buf = opt["GET_INT_BUF"]
  get_int_buf_over = opt["GET_INT_BUF_OVER"]
  def stage1(p):
    p.recvuntil("6. Exit\n")
    p.sendline("A"*get_int_buf_over*10)
    return True
  def stage2(p):
    ret = []
    ret.append(create_file(p,"A",0x10,"B"*0xf))
    p.recvuntil("6. Exit\n")
    p.sendline("3")
    ret = check(p,"Name to read\n")
    p.sendline("A\x00"+"A"*(get_file_buf_over-2)*10)
    return ret

  def stage3(p):
    ret = []
    ret.append(create_file(p,"A"*(vname_size-1),0x10,"B"*0xf))
    ret.append(modify_file(p,"A"*(vname_size-1),"C"*(vname_size*10),6,"C"*10,True))
    return all(ret)
  
  return [stage1,stage2,stage3]

def readline(client):
  try:
    ret = client.read_until("\n")
  except EOFError:
    return ""
  return ret
if __name__ == "__main__":
  if len(sys.argv) !=3:
    print "usage: python run.py <FLAG> <PORT>"
    sys.exit(0)
  FLAG = sys.argv[1]
  s = Server("",int(sys.argv[2]))
  s.listen()

