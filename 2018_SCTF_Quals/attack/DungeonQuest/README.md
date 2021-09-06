# Dungeon Quest

## Description

Can you kill the demon and get the flag?

Running on `Qemu user-level emulation` with heap randomization + libc
randomization.

## Comments



## How to solve

0. By analysing, you can easily find double free bug @ 0x401080.
1. Overwrite list head at global(0x4120D0) to main_arena using unsorted bin attack with double free bug (Something like in house of orange). You write any value to bss. 
2. You can get arbitrary write by modifying list structure in bss.
3. Write shellcode into bss and make function pointer in list to point it.
4. Call function pointer and get Shell! (qemu usermode does not have NX)
