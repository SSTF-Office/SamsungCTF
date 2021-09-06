# memo

## Description
Simple memo server

(ip : port, binary link)

## Challenge setup
```/bin/sh
$ echo "core" > /proc/sys/kernel/core_pattern
$ cd deploy/
$ docker build -t memo .
$ docker run -p $PORT:8888 -it memo [$PORT : host port]
```

## Exploit

### phase 1 : Create coredump
After calling `abort`, `core` file is stored at the current directory.
So, we can load `core` file as a memo.

### phase 2 : Heap overflow
When we load `core` file, we can trigger heap overflow.
Then, we can corrupt other memo structures.
As a result, we can get the arbitrary read/write ability.

### phase 3 : Exploit
With arbitrary read/write, leak the base address of libc and overwrite
`free_hook` to `system`.
Finally, get a shell by deleting the memo which content is `/bin/sh`.
