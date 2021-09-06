# Catch the bug

## Description

Catch the bug!

(ip : port, binary link, libc link)

## Flag

`SCTF{Y0u_4r3_7h3_3xp3r7_0f_BUGS!}`

## Challenge setup
```/bin/sh
$ cd deploy/
$ (sudo) ./build_docker.sh
$ (sudo) ./run_docker.sh
```

## Exploit

### phase 1 : Leak libc base address
There is the format string bug on the bug inspection(menu 2).
Using this vulnerability, the attacker can easily find the libc base address,
since `rsi` holds the value `_IO_2_1_stdout_+131` when the name of the bug is
printed.

### phase 2 : Trigger arbitrary write
At the report submission(menu 3), there is buffer overflow in the global
variable `global_report.buf` when the attacker caught at least one butterfly.
By using this buffer overflow, the attacker can overwrite `global_report.cursor`
and `global_report.password_cursor`.

### phase 3 : Overwrite exit_func_list in libc and trigger __free_hook
After the report submission(menu 3), the program's main function will return.
To hijack control flow, the attacker need to use other functions in `libc`.
After `__libc_start_main` function calls `main` function, it calls `libc_exit`
function.
Inside this function, it uses `exit_func_list` to call `exit_func`.
But it is hard to trigger `exit_func` since it uses pointer guard.
Instead, `free` function is called when one `exit_func_list` element is well
handled.
Hence, the attacker should use above arbitrary write to overwrite
`exit_func_list` to `__free_hook`, and make fake chunk on `__free_hook`.
