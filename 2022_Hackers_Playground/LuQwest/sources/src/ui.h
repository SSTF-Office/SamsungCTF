#ifndef LUQWEST_UI_H
#define LUQWEST_UI_H

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void ornament(const char *prefix, int prefix_len,
              const char *infix, int infix_len,
              const char *suffix, int suffix_len,
              const char *filler, int filler_len,
              int length);

char *boxify(char c);
char *circlize(int c);

#endif //LUQWEST_UI_H
