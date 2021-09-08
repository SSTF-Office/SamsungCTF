#!/usr/bin/env python

import sys, random, subprocess
import tester
import socket, multiprocessing, telnetlib
import base64, os
import traceback

BIN_DIR = "./bin/"
SRC_DIR = "./prob/src/"
SRC_NAMES = ["main.c", "init.c", "menu.c", "utils.c"]
INC_DIR = "./prob/inc/"
USER = "abc"
MODIFY_LIMIT = {1:3, 2:4, 3:5}
TEST_COUNT = 100
MAX_LISTEN = 128

FLAG = open("flag").read().rstrip()

class Server:
    def __init__(self, host, port):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind((host, port))
        self.sock = sock
        self.clients = set()

    def listen(self):
        self.sock.listen(MAX_LISTEN)
        print "[ LOG ] Listen start"
        while True:
            client, addr = self.sock.accept()
            t = telnetlib.Telnet()
            t.sock = client
            p = multiprocessing.Process(target=self.prob, args=(t, addr)).start()
            p.daemon = True
            p.start()

    def prob(self, client, addr):
        ip = addr[0]
        try:
            if self.allow(addr):
                print "[ LOG ] Connected: {}".format(addr)
                self.clients.add(ip)
                dir_name = os.path.join(BIN_DIR, ip)
                try:
                    os.makedirs(dir_name)
                except:
                    pass
                for i in range(TEST_COUNT):
                    client.write("[*] TRY {}/{}\n".format(i + 1, TEST_COUNT))
                    p = Problem(3, ip, dir_name, i == 0)
                    if not p.go(client):
                        client.write("[!] Fail!\n")
                        client.close()
                        self.clients.discard(ip)
                        print "[ LOG ] Closed: {}".format(addr)
                        return False
                    else:
                        client.write("[*] Success!\n")
                client.write("{}\n".format(FLAG))
                client.close()
                self.clients.discard(ip)
                print "[ LOG ] Closed: {}".format(addr)
            else:
                client.write("[!] You are already connected\n")
                client.close()
        except IOError:
            self.clients.discard(ip)
            print "[ LOG ] Closed: {}".format(addr)

    def allow(self, addr):
        #return addr[0] not in self.clients
        return True


class Problem:
    def __init__(self, stage, ip, dir_name, first=False):
        self.__stage = stage
        self.__ip = ip
        self.__dir = dir_name
        self.__opt = self.__setting(stage)
        if first:
            with open("./abc_default", "rb") as f:
                self.__bin = f.read()
        else:
            self.__bin = self.__compile()

    def __setting(self, stage):
        ret = {}
        ret["READ_INT_BUFFER_SIZE"] = (random.randint(0x10, 0xff) >> 2) << 2
        ret["READ_INT_REAL_SIZE"] = ret["READ_INT_BUFFER_SIZE"] - 1

        if stage >= 2:
            ret["READ_INT_REAL_SIZE"] |= random.randint(0x1, 0x40) << 8
        if stage >= 3 :
            ret["STAGE3"] = 1
        return ret

    def __generate_dummy(self):
        src_files = list(map(lambda x: os.path.join(SRC_DIR, x), SRC_NAMES))
        dst_files = list(map(lambda x: os.path.join(self.__dir, x), SRC_NAMES))
        for src, dst in zip(src_files, dst_files):
            res = []
            with open(src) as s:
                data = s.readlines()
                for l in data:
                    if "void dummy" in l:
                        nops = "nop;" * random.randint(0x10, 0x200)
                        res.append(l.replace("{}", "{{__asm__ __volatile__ (\"{}\");}}".format(nops)))
                    else:
                        res.append(l)
            with open(dst, "w") as d:
                d.write("".join(res))
        return dst_files

    def __compile(self):
        while True:
            bin_name = os.path.join(self.__dir, "abc")
            src_files = self.__generate_dummy()
            options = ["-s", "-fPIE", "-pie", "-z", "relro", "-z", "now", "-fno-stack-protector", "-I" + INC_DIR]
            param = ["/usr/bin/gcc", "-o", bin_name] + options + src_files
            for key in self.__opt:
                param.append("-D{}={}".format(key, self.__opt[key]))
            p = subprocess.Popen(param)
            p.wait()
            with open(bin_name, "rb") as f:
                ret = f.read()
            if ord(ret[ret.find("4889c6bf00000000".decode("hex")) - 4]) < (0x100 - 6):
                break
        return ret

    def test(self, ans, client):
        patch = os.path.join(self.__dir, "abc.ans")
        with open(patch, "wb") as f:
            f.write(ans)
        os.chmod(patch, 0o755)
        client.write("[*] SLA Test Start...")
        if not tester.Tester(patch, USER, alarm=7).run_tests(gen_SLA_test(self.__opt)):
            client.write("[!] SLA Test Failed!")
            return False
        client.write("[*] SLA Test Done")
        client.write("[*] Vulnerability Test Start...")
        if not tester.Tester(patch, USER, alarm=7).run_tests(gen_PATCH_test(self.__opt)[:self.__stage]):
            client.write("[!] Vulnerability Test Failed!")
            return False
        client.write("[*] Vulnerability Test Done")
        return True

    def go(self, client):
        maxoff = len(self.__bin) - 1
        ans = list(self.__bin)
        client.write(base64.b64encode(self.__bin) + "\n")
        try:
            client.write("[*] You can only modify the binary with {} byte(s) or less\n".format(MODIFY_LIMIT[self.__stage]))
            client.write("[*] Input your modification as \"off, byte\"\n")
            client.write("[*] e.g. Change a byte at offset 0x10 to 'A' -> 0x10, 0x41\n\n")
            client.write("[*] If you finished to modify the binary, then input only newline\n")
            for i in range(MODIFY_LIMIT[self.__stage]):
                client.write("[?] INPUT {}: ".format(i + 1))
                line = readline(client)
                if line == "\n":
                    break
                off, byte = line.split(",")
                off = int(off, 16)
                byte = chr(int(byte, 16))
                if off < 0 or off > maxoff:
                    raise ValueError()
                ans[off] = byte
        except ValueError:
            client.write("[!] Wrong Input\n")
            return False
        return self.test("".join(ans), client)


