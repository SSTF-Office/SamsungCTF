#include <locale.h>
#include <ncursesw/ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "yo.h"
#include "yay.h"

#include "stage.h"

void set_text(WINDOW *screen, char *text) {
    int x, y, i;
    getmaxyx(screen, y, x);
    mvwprintw(screen, y/2, (x - strlen(text))/2, text);
}

void decrypt_flag(char *flag_buf) {
    int flag_len = strlen(flag_buf);
    int i;
    for (i = 0; i < flag_len; i++) {
        flag_buf[i] = xor_chr(flag_buf[i], next_chr(1));
    }
}

void set_flag(WINDOW *screen, char *flag) {
    int x, y, i;
    getmaxyx(screen, y, x);
    mvwprintw(screen, y/2, (x - strlen(flag))/2-2, "SCTF{%s}", flag);
}

void set_score(WINDOW *screen, unsigned int score) {
    int x, y, i;
    getmaxyx(screen, y, x);
    mvwprintw(screen, y/2, 2, "%u", score);
}

void set_log(WINDOW *screen, char *log, int frame_num) {
    werase(screen);
    log_msg = log;
    log_frame = frame_num;
}

void show_log(WINDOW *screen) {
    if (log_frame > 0)
        set_text(screen, log_msg);
    else if (log_frame < 0)
        set_text(screen, "");
    else
        werase(screen);

    log_frame--;
}

void set_count(char **stage) {
    int i = 0;
    all_count = 0;
    while (stage[i]) {
        if (strchr(stage[i], 'o')) all_count++;
        i++;
    }
}

void set_title(WINDOW *screen, char *title) {
    mvwprintw(screen, 0, 2, title);
}

void draw_lanes(WINDOW *screen) {
    int i;
    for (i = 0; i < sizeof(field_lane)/sizeof(char *); i++) {
        mvwprintw(screen, i+1, 2, field_lane[i]);
    }

    return;
}

void instruction() {
    int i;

    for (i = 0; i < sizeof(inst)/sizeof(char *); i++) {
        mvprintw(i+2, 5, inst[i]);
    }

    mvprintw(13, 0, "Press any key to conitnue...");
    return;
}

void print_logo() {
    int i;
    for (i = 0; i < sizeof(logo)/sizeof(char *); i++) {
        mvprintw(i, 5, logo[i]);
    }

    mvprintw(13, 0, "Press any key to conitnue...");
    return;
}

