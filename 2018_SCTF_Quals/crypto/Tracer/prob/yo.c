#include "gmp.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "yay.h"

char hex_flag[100];
char flag[100];
mpz_t mpz_flag, y_2, r_num, priv_key;
point base_point, generator, pub_key, rnd_key, c_text, p_text;

void init() {
    int i, fd;
    memset(flag, 0, 100);
    memset(hex_flag, 0, 100);

    fd = open("./flag", O_RDONLY);
    read(fd, flag, 100);
    for (i = 0; i < strlen(flag); i++) {
        sprintf((char *)(hex_flag + (i * 2)), "%02x", flag[i]);
    }

    mpz_init(mpz_flag);
    mpz_init(y_2);
    mpz_set_str(mpz_flag, hex_flag, 16);

    mpz_set(y_2, mpz_flag);
    mpz_mul(y_2, y_2, mpz_flag);
    mpz_mul(y_2, y_2, mpz_flag);
    mpz_submul(y_2, mpz_flag, A);
    mpz_add(y_2, y_2, B);

    mpz_mod(y_2, y_2, P);

    if (mpz_legendre(y_2, P) != 1) {
        puts("Cannot calculate quadratic residue of flag");
        exit(1);
    }

    point_init(&p_text);
    mpz_set(p_text.x, mpz_flag);

    mpz_t bit, tmp_k, tmp_base;
    mpz_init(bit);
    mpz_init(tmp_k);
    mpz_init(tmp_base);
    mpz_set(tmp_k, P);
    mpz_add_ui(tmp_k, tmp_k, 1);
    mpz_div_ui(tmp_k, tmp_k, 4);
    mpz_set(tmp_base, y_2);

    mpz_set_ui(p_text.y, 1);

    while (mpz_cmp_ui(tmp_k, 0) > 0) {
        mpz_mod_ui(bit, tmp_k, 2);
        mpz_div_ui(tmp_k, tmp_k, 2);

        if (mpz_cmp_ui(bit, 1) == 0) {
            mpz_mul(p_text.y, p_text.y, tmp_base);
            mpz_mod(p_text.y, p_text.y, P);
        }

        mpz_mul(tmp_base, tmp_base, tmp_base);
        mpz_mod(tmp_base, tmp_base, P);
    }

    point_init(&base_point);
    point_init(&generator);
    mpz_set_str(base_point.x, "c6858e06b70404e9cd9e3ecb662395b4429c648139053fb521f828af606b4d3dbaa14b5e77efe75928fe1dc127a2ffa8de3348b3c1856a429bf97e7e31c2e5bd66", 16);
    mpz_set_str(base_point.y, "11839296a789a3bc0045c8a5fb42c7d1bd998f54449579b446817afbd17273e662c97ee72995ef42640c550b9013fad0761353c7086a272c24088be94769fd16650", 16);

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_init(r_num);

    mpz_urandomb(r_num, state, 512);

    point_mul(&generator, &base_point, r_num);

    mpz_init(priv_key);
    point_init(&pub_key);

    mpz_urandomb(priv_key, state, 512);

    point_mul(&pub_key, &generator, priv_key);

    mpz_urandomb(r_num, state, 512);

    point_init(&rnd_key);
    point_mul(&rnd_key, &generator, r_num);

    point_mul(&c_text, &pub_key, r_num);
    point_add(&c_text, &c_text, &p_text);
}

int main(int argc, char *argv[]) {
    lib_init();
    init();

    printf("g = (");
    mpz_out_str(stdout, 10, generator.x);
    printf(", ");
    mpz_out_str(stdout, 10, generator.y);
    puts(")");

    printf("pub key = (");
    mpz_out_str(stdout, 10, pub_key.x);
    printf(", ");
    mpz_out_str(stdout, 10, pub_key.y);
    puts(")");

    printf("ciphertext = { (");
    mpz_out_str(stdout, 10, rnd_key.x);
    printf(", ");
    mpz_out_str(stdout, 10, rnd_key.y);
    printf("), (");
    mpz_out_str(stdout, 10, c_text.x);
    printf(", ");
    mpz_out_str(stdout, 10, c_text.y);
    puts(") }");



    /*
    point ptext;
    point_init(&ptext);

    point_mul(&ptext, &rnd_key, priv_key);
    point_sub(&ptext, &c_text, &ptext);

    mpz_out_str(stdout, 16, c_text.x);
    puts("");
    mpz_out_str(stdout, 16, c_text.y);
    puts("");
    puts("");

    mpz_out_str(stdout, 16, ptext.x);
    puts("");
    mpz_out_str(stdout, 16, ptext.y);
    puts("");
    puts("");

    mpz_out_str(stdout, 16, p_text.x);
    puts("");
    mpz_out_str(stdout, 16, p_text.y);
    puts("");
    */
    return 0;
}
