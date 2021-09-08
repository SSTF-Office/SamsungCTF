/*
Feistel cipher.
32-bit block.
16-bit half block.
8-rounds.
ECB mode.

Round keys: 16-bit * 8 = 128 bit.

// L: $8, R: $9
$10 = F(R, K)
$10 ^= $8
$8 = $9
$9 = $10

F(R, K):
    R = $9
    Split R into 4 nibbles.
    Run s-box.
    xor with rk.
    rol by 3.
    return $10.
*/

#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

const u8 sbox[16] = {
    0xc,0x2,0x7,0x1,0xf,0xe,0x3,0xa,0x0,0x6,0xd,0xb,0x5,0x8,0x4,0x9
};

u32 f(u16 x) {
    u8 x0 = (x      ) & 0xf;
    u8 x1 = (x >>  4) & 0xf;
    u8 x2 = (x >>  8) & 0xf;
    u8 x3 = (x >> 12) & 0xf;

    u8 y0 = sbox[x0];
    u8 y1 = sbox[x1];
    u8 y2 = sbox[x2];
    u8 y3 = sbox[x3];

    u16 y = 0;
    y |= y0;
    y |= y1 << 4;
    y |= y2 << 8;
    y |= y3 << 12;

    u16 hi = y << 3;
    u16 lo = y >> 13;
    u32 z = (hi | lo) << 16;
    return z >> 16;
}

int main() {
    int i;
    u32 pt[8];
    u32 ct[8];
    memcpy(pt, "SCTF{HomebReweD__MIPS__Emulat0r}", 32);

    printf("_sbox:\n");
    for (i=0; i<16; i++)
        printf(".word 0x%02x\n", sbox[i]);

    printf("_pt:\n");
    for (i=0; i<8; i++)
        printf(".word 0x%08x\n", htonl(pt[i]));

    int j;
    for (j=0; j<8; j++) {
        u32 B = ntohl(pt[j]);
        u32 L, R, F;
        for (i=0; i<8; i++) {
            R = B & 0xffff;
            L = B >> 16;

            F = f(R);
            F ^= L;
            L = R;
            R = F;

            B = (L << 16) | R;
        }
        ct[j] = B;
    }

    printf("_ans:\n");
    for (j=0; j<8; j++)
        printf(".word 0x%08x\n", ct[j]);

    return 0;
}
