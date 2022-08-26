#include "ui.h"

void ornament(const char *prefix, int prefix_len,
              const char *infix, int infix_len,
              const char *suffix, int suffix_len,
              const char *filler, int filler_len,
              int length) {
    if (prefix_len > 0) {
        printf("%s", prefix);
    }

    for (int i = 0; i < 2; ++i) {
        if (filler_len > 0) {
            int max = (length - (prefix_len + infix_len + suffix_len)) / 2;
            for (int j = 0; j < max; j += filler_len) {
                printf("%s", filler);
            }
        }

        if (i == 0) {
            if (infix_len > 0) {
                printf("%s", infix);
            }
        }
    }

    if (suffix_len > 0) {
        printf("%s", suffix);
    }
}

char *boxify(char c) {
    switch (c) {
        case 'A': case 'a': return "🅰";
        case 'B': case 'b': return "🅱";
        case 'C': case 'c': return "🅲";
        case 'D': case 'd': return "🅳";
        case 'E': case 'e': return "🅴";
        case 'F': case 'f': return "🅵";
        case 'G': case 'g': return "🅶";
        case 'H': case 'h': return "🅷";
        case 'I': case 'i': return "🅸";
        case 'J': case 'j': return "🅹";
        case 'K': case 'k': return "🅺";
        case 'L': case 'l': return "🅻";
        case 'M': case 'm': return "🅼";
        case 'N': case 'n': return "🅽";
        case 'O': case 'o': return "🅾";
        case 'P': case 'p': return "🅿";
        case 'Q': case 'q': return "🆀";
        case 'R': case 'r': return "🆁";
        case 'S': case 's': return "🆂";
        case 'T': case 't': return "🆃";
        case 'U': case 'u': return "🆄";
        case 'V': case 'v': return "🆅";
        case 'W': case 'w': return "🆆";
        case 'X': case 'x': return "🆇";
        case 'Y': case 'y': return "🆈";
        case 'Z': case 'z': return "🆉";
        default: return NULL;
    }
}

char *circlize(int c) {
    switch (c) {
        case 1: return "❶";
        case 2: return "❷";
        case 3: return "❸";
        case 4: return "❹";
        case 5: return "❺";
        case 6: return "❻";
        case 7: return "❼";
        case 8: return "❽";
        case 9: return "❾";
        case 10: return "❿";
        case 11: return "⓫";
        case 12: return "⓬";
        case 13: return "⓭";
        case 14: return "⓮";
        case 15: return "⓯";
        case 16: return "⓰";
        case 17: return "⓱";
        case 18: return "⓲";
        case 19: return "⓳";
        case 20: return "⓴";
        default: return NULL;
    }
}