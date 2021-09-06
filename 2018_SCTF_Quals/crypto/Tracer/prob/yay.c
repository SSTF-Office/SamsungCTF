#include <gmp.h>
#include <stdbool.h>
#include <stdio.h>

#include "yay.h"

void point_init(point *p) {
    mpz_init(p->x);
    mpz_init(p->y);
}

void point_free(point *p) {
    mpz_clear(p->x);
    mpz_clear(p->y);
}

void point_copy(point *dst, point *src) {
    mpz_set(dst->x, src->x);
    mpz_set(dst->y, src->y);
}


void mpz_mulinv(mpz_t res, mpz_t n, mpz_t p) {
    mpz_t g, t;
    mpz_init(g);
    mpz_init(t);

    mpz_gcdext(g, res, t, n, p);
    mpz_mod(res, res, p);

    mpz_clear(g);
    mpz_clear(t);
}

void point_add(point *res, point *p, point *q) {
    mpz_t tmp1, tmp2, tmp3, m;
    point p_copy, q_copy, res_copy;

    point_init(&p_copy);
    point_init(&q_copy);
    point_init(&res_copy);
    point_copy(&p_copy, p);
    point_copy(&q_copy, q);

    mpz_init(tmp1);
    mpz_init(tmp2);
    mpz_init(tmp3);
    mpz_init(m);

    if ((mpz_cmp(p->x, q->x) == 0) && (mpz_cmp(p->y, q->y) == 0)) {
        mpz_mul_ui(tmp1, p_copy.y, 2);
        mpz_mul_ui(tmp2, p_copy.x, 3);
        mpz_mul(tmp2, tmp2, p_copy.x);
        mpz_sub(tmp2, tmp2, A);
        mpz_mulinv(tmp3, tmp1, P);
        mpz_mul(m, tmp3, tmp2);
        mpz_mod(m, m, P);

        mpz_mul(res_copy.x, m, m);
        mpz_sub(res_copy.x, res_copy.x, p_copy.x);
        mpz_sub(res_copy.x, res_copy.x, p_copy.x);
        mpz_mod(res_copy.x, res_copy.x, P);

        mpz_sub(res_copy.y, p_copy.x, res_copy.x);
        mpz_mul(res_copy.y, res_copy.y, m);
        mpz_sub(res_copy.y, res_copy.y, p_copy.y);
        mpz_mod(res_copy.y, res_copy.y, P);
    } else {
        mpz_sub(tmp1, p_copy.y, q_copy.y);
        mpz_sub(tmp2, p_copy.x, q_copy.x);
        mpz_mulinv(tmp3, tmp2, P);
        mpz_mul(m, tmp1, tmp3);
        mpz_mod(m, m, P);

        mpz_mul(res_copy.x, m, m);
        mpz_sub(res_copy.x, res_copy.x, p_copy.x);
        mpz_sub(res_copy.x, res_copy.x, q_copy.x);
        mpz_mod(res_copy.x, res_copy.x, P);

        mpz_sub(res_copy.y, p_copy.x, res_copy.x);
        mpz_mul(res_copy.y, res_copy.y, m);
        mpz_sub(res_copy.y, res_copy.y, p_copy.y);
        mpz_mod(res_copy.y, res_copy.y, P);
    }

    point_copy(res, &res_copy);

    point_free(&p_copy);
    point_free(&q_copy);
    point_free(&res_copy);

    mpz_clear(tmp1);
    mpz_clear(tmp2);
    mpz_clear(tmp3);
    mpz_clear(m);
}

void point_neg(point *res, point *p) {
    point_copy(res, p);
    mpz_neg(res->y, res->y);
    mpz_mod(res->y, res->y, P);
}

void point_sub(point *res, point *p, point *q) {
    point q_copy;
    point_init(&q_copy);

    point_neg(&q_copy, q);
    point_add(res, p, &q_copy);

    point_free(&q_copy);
}

void point_mul_ui(point *res, point *p, unsigned int k) {
    int bit;
    bool set = false;

    point tmp;
    point_init(&tmp);
    point_copy(&tmp, p);

    while (k > 0) {
        bit = k % 2;
        k >>= 1;

        if (bit) {
            if (set) {
                point_add(res, res, &tmp);
            } else {
                point_copy(res, &tmp);
                set = true;
            }
        }

        point_add(&tmp, &tmp, &tmp);
    }

    point_free(&tmp);
}

void point_mul(point *res, point *p, mpz_t _k) {
    bool set = false;
    mpz_t k;
    mpz_init_set(k, _k);

    point tmp;
    point_init(&tmp);
    point_copy(&tmp, p);

    mpz_t bit;
    mpz_init(bit);

    while (mpz_cmp_ui(k, 0) > 0) {
        mpz_mod_ui(bit, k, 2);
        mpz_div_ui(k, k, 2);

        if (mpz_cmp_ui(bit, 1) == 0) {
            if (set) {
                point_add(res, res, &tmp);
            } else {
                point_copy(res, &tmp);
                set = true;
            }
        }

        point_add(&tmp, &tmp, &tmp);
    }

    point_free(&tmp);
}

void lib_init() {
    int i;
    mpz_init_set_ui(A, 3);
    mpz_init(B);
    mpz_set_str(B, "51953eb9618e1c9a1f929a21a0b68540eea2da725b99b315f3b8b489918ef109e156193951ec7e937b1652c0bd3bb1bf073573df883d2c34f1ef451fd46b503f00", 16);
    mpz_init_set_ui(P, 1);

    for (i = 0; i < 521; i++) {
        mpz_mul_ui(P, P, 2);
    }
    mpz_sub_ui(P, P, 1);
}
