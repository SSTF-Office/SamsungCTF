# thumb

## Description

```
Find the password.

[Download link to main.bin]
```

## Comments

Crackme.

ARM (thumb mode) baremetal binary, runs on versatile ARM926EJ-S CPU.
Vanilla qemu can run and debug this program.

In IDA, you need to load binary onto correct offset 0x10000.

Algorithm is simple Mersenne Twister and matrix transformations.
Reverting them is pretty straightforward.

## Deploy

Let players to download `main.bin` file.

