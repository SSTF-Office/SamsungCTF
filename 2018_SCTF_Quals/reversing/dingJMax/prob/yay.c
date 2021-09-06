#include <string.h>

#include "yay.h"

void yay_init() {
    int i;
    idx_i = 0;
    idx_j = 0;

    str_dict = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_!";
    dict_len = strlen(str_dict);
    for (i = 0; i < dict_len; i++) {
        S[i] = i;
    }
}

unsigned int get_idx(unsigned char c) {
    int i;
    for (i = 0; i < dict_len; i++) {
        if (c == str_dict[i]) return i;
    }

    return -1;
}

unsigned char next_chr(int n) {
    int loop_cnt = 0;
    unsigned char tmp, res;

    while (n > loop_cnt) {
        idx_i = (idx_i + 1) % dict_len;
        idx_j = (idx_j + S[idx_i]) % dict_len;
        tmp = S[idx_i];
        S[idx_i] = S[idx_j];
        S[idx_j] = tmp;

        res = S[(S[idx_i] + S[idx_j]) % dict_len];

        loop_cnt++;
    }

    return str_dict[res];
}

unsigned char xor_chr(unsigned char a, unsigned char b) {
    unsigned int A = get_idx(a);
    unsigned int B = get_idx(b);

    return str_dict[(A ^ B) % dict_len];
}
