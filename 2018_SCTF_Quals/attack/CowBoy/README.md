# CowBoy

## Description

```
I made a new heap allocator.
Would you test this one?

(service address)
(binary & libc download link)
```

## How to run
In deploy folder,
```
# ./run.sh
```

## How to solve
1. When new heap chunk is created, next pointer is not initialized.
2. At fill menu(4), attacker can create free chunk. When bin size is same with the size of struct, written data will be filled over freed metadata. Thus, you can control next pointer of chunk metadata.
3. Using fake chunk metadata, you can point GOT from it. Leak libc base from GOT & Overwrite GOT
4. Overwrite free GOT with system address using given libc, and run system("cat flag").

## FLAG
```
SCTF{H4v3_y0u_ev3r_seen_CowBoy_B1B0P?}
```
