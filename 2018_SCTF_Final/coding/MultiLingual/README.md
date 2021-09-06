# Multi Lingual (Medium)

## Description

Now, I'm finding machine language expert!

nc multilingual.eatpwnnosleep.com 9999


## Comments

The key idea for solving this challenge is `detect and jump`.

If you can build bytes code that can detect architecture and jump to archtecture-specific logic,
You can solve this challenge.

1. Detecting X86 or X86_64

Use `\x90\xEB\x0D\x31`: `nop; jmp 0x1000010; .byte 0x31`.
`0x31` prevents aarch64, mips from crash

2. Detecting aarch64

Use `\x17\x00\x00\x14`: `b 0x100005c`: Jump to the 0x100005c

3. Jump to the mips-specific logic

Use `\x21\x00\x40\x08`: `j 0x1000084`: Jump to the 0x1000084

4. Distinguish X86 and X86_64

You can distinguish X86 and X86_64 with `\x66\x31\xC9\x41\x57\x83\xF9\x01\x74\x1C`
which is <code>xor cx, cx;push r15;cmp ecx, 1;je $+30</code> in X86_64, and 
<code>xor cx,cx; inc ecx; push edi;cmp ecx,0x1;je $+0x26</code> in X86.

5. Setup arch-specific logic at jump location
