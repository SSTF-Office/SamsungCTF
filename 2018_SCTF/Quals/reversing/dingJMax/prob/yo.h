#define LANE_Y_MIN 2
#define LANE_Y_MAX 21
#define LANE_Y_LEN 20

#define D_OFFSET 5
#define F_OFFSET 11
#define J_OFFSET 17
#define K_OFFSET 23

#define NOTE "o"
#define NOT_NOTE " "

#define DIVISOR 20

#define MAX_SCORE 1000000

//char encrypted_flag[] = "I_w0u1d_l1k3_70_d3v3l0p_GUI_v3rs10n_n3x7_t1m3";
char encrypted_flag[] = "qN7BuRx4rElDv84dgNaaNBanZf0HSHFjqOvbkFfgTRg3r";

char *field_lane[] = {
    "┌─────┬─────┬─────┬─────┐",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "│     │     │     │     │",
    "┝━━━━━┿━━━━━┿━━━━━┿━━━━━┥",
    "│     │     │     │     │",
    "│  d  │  f  │  j  │  k  │",
    "│     │     │     │     │",
    "└─────┴─────┴─────┴─────┘",
};

char *logo[] = {
    "      _ _                  _ __  __            ",
    "     | (_)                | |  \\/  |           ",
    "   __| |_ _ __   __ _     | | \\  / | __ ___  __",
    "  / _` | | '_ \\ / _` |_   | | |\\/| |/ _` \\ \\/ /",
    " | (_| | | | | | (_| | |__| | |  | | (_| |>  < ",
    "  \\__,_|_|_| |_|\\__, |\\____/|_|  |_|\\__,_/_/\\_\\",
    "                 __/ |                         ",
    "                |___/                          ",
};

char *inst[] = {
    " 1. Press D, F, J, K Button when 'o' is closed to baseline(thick one)",
    " 2. You can get score depending on your correctness",
    " 3. If you got MAX SCORE(1000000), you would see correct flag on flag dialog",
    " 4. This is a really hard game, so you need some practice :)",
    " 5. Bad luck :P",
};

int p_count;
int g_count;
int m_count;
int all_count;

char lane_buf[20][4];

char *log_msg;
int log_frame;
