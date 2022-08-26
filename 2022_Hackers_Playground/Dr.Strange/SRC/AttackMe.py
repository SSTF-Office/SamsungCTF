#!/usr/bin/python3

def readKey():
    global KEY
    f = open("./flag","r")
    KEY = f.read(32)
    f.close()

def read_stdin(): return input("input>")

def do_crypt(plain_text):
    out = [] 
    i = 0
    for p in plain_text:
        p = ord(p)
        d = (ord(KEY[ i % len(KEY) ]) ^ p) * ord(KEY[ (i+1) % len(KEY) ])
        e = (d << p) % 500009
        for pad in range (0, 6-len(str(e))): e*=10
        o = pow(p, e)
        out.append(o)
        i+=1;
    return out

def func1(cipher_text):
    ret_value=0
    for value in cipher_text:
        ret_value += int(value) % 100003
    if ret_value % 2:
        return 1
    else:
        return 0;

def main():
    print("The flag consists of lowercase letters. ex)sctf{oooo_ooooo_oooo_ooo}");

    readKey()
    plain_text = read_stdin()
    print(plain_text)
    cipher_text = do_crypt(plain_text)
    print("value: {}".format(func1(cipher_text))) 

if __name__ =="__main__":
    main()
