#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "filesys.h"

volatile struct sector *mem_disk = 0;
bool filesys_init __attribute__((aligned (16))) = false;
bool *bitmaps = 0;

#define DISK_SIZE 0x40000

static inline void error(char *msg)
{
    printf("%s\n", msg);
    exit(-1);
}

static inline void assert_init()
{
    if(!filesys_init)
        error("filesystem is not initialized.");
}

/*
 * int find_first_free():
 *  find first free disk sector
 *  on error return -1
 *  else return free_sector_num
 */
static inline int find_first_free()
{
    assert_init();

    int i;
    for(i = 0; i < DISK_SIZE/SECTOR_SIZE; i++)
    {
        if(bitmaps[i] == false)
        {
            bitmaps[i] = true;
            return i;
        }
    }

    error("filesystem is full");
}


void init_filesys()
{
    mem_disk = malloc(DISK_SIZE);
    bitmaps = malloc(DISK_SIZE/SECTOR_SIZE);
    if(!mem_disk || !bitmaps)
        error("init_filesys() fail");

    memset((void *)mem_disk, '\xff', DISK_SIZE);
    filesys_init = true;
}

size_t byte_to_sector(struct entry_block *block, size_t offset)
{
    offset = (offset & (~(SECTOR_SIZE - 1))) >> 8;
    if(offset < 7)
        return block->direct_block[offset];
    else if(offset >= 7 && offset < ENTRY_NUM * 4 + 7)
    {
        offset = offset - 7;
        size_t iptr_no = (offset) / ENTRY_NUM;
        size_t iptr_ptr_no = (offset) % ENTRY_NUM;
        if(block->iptrs[iptr_no] != -1)
        {
            struct iptr *_iptr = (struct iptr *)&mem_disk[block->iptrs[iptr_no]];
            return _iptr->sectors[iptr_ptr_no];
        }
    }
    return -1;
}

int filesys_read(struct myfile *file, size_t offset, char *ptr, size_t size)
{
    assert_init();
    if(file)
    {
        size_t sector_t = file->disk_sector_no;
        struct entry_block *block = (struct entry_block *)&mem_disk[sector_t];

        int idx;
        for(idx = 0; idx < size; idx += SECTOR_SIZE)
        {
            size_t sector_no = byte_to_sector(block, idx + offset);
            if(sector_no == -1)
                return idx;
            struct sector *_sector = (struct sector *)&mem_disk[sector_no];
            memcpy(ptr + idx, _sector->bytes,
                    (size - idx) > SECTOR_SIZE ?
                    SECTOR_SIZE : (size - idx));
        }
        return size;
    }
    return -1;
}

int filesys_write(struct myfile *file, size_t offset, char *ptr, size_t size)
{
    assert_init();
    int i;
    if(file)
    {
        size_t sector_t = file->disk_sector_no;
        struct entry_block *block = (struct entry_block *)&mem_disk[sector_t];

        // file growth
        size_t sector_no = byte_to_sector(block, offset + size);
        if(sector_no == -1)
        {
            int allocate_sectors = ((offset + size) & (~(SECTOR_SIZE - 1))) >> 8;
            if(allocate_sectors >= ENTRY_NUM * 4 + 7)
                return -1;

            for(i = 0; i < allocate_sectors; i++)
            {
                if(i < 7 && block->direct_block[i] == -1)
                {
                    block->direct_block[i] = find_first_free();
                    struct sector *ptr = (struct sector *)&(mem_disk[block->direct_block[i]]);
                    memset(ptr->bytes, 0, SECTOR_SIZE);
                }
                else if(i >= 7 && i < ENTRY_NUM * 4 + 7)
                {
                    int offset = i - 7;
                    size_t iptr_no = offset / ENTRY_NUM;
                    size_t iptr_ptr_no = offset % ENTRY_NUM;
                    if(block->iptrs[iptr_no] == -1)
                    {
                        block->iptrs[iptr_no] = find_first_free();
                    }

                    struct iptr *_iptr = (struct iptr *)&(mem_disk[block->iptrs[iptr_no]]);

                    if(_iptr->sectors[iptr_ptr_no] == -1)
                    {
                        int res = find_first_free();
                        _iptr->sectors[iptr_ptr_no] = res;
                        struct sector *ptr = (struct sector *)&(mem_disk[res]);
                        memset(ptr->bytes, 0, SECTOR_SIZE);
                    }
                }
            }
        }

        // now write on file
        int idx;
        for(idx = 0; idx < size; idx += SECTOR_SIZE)
        {
            size_t sector_no = byte_to_sector(block, idx + offset);
            if(sector_no == -1)
                return idx;
            struct sector *_sector = (struct sector *)&mem_disk[sector_no];
            memcpy(_sector->bytes, ptr + idx,
                    (size - idx) > SECTOR_SIZE ?
                    SECTOR_SIZE : (size - idx));
        }
        return size;
    }
    return -1;
}

struct myfile *filesys_make()
{
    assert_init();

    int sector_t = find_first_free();
    if(sector_t == -1)
    {
        error("filesys make fail");
    }

    struct myfile *res = malloc(sizeof(struct myfile));
    res->disk_sector_no = sector_t;

    struct entry_block *block = (struct entry_block *)&mem_disk[sector_t];

    int i;
    for(i = 0; i < 7; i++)
        block->direct_block[i] = -1;

    for(i = 0; i < 4; i++)
        block->iptrs[i] = -1;

    return res;
}

void filesys_delete(struct myfile *file)
{
    assert_init();
    if(!file)
        return;

    int i, j;
    int sector_t = file->disk_sector_no;
    struct entry_block *block = (struct entry_block *)&mem_disk[sector_t];
    for(i = 0; i < 7; i++)
    {
        if(block->direct_block[i] != -1)
        {
            bitmaps[block->direct_block[i]] = false;
        }
    }

    struct iptr *iptrs;
    for(i = 0; i < 4; i++)
    {
        if(block->iptrs[i] != -1)
        {
            iptrs = (struct iptr *)&mem_disk[block->iptrs[i]];
            for(j = 0; j < ENTRY_NUM; j++)
            {
                if(iptrs->sectors[j] != -1)
                {
                    bitmaps[iptrs->sectors[j]] = false;
                }
            }
        }
    }
    bitmaps[sector_t] = false;
    free(file);
}

int pad()
{
    __asm__(
            "nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");
}
