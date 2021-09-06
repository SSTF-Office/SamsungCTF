mpz_t A, B, P;

typedef struct _point {
    mpz_t x;
    mpz_t y;
} point;

void point_init(point *p);
void point_free(point *p);
void point_copy(point *dst, point *src);

void point_add(point *res, point *p, point *q);
void point_neg(point *res, point *p);
void point_sub(point *res, point *p, point *q);
void point_mul_ui(point *res, point *p, unsigned int k);
void point_mul(point *res, point *p, mpz_t _k);

void mpz_mulinv(mpz_t res, mpz_t n, mpz_t p);

void lib_init();
