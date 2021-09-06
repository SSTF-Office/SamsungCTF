# FILESYSTEM

## description

```
LIBFIELSYS(3)                  Library Functions Manual                LIBFIELSYS(3)

NAME
     init_filesys, filesys_make, filesys_read, filesys_write, filesys_delete - memory-based filesystem.

LIBRARY
     Filesys C Library (-lfilesys)

SYNOPSIS
     #include <filesys.h>
     void
     init_filesys();

     struct myfile*
     filesys_make();

     int
     filesys_read(struct myfile *file, size_t offset, char *dst, size_t len);

     int
     filesys_write(struct myfile *file, size_t offset, char *src, size_t len);

     void
     filesys_delete(struct myfile *file)

DESCRIPTION
     Emulating Unix-like filesystem on memory.
     Before call function on this library, call init_filesys() to initialize filesystem.

     The filesys_make() return the pointer of file on success. If failed, program will be exited.

     The filesys_read() return the readbytes on success. If failed return -1.

     The filesys_write() return the writebytes on success. If failed return -1.

     The filesys_delete() free the file structure and unallocating sectors using for file.

FLAG
     Patch this library(link of libfilesys).
     There are two bugs and two flags.
     You can get flag by patching each bug.

RESTRICTION
    DO NOT CHANGE the maximum size of filesystem.
    DO NOT CHANGE the size of binary.
    `sum(map(lambda x, y: return x != y, orig_data, patched_data)) < 120` should be
    satisfied.

POSTSCRIPT
     Your binary will be tested with my program as library.
     You can submit your answer ONCE PER FIVE MINUTES.
     In the test code, there is no direct access to the structure defined in
     library and no illegal argument when calling the library function.
     I make some big space for your convenience.
```

[binary will be given here]


## structure

Unix-like filesystem.

There are 7 direct blocks, and 4 indirect blocks.

## bugs
### Part1
uninitialized disk block while creation of indirect block.

initialize disk block (e.g, using memset).


### Part2
Race condition when allocating disk_sector since there is no lock while set bitmap.

Add lock_acquire, lock_release!

