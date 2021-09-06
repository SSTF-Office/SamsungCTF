#include <stdint.h>

#define UART_BASE 0x101f1000
static volatile unsigned int* UARTDR = (unsigned int*)(UART_BASE + 0x000);
static volatile unsigned int* UARTFR = (unsigned int*)(UART_BASE + 0x018);
#define FR_TXFE 7  // Transmit FIFO empty
#define FR_RXFF 6  // Receive FIFO full
#define FR_TXFF 5  // Transmit FIFO full
#define FR_RXFE 4  // Receive FIFO is empty

#define bittest(addr, pos) (*(addr) & (1 << (pos)))

void uart_putchar(char c) {
    while (bittest(UARTFR, FR_TXFF));
    *UARTDR = (unsigned int)c;
}

int uart_getchar() {
    while (!bittest(UARTFR, FR_RXFF));
    return *UARTDR & 0xff;
}

void uart_puts(const char *s) {
     while(*s != '\0')
         uart_putchar(*s++);
}

#ifdef DEBUG
int printnum(int x) {
    if (x == 0) {
        uart_putchar('0');
        return 1;
    }
    char buf[16] = {' ',};
    int len = 0;
    while (x > 0) {
        buf[len++] = (x % 10) + '0';
        x = x / 10;
    }
    int pos = len - 1;
    while (pos >= 0) {
        uart_putchar(buf[pos--]);
    }
    return len;
}
static const char* hextab = "0123456789abcdef";
void printhex(int x) {
    int hi = (x >> 4) & 0xf;
    int lo = x & 0xf;
    uart_putchar(hextab[hi]);
    uart_putchar(hextab[lo]);
}
#endif

#if 0
static unsigned int rand_state = 1;
void srand(unsigned int seed) {
    rand_state = seed;
}
unsigned int rand() {
    rand_state = (1103515245 * rand_state + 12345) & 0x7fffffff;
    return rand_state & 0xffff;
}

#else
enum
{
    N = 624,
    M = 397,
    R = 31,
    A = 0x9908B0DF,
    F = 1812433253,
    U = 11,
    S = 7,
    B = 0x9D2C5680,
    T = 15,
    C = 0xEFC60000,
    L = 18,
    MASK_LOWER = (1ull << R) - 1,
    MASK_UPPER = (1ull << R)
};

static uint32_t mt[N];
static uint16_t index;
void my_srand(uint32_t seed)
{
    uint32_t  i;
    mt[0] = seed;
    for ( i = 1; i < N; i++ )
        mt[i] = (F * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
    index = N;
}

static void Twist()
{
    uint32_t  i, x, xA;
    for ( i = 0; i < N; i++ )
    {
        x = (mt[i] & MASK_UPPER) + (mt[(i + 1) % N] & MASK_LOWER);
        xA = x >> 1;
        if ( x & 0x1 )
            xA ^= A;
        mt[i] = mt[(i + M) % N] ^ xA;
    }
    index = 0;
}

static int cnt = 0;
uint32_t my_rand()
{
    uint32_t  y;
    int       i = index;
    if ( index >= N )
    {
        Twist();
        i = index;
    }
    y = mt[i];
    index = i + 1;
    y ^= (mt[i] >> U);
    y ^= (y << S) & B;
    y ^= (y << T) & C;
    y ^= (y >> L);
    return y;
}
#endif
