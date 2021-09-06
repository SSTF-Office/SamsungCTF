# Speculator

## Description

```
Speculator! The "r34l" CPU emulator.

Reverse program.bin and find flag.
```

## Comments

The Speculator is a CPU simulator that runs binary files written in a custom ISA.
It implements following architecutral features:

- Cache memory
- Branch prediction
- Speculative execution
- 5-stage pipeline: Fetch, Decode, Execute, Memory Access, WriteBack
- Data Forwarding
- Kernel/user memory priviledge separation

But as a reversing problem, just think it as another virtual machine reversing task.

This VM is very similar to MIPS. For example, [there are three types of instructions](https://en.wikibooks.org/wiki/MIPS_Assembly/Instruction_Formats) (R-format, I-format, J-format).

You can find opcodes and instruction semantics by analysing `Decode` and `Execute` pipeline stages.

After that, disassemble the `prob.bin`: which is a small Fiestel cipher.
