#ifndef __FILESYS__
#define __FILESYS__

#include <stdio.h>
#define SECTOR_SIZE 0x100
#define ENTRY_NUM ((SECTOR_SIZE)/sizeof(size_t))


struct entry_block
{
    size_t direct_block[7];
    size_t iptrs[4];
    void *__attribute__((unused)) padding[ENTRY_NUM - 11];
};

struct sector
{
    char bytes[SECTOR_SIZE];
};

struct iptr
{
    size_t sectors[ENTRY_NUM];
};

struct myfile
{
    size_t disk_sector_no;
};

void init_filesys();
int filesys_read(struct myfile *, size_t, char *, size_t);
int filesys_write(struct myfile *, size_t, char *, size_t);
struct myfile *filesys_make();
void filesys_delete();
#endif

