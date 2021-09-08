# SObfuscator

## Description

```
SMachine is back! with brand-new obfuscation technique!
Do you want to see the source code of SMachine? [Click](https://github.com/kaishack/sctf2018_qual/blob/master/reversing/SMachine/prob/main.c)
You should notice that SMachine is slightly fixed to support SObfuscator.(This means current version is different with the public source code).
Please analyze the given program and get the flag!

[Download](https://s3.ap-northeast-2.amazonaws.com/sctf2018-final-binaries/sobfuscator.tar.gz_0777fe4837ad3abed57237e8a558744b9473f218)
```

## Obfuscation and ways to solve this challenge

This challenge is written based on the concept of One Instruction Set
Computer(OISC). Actually, this concept is not that new. I mean, if you see this
[link](https://en.wikipedia.org/wiki/One_instruction_set_computer), some of
references are published more than 10 years ago. Among several variation of
OISC, this challenge follows the version of subleq, subtract and branch if less
than or equal to zero. If you wanna know details, read the link.

My intention of writting this challenge is introducing the concept of OISC with
general perspective. Some of reverse engineers may encounter
[movfuscator](https://github.com/xoreaxeaxeax/movfuscator) within some CTF
reversing challenges. Movfuscator is just a variant of OISC. To show OISC can
work with other instruction sets, I designed SMachine and used it for
SObfuscator.

Obfuscation is not a way to prevent analyzing the target program. It just makes
the target program hard to analyze. The easiest way to solve this challenge is
just follow the code. Since I open the sourece code of SMachine, challengers can
easily write disassembler for the target program.

However, it is a kind of time-wasting job. If you reversing the target
disassembly, you may find some patterns into the program. For example, variables
used to obfuscate a single instruction are positioned at high memory addresses,
they are initialized before it is used, something like that. So you can
patternize them, and you can write a de-obfuscator for SObfuscator.

There is no the only answer for reversing challenges. You may try to analyze
this program dynamically with a little modification of SMachine since you have
source code of it even though it has been fixed. Whatever you tried, if you get
a flag, then it's done!. Enjoy this challenge.

## Flag

`SCTF{D1d_y0u_h34r_ab0u7_0i5C???}`
