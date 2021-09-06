#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LEN 10000

void str_n_cpy(char *dst, char *src, int n) {
    int i = 0;
    while (src[i] && (i < n)) {
        dst[i] = src[i];
        i++;
    }
}

bool cmp(char *a, char *b) { // if a >= b return true
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] < b[i]) return false;
        else if (a[i] > b[i]) return true;
        i++;
    }

    return true;
}

int main() {
    bool flag;
    int i, j;
    char* tmp_cand = (char *)malloc(MAX_LEN + 1);
    char* tmp_rotate = (char *)malloc(MAX_LEN + 1);
    char* inp = (char *)malloc(MAX_LEN + 1);
    memset(inp, 0, MAX_LEN + 1);
    fgets(inp, MAX_LEN, stdin);

    char* nl = strchr(inp, '\n'); // find and remove newline char to null char
    if (nl != 0) {
        *nl = 0;
    }

    unsigned int str_len = strlen(inp);

    while (str_len >= 1) {
        for (i = 0; i < (strlen(inp) - str_len + 1); i++) {
            memset(tmp_cand, 0, str_len + 1);
            str_n_cpy(tmp_cand, (char *)(inp + i), str_len);

            flag = true;
            for (j = 1; j < str_len; j++) {
                memset(tmp_rotate, 0, str_len + 1);
                str_n_cpy(tmp_rotate, (char *)(tmp_cand + j), str_len - j);
                str_n_cpy((char *)(tmp_rotate + str_len - j), tmp_cand, j);

                if (cmp(tmp_cand, tmp_rotate)) {
                    flag = false;
                    break;
                }
            }

            if (flag) {
                str_len = 1;
                break;
            }
        }
        str_len--;
    }

    printf("%s\n", tmp_cand);

    return 0;
}
