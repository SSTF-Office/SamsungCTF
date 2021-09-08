#include <stdbool.h>

#define get_ptr(x, i) (&x[i])
#define PAGE 4096
#define BIN_NUM 8

typedef struct _node {
    void *addr;
    struct _node* next;
} node;

int ding_init();
void* ding_malloc(unsigned short n);
void ding_free(void *p);

void *ding_base;
node *ding_mem_bitmap1;
char **ding_mem_bitmap2;

// 16, 32, 64, 128, 256, 512, 1024, 2048
