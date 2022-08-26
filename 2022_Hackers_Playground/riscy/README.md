Have you heard of RISCV? There is a rumor that it is the most _secure_
ISA. Let's see how RISCY is RSICV? Let's ROP!

Check `Dockerfile`, `build.sh` and `run.sh` for emulation.

To emulate,

    $ qemu-riscv64 ./target

To debug,

    $ qemu-riscv64 -g 9000 ./target
    $ gdb-multiarch
    (gdb) target remote localhost:9000
    (gdb) c

To disassemble,

    $ /usr/riscv64-linux-gnu/bin/objdump -d ./target

Good luck!

* Refs
- https://content.riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf


## Solving Strategy
ROP in RISCV do not require any special technique!

Note that there are useful gadgets in the binary:

    43794:       70e2                    ld      ra,56(sp)
    43796:       7442                    ld      s0,48(sp)
    43798:       6522                    ld      a0,8(sp)
    4379a:       74a2                    ld      s1,40(sp)
    4379c:       7902                    ld      s2,32(sp)
    4379e:       69e2                    ld      s3,24(sp)
    437a0:       6121                    addi    sp,sp,64
    437a2:       8082                    ret

    10a56                                ld      a5,0(s0)
    10a58:       8652                    mv      a2,s4
    10a5a:       85ce                    mv      a1,s3
    10a5c:       854a                    mv      a0,s2
    10a5e:       0485                    addi    s1,s1,1
    10a60:       9782                    jalr    a5
    10a62:       0421                    addi    s0,s0,8
    10a64:       fe9a99e3                bne     s5,s1,10a56 <__libc_csu_init+0x6c>
    10a68:       70e2                    ld      ra,56(sp)
    10a6a:       7442                    ld      s0,48(sp)
    10a6c:       74a2                    ld      s1,40(sp)
    10a6e:       7902                    ld      s2,32(sp)
    10a70:       69e2                    ld      s3,24(sp)
    10a72:       6a42                    ld      s4,16(sp)
    10a74:       6aa2                    ld      s5,8(sp)
    10a76:       6121                    addi    sp,sp,64
    10a78:       8082                    ret

Using those gadgets build ROP chains of `open()`, `read()`, and `write()` functions, and get the flag!

We can also return to `start()` if the buffer is too small to put all the payload at once.
