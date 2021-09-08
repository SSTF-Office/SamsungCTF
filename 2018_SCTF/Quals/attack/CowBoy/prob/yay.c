#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include "yay.h"

int ding_chunk_size[BIN_NUM] = {16, 32, 64, 128, 256, 512, 1024, 2048};

int ding_init() {
    int i, j;

    srand(time(NULL));
    size_t prefix = ((size_t)rand() >> 4);
    ding_base = mmap((void *)(prefix << 16), PAGE * 16,
            PROT_READ|PROT_WRITE,
            MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED,
            -1,
            0);

    if ((long long int)ding_base == -1) {
        return 1;
    }

    ding_mem_bitmap1 = (node *)malloc(BIN_NUM *
            sizeof(node));
    ding_mem_bitmap2 = (char **)malloc(BIN_NUM *
            sizeof(char *));

    for (i = 0; i < BIN_NUM; i++) {
        get_ptr(ding_mem_bitmap1, i)->next = NULL;
        ding_mem_bitmap2[i] = (char *)malloc(PAGE/ding_chunk_size[i]);

        for (j = 0; j < (PAGE/ding_chunk_size[i]); j++) {
            ding_mem_bitmap2[i][j] = 0;
        }
    }

    for(i = 0; i < 100000; i++) {
        malloc(rand() & 0xff);
    }

    return 0;
}

void* ding_malloc(unsigned short n) {
    int i, j;
    int chk_idx, inner_idx;

    void *res;
    node *new_chunk, *pivot_chunk;

    if (n > 2048) return (void *)-1;
    for (i = 0; i < BIN_NUM; i++) {
        if (n <= ding_chunk_size[i]) {
            chk_idx = i;
            break;
        }
    }

    inner_idx = -1;
    for (j = 0; j < (PAGE/ding_chunk_size[chk_idx]); j++) {
        if(!(ding_mem_bitmap2[chk_idx][j])) {
            ding_mem_bitmap2[chk_idx][j] = 1;
            inner_idx = j;
            break;
        }
    }

    if (inner_idx == -1) return (void *)-1;

    res = (void *)((size_t)ding_base + (chk_idx << 12) +
            inner_idx * ding_chunk_size[chk_idx]);

    new_chunk = (node *)malloc(sizeof(node));
    new_chunk->addr = res;

    pivot_chunk = get_ptr(ding_mem_bitmap1, chk_idx);
    while (pivot_chunk->next != NULL) {
        pivot_chunk = pivot_chunk->next;
    }

    pivot_chunk->next = new_chunk;

    return res;
}

void ding_free(void *p) {
    int chk_idx = ((size_t)p & 0xf000) >> 12;
    int inner_idx = ((size_t)p & 0xfff) / ding_chunk_size[chk_idx];

    node *pivot_chunk, *prev_pivot_chunk;

    ding_mem_bitmap2[chk_idx][inner_idx] = 0;

    prev_pivot_chunk = get_ptr(ding_mem_bitmap1, chk_idx);
    pivot_chunk = prev_pivot_chunk->next;
    while (pivot_chunk != NULL) {
        if (pivot_chunk->addr == p) {
            prev_pivot_chunk->next = pivot_chunk->next;
            free(pivot_chunk);
            break;
        }
        prev_pivot_chunk = pivot_chunk;
        pivot_chunk = pivot_chunk->next;
    }
    return;
}