int main() {
    bool D_PRESSED, F_PRESSED, J_PRESSED, K_PRESSED;
    unsigned long long loop_cnt = 0;
    int i, j;

    char *flag_buf = (char *)malloc(strlen(encrypted_flag) + 1);
    strcpy(flag_buf, encrypted_flag);

    unsigned long stage_len = sizeof(stage)/sizeof(char *);

    yay_init();

    setlocale(LC_ALL,"");
    initscr();
    keypad(stdscr, true);
    noecho();
    curs_set(false);

    print_logo();
    getch();
    erase();

    instruction();
    getch();
    werase(stdscr);
    wrefresh(stdscr);

    WINDOW *field = newwin(30, 29, 0, 0);
    WINDOW *score = newwin(5, 25, 0, 35);
    WINDOW *flag = newwin(5, 60, 7, 35);
    WINDOW *dialog = newwin(5, 40, 10, 10);
    WINDOW *log = newwin(5, 20, 15, 35);

    box(field, 0, 0);
    box(score, 0, 0);
    box(flag, 0, 0);
    box(dialog, 0, 0);
    box(log, 0, 0);

    draw_lanes(field);
    set_title(field, " dingJMax ");
    set_title(score, " SCORE (max 1000000) ");
    set_title(flag, " here is your FLAG (Did you get perfect score?) ");
    set_title(log, " Log ");

    set_text(dialog, "stage start");

    wrefresh(field);
    wrefresh(score);
    wrefresh(flag);
    wrefresh(dialog);
    wrefresh(log);

    getch();

    erase();

    set_count(stage);
    p_count = 0;
    g_count = 0;
    m_count = 0;
    nodelay(stdscr, true);
    set_log(log, "start", 500);

    for (i = 0; i < LANE_Y_LEN; i++) {
        for (j = 0; j < 4; j++) {
            lane_buf[i][j] = ' ';
        }
    }

    while(loop_cnt/DIVISOR < sizeof(stage)/sizeof(char *) - LANE_Y_LEN) {
        switch (getch()) {
            case 'd':
                D_PRESSED = true;
#if RELEASE
                next_chr(loop_cnt*'d');
                decrypt_flag(flag_buf);
#endif
                break;
            case 'f':
                F_PRESSED = true;
#if RELEASE
                next_chr(loop_cnt*'f');
                decrypt_flag(flag_buf);
#endif
                break;
            case 'j':
                J_PRESSED = true;
#if RELEASE
                next_chr(loop_cnt*'j');
                decrypt_flag(flag_buf);
#endif
                break;
            case 'k':
                K_PRESSED = true;
#if RELEASE
                next_chr(loop_cnt*'k');
                decrypt_flag(flag_buf);
#endif
                break;
            default:
                D_PRESSED = false;
                F_PRESSED = false;
                J_PRESSED = false;
                K_PRESSED = false;
                break;
        }

        box(field, 0, 0);
        box(score, 0, 0);
        box(flag, 0, 0);
        box(log, 0, 0);

        draw_lanes(field);
        set_title(field, " dingJMax ");
        set_title(score, " SCORE (max 1000000) ");
        set_title(flag, " Here is your FLAG (Did you get perfect score?) ");
        set_title(log, " Log ");

        if (loop_cnt % DIVISOR == 0) {
            for (i = LANE_Y_LEN-1; i > 0; i--) {
                memcpy(lane_buf[i], lane_buf[i-1], 4);
            }

            memcpy(lane_buf[0], stage[loop_cnt/DIVISOR], 4);
#if SOLUTION
            if (lane_buf[LANE_Y_LEN-1][0] == 'o') {
                next_chr(loop_cnt*'d');
                decrypt_flag(flag_buf);
            } else if (lane_buf[LANE_Y_LEN-1][1] == 'o') {
                next_chr(loop_cnt*'f');
                decrypt_flag(flag_buf);
            } else if (lane_buf[LANE_Y_LEN-1][2] == 'o') {
                next_chr(loop_cnt*'j');
                decrypt_flag(flag_buf);
            } else if (lane_buf[LANE_Y_LEN-1][3] == 'o') {
                next_chr(loop_cnt*'k');
                decrypt_flag(flag_buf);
            } else {
                // nothing
            }
#endif
        }

        for (i = LANE_Y_MIN; i <= LANE_Y_MAX; i++) {
            mvwprintw(field, i, D_OFFSET, "%c", lane_buf[i-LANE_Y_MIN][0]);
            mvwprintw(field, i, F_OFFSET, "%c", lane_buf[i-LANE_Y_MIN][1]);
            mvwprintw(field, i, J_OFFSET, "%c", lane_buf[i-LANE_Y_MIN][2]);
            mvwprintw(field, i, K_OFFSET, "%c", lane_buf[i-LANE_Y_MIN][3]);
        }

        if (D_PRESSED) {
            if (lane_buf[LANE_Y_LEN-1][0] == 'o') {
                if (loop_cnt % DIVISOR == 0) {
                    set_log(log, "PERFECT!", 200);
                    p_count++;
                } else {
                    set_log(log, "GREAT!", 200);
                    g_count++;
                }
            } else {
                set_log(log, "MISS!", 200);
                m_count++;
            }
        } else if (F_PRESSED) {
            if (lane_buf[LANE_Y_LEN-1][1] == 'o') {
                if (loop_cnt % DIVISOR == 0) {
                    set_log(log, "PERFECT!", 200);
                    p_count++;
                } else {
                    set_log(log, "GREAT!", 200);
                    g_count++;
                }
            } else {
                set_log(log, "MISS!", 200);
                m_count++;
            }
        } else if (J_PRESSED) {
            if (lane_buf[LANE_Y_LEN-1][2] == 'o') {
                if (loop_cnt % DIVISOR == 0) {
                    set_log(log, "PERFECT!", 200);
                    p_count++;
                } else {
                    set_log(log, "GREAT!", 200);
                    g_count++;
                }
            } else {
                set_log(log, "MISS!", 200);
                m_count++;
            }
        } else if (K_PRESSED) {
            if (lane_buf[LANE_Y_LEN-1][3] == 'o') {
                if (loop_cnt % DIVISOR == 0) {
                    set_log(log, "PERFECT!", 200);
                    p_count++;
                } else {
                    set_log(log, "GREAT!", 200);
                    g_count++;
                }
            } else {
                set_log(log, "MISS!", 200);
                m_count++;
            }
        }

        mvprintw(30, 33, "PERFECT    : %d", p_count);
        mvprintw(31, 33, "GREAT      : %d", g_count);
        mvprintw(32, 33, "MISS       : %d", m_count);
        mvprintw(33, 33, "TOTAL NOTES: %d", all_count);

        show_log(log);
        set_score(score, (MAX_SCORE * p_count + MAX_SCORE * g_count / 2) / all_count);
        set_flag(flag, flag_buf);

        wrefresh(field);
        wrefresh(score);
        wrefresh(flag);
        wrefresh(log);

        usleep(1000);
        loop_cnt++;
    }

    nodelay(stdscr, false);
    usleep(2000);
    getch();


    curs_set(true);
    endwin();
    return 0;
}
