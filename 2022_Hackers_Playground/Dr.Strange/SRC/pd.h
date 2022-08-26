
#include <stdlib.h>
#include <stdint.h>

typedef struct arr_type {
    unsigned int val1;
    unsigned int val2;
    char value[1024-8];
}arr_type;  
arr_type * get_eTable_ptr(int idx);