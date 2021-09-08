# ABC

## Description

There are 3 different vulnerabilities.  
Find all vulnerabilities and patch the binary automatically for 100 tests.

(ip : port)

## Important

Please remove `deploy/tester` when the problem becomes public.

## Challenge setup
```/bin/sh
$ cd deploy/
$ (sudo) ./build_docker.sh
$ (sudo) ./run_docker.sh
$ (sudo) ./attach_docker.sh

[container]$ ./run.sh <port>
[Ctrl + p, Ctrl + q to escape container]
```

## Exploit

### Stage 1
There is format string bug in `view_memo` function.  
Befroe FSB is triggered, there are instructions as below.
```
1: lea     rdi, aS         ; "[%s]\n"
2: mov     rax, [rbp+s2]
3: mov     rax, [rax+10h]
4: mov     rdi, rax        ; format
5: mov     eax, 0
6: call    _printf
```
By patching 3 bytes(2 bytes in line 1, 1 byte in line 4), you can get below
instructions, which does not have format string vulnerability.
```
1: lea     rdi, aS0        ; "%s"
2: mov     rax, [rbp+s2]
3: mov     rax, [rax+10h]
4: mov     rsi, rax        ; format
5: mov     eax, 0
6: call    _printf
```

### Stage 2
There is stack based BOF in `read_int` function.  
Make input size same as buffer size - 1.  
Then the vulerability disappears.

### Stage 3
There is type confusion in `modify_memo` function.  
Due to this vulerability, heap overflow can occur.  
By modifying the conditional jump `jge` around `call realloc` to `jae`, the
vulerability is fixed.

### Changes
Now all 3 stages are integrated.

### Etc
Stage 2 contains the vulnerabilty of Stage 1.  
Stage 3 also contains the vulnerabilities of Stage 1 and Stage 2.