def check(p, data):
    ret = p.recvuntil(data)
    return ret == data

def new_memo(p, name, length, contents):
    p.recvuntil(">> ")
    for send, recv in [("1", "New memo name: "),
                       (name, "New memo length: "),
                       (str(length), "New memo contents: "),
                       (contents, "1. New memo")]:
        p.sendline(send)
        if not check(p, recv):
            return False
    return True

def view_memo(p, name, contents, no=False):
    p.recvuntil(">> ")
    p.sendline("2")
    if not check(p, "View memo name: "):
        return False
    p.sendline(name)
    if no:
        if not check(p, "1. New memo"):
            return False
    else:
        if not check(p, "[{}]\n{}1. New memo".format(name, contents)):
            return False
    return True

def modify_memo(p, name, new_length, new_contents):
    p.recvuntil(">> ")
    for send,recv in [("3", "Modify memo name: "),
                      (name, "New memo length: "),
                      (str(new_length), "New memo contents: "),
                      (new_contents, "1. New memo")]:
        p.sendline(send)
        if not check(p,recv):
            return False
    return True

def delete_memo(p, name):
    p.recvuntil(">> ")
    p.sendline("4")
    if not check(p, "Delete memo name: "):
        return False
    p.sendline(name)
    if not check(p, "1. New memo"):
        return False
    return True

def exit_(p):
    p.recvuntil(">> ")
    p.sendline("5")
    return True

def gen_SLA_test(opt):
    def SLA_test(p):
        ret = []
        name_list = ["TEST1", "TEST2", "TEST3"]
        contents_list = [random.randint(0x100000000, 0x200000000) for _ in range(3)]
        for name, contents in zip(name_list, contents_list):
            ret.append(new_memo(p, name, len(str(contents)) + 1, str(contents)))
        for name, contents in zip(name_list, contents_list):
            ret.append(view_memo(p, name, str(contents)))

        length_list = []
        for name, contents in zip(name_list, contents_list):
            length = len(str(contents)) + 0x20
            length_list.append(length)
            ret.append(modify_memo(p, name, length, "\xcc" * (length - 1)))
        for name, contents, length in zip(name_list, contents_list, length_list):
            ret.append(view_memo(p, name, "\xcc" * (length - 1)))

        ret.append(delete_memo(p, name_list[1]))
        ret.append(delete_memo(p, name_list[2]))
        ret.append(delete_memo(p, name_list[0]))

        for name, contents in zip(name_list, contents_list):
            ret.append(view_memo(p, name, "", True))

        return exit_(p)

    return [SLA_test]

def gen_PATCH_test(opt):
    read_int_buffer_size = opt["READ_INT_BUFFER_SIZE"]
    def stage1(p):
        memo_name = "TEST"
        memo_length = 0x20
        fsb_test = "%p%c%d%l%u%x%lu%llu%s%n"
        return new_memo(p, memo_name, memo_length, fsb_test) \
               and view_memo(p, memo_name, fsb_test) \
               and exit_(p)

    def stage2(p):
        bof_test = "A" * (read_int_buffer_size + 0x30)
        p.recvuntil(">> ")
        p.sendline(bof_test)
        return exit_(p)

    def stage3(p):
        memo_name = "TEST"
        memo_length = 0x20
        memo_contents = "A" * 0x10
        victim_name = "VICTIM"
        victim_length = 0x40
        victim_contents = "THIS_IS_THE_CONTENT_WHICH_MUST_NOT_BE_TAINTED."
        new_memo_length = 0x8000
        new_memo_contents = "THIS_IS_THE_CONTENT_WHICH_MUST_NOT_BE_TAINTED.".ljust(0x300, "A")
        return new_memo(p, memo_name, memo_length, memo_contents) \
               and new_memo(p, victim_name, victim_length, victim_contents) \
               and modify_memo(p, memo_name, new_memo_length, new_memo_contents) \
               and view_memo(p, victim_name, victim_contents) \
               and view_memo(p, memo_name, new_memo_contents) \
               and exit_(p)

    return [stage1, stage2, stage3]

def readline(client):
    try:
        ret = client.read_until("\n")
    except EOFError:
        return ""
    return ret

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print "Usage: python run.py <PORT>"
        sys.exit(0)
    print "[ LOG ] Starting server"
    try:
        os.makedirs(BIN_DIR)
    except:
        pass

    try:
        s = Server("", int(sys.argv[1]))
        s.listen()
    except socket.error as e:
        print traceback.format_exc()
        exit(123)
    except KeyboardInterrupt:
        pass
