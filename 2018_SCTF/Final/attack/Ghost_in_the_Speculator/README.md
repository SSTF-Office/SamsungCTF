# Ghost in the Speculator

## Description

(This problem uses the same Speculator binary as the [Speculator](/reversing/Speculator) binary.)

There is a legend about this speculator: Ghost's treasure!

They say that the treasure is located at 0xdeadbeef.

However, I do not see a route to there.

Can you find it for me?

P.S. the Ghost's name is [Spectre](https://meltdownattack.com/)

## Comments

Exploit a Spectre variant 2 (branch target injection) bug in a MIPS-like CPU simulator.

## How to solve

### Goal

This program runs two programs, one for kernel and one for user.
The CTF service runs the `kernel.bin` as kernel and user-submitted binary as user program.

Flag is loaded onto the address 0xdeadbeef, which is in the kernel memory space.
User submitted programs cannot read the flag using ordinary `load` instruction.

### Branch Target Injection

The branch predictor is a simple table that uses `(pc >> 2) & 31` as its lookup index.

Then, for example, branch target of 0x40001c (user space) affects the
prediction result for branch at 0x8000001c (kernel space)

```
0x40001c: beq $10, $10, 0x400020   ; Record a branch 0x40001c -> 0x400020
0x400020: lw $9, ($9)    ; Load a word from the address in register $9.
```

After that, when following kernel instruction is executed:

```
0x8040001c: beq $4, $5, _out_of_bounds
```

The branch prediction unit returns the user address 0x400020.

The CPU runs a few cycle before knowing that the prediction was wrong.
During that few cycles, instruction 0x400020 runs.
Soon the CPU recognizes that the prediction failed.
All the changes caused by 0x400020 are canceled except cache memory.

### Collecting the secret

Using the bug, load the flag on the cache memory.

The last piece of bug is that the user can access cache memory for kernel memory.

From here, you can load the flag and use it in the user program.

