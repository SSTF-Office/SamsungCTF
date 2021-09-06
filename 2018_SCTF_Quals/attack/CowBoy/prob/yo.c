#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "yay.h"

extern int ding_chunk_size[BIN_NUM];

void do_ding_malloc() {
    unsigned short n;

    puts("Let's ding_malloc!");
    printf("Give me size n < 2049: ");
    scanf("%hu", &n);
    getchar();

    void *p = ding_malloc(n);

    printf("ding_malloc(%d) = %p\n", n, p);
}

void do_ding_free() {
    unsigned short n;
    int bin_idx, inner_idx;
    node *tmp_chunk;

    printf("bin num? : ");
    scanf("%hu", &n);
    getchar();
    bin_idx = n;

    printf("chunk num? : ");
    scanf("%hu", &n);
    getchar();
    inner_idx = n;

    tmp_chunk = get_ptr(ding_mem_bitmap1, bin_idx)->next;
    while (inner_idx) {
        tmp_chunk = tmp_chunk->next;
        inner_idx--;
    }

    ding_free(tmp_chunk->addr);
}

void do_show() {
    int i;
    node *pivot_chunk;
    for (i = 0; i < BIN_NUM; i++) {
        printf("bin[%d]: ", i);

        pivot_chunk = get_ptr(ding_mem_bitmap1, i)->next;
        while (pivot_chunk != NULL) {
            printf("%p ", pivot_chunk->addr);
            pivot_chunk = pivot_chunk->next;
        }

        puts("");
    }
}

void do_fill() {
    unsigned short n;
    int bin_idx, inner_idx;
    node *tmp_chunk;
    char *buf;
    size_t chk_size;

    printf("bin num? : ");
    scanf("%hu", &n);
    getchar();
    bin_idx = n;

    printf("chunk num? : ");
    scanf("%hu", &n);
    getchar();
    inner_idx = n;

    tmp_chunk = get_ptr(ding_mem_bitmap1, bin_idx)->next;
    while (inner_idx) {
        tmp_chunk = tmp_chunk->next;
        inner_idx--;
    }

    chk_size = ding_chunk_size[((size_t)tmp_chunk->addr & 0xf000) >> 12];

    buf = (char *)malloc(chk_size);
    memset(buf, 0, chk_size);

    printf("input: ");
    n = read(0, buf, chk_size);
    memcpy(tmp_chunk->addr, buf, n);
    free(buf);
}

void do_exit() {
    puts("Bye!");
    exit(0);
}

void print_memu() {
    puts("----------------------------------------");
    puts("[*] Testing dinggul's allocator...");
    puts("");
    puts("1. alloc");
    puts("2. free");
    puts("3. show heap chunks");
    puts("4. fill data");
    puts("5. exit");
    puts("----------------------------------------");
    return;
}

int main() {
    int menu;

    if (ding_init()) {
        puts("call admin");
        exit(1);
    }

    while (1) {
        print_memu();
        menu = getchar()-0x30;
        getchar();
        switch (menu) {
            case 1:
                do_ding_malloc();
                break;
            case 2:
                do_ding_free();
                break;
            case 3:
                do_show();
                break;
            case 4:
                do_fill();
                break;
            case 5:
                do_exit();
                break;
            default:
                puts("Wrong input :P");
                break;
        }
    }
    return 0;
}
