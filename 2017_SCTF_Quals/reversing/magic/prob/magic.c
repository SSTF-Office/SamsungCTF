#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

typedef unsigned long long ull;

ull serial;
ull table1[4] = {
    0x577402d733aa7e7,
    0x69bfdbf76396db95,
    0xbf4477de78df7ce1,
    0x6496707f80b1999f,
};

ull table2[4] = {
    32083,
    23189,
    21929,
    20849,
};

ull table3[4] = {
    46439,
    58309,
    60953,
    65099,
};

void checkAndParse(char *s) {
    int i;
    unsigned int tmp[2];
    if(strlen(s) != 17) {
        printf("format error\n");
        exit(-1);
    }

    if(s[8] != '-') {
        printf("format error\n");
        exit(-1);
    }
    for(i = 0; i < 2; i++) {
        tmp[i] = strtoul(s+(i*9), 0, 16);
    }
    serial = ((ull)tmp[0] << 32) + tmp[1];
}

void auth() {
    int i, j;
    ull tmp;
    ull res[4];
    ull res_res = 0;
    unsigned char s[17];
    unsigned char hash[SHA_DIGEST_LENGTH];
    unsigned long long chk[3] = {0x865530e1da2be0b2, 0x1e4f88f352820730, 0x0000000006d51c1a};
    memset(s, 0, 17);

    for(i = 0; i < 4; i++) {
        asm(
                "mov %1, %%rax\n"
                "mov %2, %%rdx\n"
                "mul %%rdx\n"
                "mov %%rdx, %0\n"
                : "=r"(res[i])
                : "r"(serial), "r"(table1[i])
           );
    }

    res[0] = table2[0] * ((((serial - res[0]) >> 1) + res[0]) >> 0xe);
    res[1] = table2[1] * ((((serial - res[1]) >> 1) + res[1]) >> 0xe);
    res[2] = table2[2] * (res[2] >> 0xe);
    res[3] = table2[3] * (res[3] >> 0xd);

    for(i = 0; i < 4; i++) {
        tmp = serial - res[i];
        for(j = 0; j < 4; j++) {
            if (j != i) tmp *= table3[j];
        }
        res_res += tmp;
    }

    if(res_res != 14132225825983922664U) {
        printf("check error\n");
        exit(-1);
    }

    sprintf(s, "%016llx", serial);
    SHA1(s, 16, hash);

    if(memcmp(hash, chk, 20)) {
        printf("check error\n");
        exit(-1);
    }
}

int main() {
    int i;
    char buf[100];
    memset(buf, 0, 100);
    printf("Input serial : ");
    scanf("%17s", buf);
    checkAndParse(buf);

    auth();

    printf("Congratulation, you got a flag!\n");
    system("/bin/cat flag");

    return 0;
}
