#include <unistd.h>
#include <stdio.h>
#include <locale.h>
#include "slot.h"

#define STRIP_LENGTH 64

long score = 0;
int *strips[3];
int *strip_length;
int *strip_weight[3];
int *strip_weight_sum;
int *paytable[7];
int interval = 20;
int frame_per_reel = 200;

extern void init_weight();
extern int get_random();

void set_interval(int _interval) {
	interval = _interval;
}

void reset_cursor() {
	printf("\033[A\033[A\033[A\033[A\033[A\033[A\033[A");
}

void print_score() {
	printf(" SCORE   %'15ld\n", score);
}

void print_horizon_line() {
	printf("+-------+-------+-------+\n");
}

void print_vertical_line(int last) {
	if (last == 1) {
		printf("|\n");
	} else {
		printf("|");
	}
}

void print_symbol(int symbol, int part) {
	switch(symbol) {
		case SYMBOL_SEVEN1:
			printf(F_BLUE);
			break;
		case SYMBOL_SEVEN2:
		case SYMBOL_BONUS2:
			printf(F_WHITE);
			break;
		case SYMBOL_SEVEN3:
			printf(F_RED);
			break;
		case SYMBOL_BONUS1:
			printf(F_YELLOW);
			break;
		case SYMBOL_BLANK:
			printf("       ");
			return;
	}
	switch(part) {
		case 0:
			switch(symbol) {
				case SYMBOL_SEVEN1:
				case SYMBOL_SEVEN2:
				case SYMBOL_SEVEN3:
					printf(" ▕▛▀▜▛ ");
					break;
				case SYMBOL_BONUS1:
				case SYMBOL_BONUS2:
					printf(" ▗▆▆▆▖ ");
					break;
			}
			break;
		case 1:
			switch(symbol) {
				case SYMBOL_SEVEN1:
				case SYMBOL_SEVEN2:
				case SYMBOL_SEVEN3:
					printf("   ▟▛  ");
					break;
				case SYMBOL_BONUS1:
				case SYMBOL_BONUS2:
					printf("  ▝█▘  ");
					break;
			}
			break;
	}
	printf(F_WHITE);
}

void print_all(float offsetfs[3], int expectation, int first) {
	int offsets[3];
	offsets[0] = (int)(offsetfs[0] + 0.1);
	offsets[1] = (int)(offsetfs[1] + 0.1);
	offsets[2] = (int)(offsetfs[2] + 0.1);
	if (first) reset_cursor();
	print_score();
	print_horizon_line();

	print_vertical_line(0);
	if (expectation == 0) printf(B_GREEN);
	print_symbol(strips[0][offsets[0] / 2 % strip_length[0]], offsets[0] % 2);
	printf(B_BLACK);
	print_vertical_line(0);
	if (expectation == 1) printf(B_GREEN);
	print_symbol(strips[1][offsets[1] / 2 % strip_length[1]], offsets[1] % 2);
	printf(B_BLACK);
	print_vertical_line(0);
	if (expectation == 2) printf(B_GREEN);
	print_symbol(strips[2][offsets[2] / 2 % strip_length[2]], offsets[2] % 2);
	printf(B_BLACK);
	print_vertical_line(1);

	print_vertical_line(0);
	printf(B_CYAN);
	if (expectation == 0) printf(B_GREEN);
	print_symbol(strips[0][(offsets[0] + 1) / 2 % strip_length[0]], (offsets[0] + 1) % 2);
	printf(B_CYAN);
	print_vertical_line(0);
	if (expectation == 1) printf(B_GREEN);
	print_symbol(strips[1][(offsets[1] + 1) / 2 % strip_length[1]], (offsets[1] + 1) % 2);
	printf(B_CYAN);
	print_vertical_line(0);
	if (expectation == 2) printf(B_GREEN);
	print_symbol(strips[2][(offsets[2] + 1) / 2 % strip_length[2]], (offsets[2] + 1) % 2);
	printf(B_BLACK);
	print_vertical_line(0);
	printf("LINE\n");

	print_vertical_line(0);
	printf(B_CYAN);
	if (expectation == 0) printf(B_GREEN);
	print_symbol(strips[0][(offsets[0] + 2) / 2 % strip_length[0]], (offsets[0] + 2) % 2);
	printf(B_CYAN);
	print_vertical_line(0);
	if (expectation == 1) printf(B_GREEN);
	print_symbol(strips[1][(offsets[1] + 2) / 2 % strip_length[1]], (offsets[1] + 2) % 2);
	printf(B_CYAN);
	print_vertical_line(0);
	if (expectation == 2) printf(B_GREEN);
	print_symbol(strips[2][(offsets[2] + 2) / 2 % strip_length[2]], (offsets[2] + 2) % 2);
	printf(B_BLACK);
	print_vertical_line(0);
	printf("LINE\n");

	print_vertical_line(0);
	if (expectation == 0) printf(B_GREEN);
	print_symbol(strips[0][(offsets[0] + 3) / 2 % strip_length[0]], (offsets[0] + 3) % 2);
	printf(B_BLACK);
	print_vertical_line(0);
	if (expectation == 1) printf(B_GREEN);
	print_symbol(strips[1][(offsets[1] + 3) / 2 % strip_length[1]], (offsets[1] + 3) % 2);
	printf(B_BLACK);
	print_vertical_line(0);
	if (expectation == 2) printf(B_GREEN);
	print_symbol(strips[2][(offsets[2] + 3) / 2 % strip_length[2]], (offsets[2] + 3) % 2);
	printf(B_BLACK);
	print_vertical_line(1);

	print_horizon_line(0);
}

void spin(int targets[3]) {
	float offsets[3];
	float speeds[3] = {0.70, 0.60, 0.50};

	offsets[0] = -1 + strip_length[0] * 2;
	offsets[1] = -1 + strip_length[1] * 2;
	offsets[2] = -1 + strip_length[2] * 2;

	print_all(offsets, -1, 0);
	usleep(interval * 1000);

	for(int i = 0; i < 30; i++) {
		offsets[0] -= speeds[0];
		offsets[1] -= speeds[1];
		offsets[2] -= speeds[2];
		if (offsets[0] < 0) offsets[0] += strip_length[0] * 2;
		if (offsets[1] < 0) offsets[1] += strip_length[1] * 2;
		if (offsets[2] < 0) offsets[2] += strip_length[2] * 2;
		print_all(offsets, -1, 1);
		usleep(interval * 1000);
	}

	// speed is changed to stop at targets
	speeds[0] = (targets[0] - offsets[0]) / (-frame_per_reel);
	while (speeds[0] < 1 - strip_length[0] * 2. / frame_per_reel) speeds[0] += strip_length[0] * 2. / frame_per_reel;
	for(int i = 0; i < frame_per_reel; i++) {
		offsets[0] -= speeds[0];
		offsets[1] -= speeds[1];
		offsets[2] -= speeds[2];
		if (offsets[0] < 0) offsets[0] += strip_length[0] * 2;
		if (offsets[1] < 0) offsets[1] += strip_length[1] * 2;
		if (offsets[2] < 0) offsets[2] += strip_length[2] * 2;
		print_all(offsets, -1, 1);
		usleep(interval * 1000);
	}

	speeds[1] = (targets[1] - offsets[1]) / (-frame_per_reel);
	while (speeds[1] < 1 - strip_length[1] * 2. / frame_per_reel) speeds[1] += strip_length[1] * 2. / frame_per_reel;
	for(int i = 0; i < frame_per_reel; i++) {
		offsets[1] -= speeds[1];
		offsets[2] -= speeds[2];
		if (offsets[1] < 0) offsets[1] += strip_length[1] * 2;
		if (offsets[2] < 0) offsets[2] += strip_length[2] * 2;
		print_all(offsets, -1, 1);
		usleep(interval * 1000);
	}

	speeds[2] = (targets[2] - offsets[2]) / (-frame_per_reel);
	while (speeds[2] < 1 - strip_length[2] * 2. / frame_per_reel) speeds[2] += strip_length[2] * 2. / frame_per_reel;
	for(int i = 0; i < frame_per_reel; i++) {
		offsets[2] -= speeds[2];
		if (offsets[2] < 0) offsets[2] += strip_length[2] * 2;
		print_all(offsets, -1, 1);
		usleep(interval * 1000);
	}
}

void add_score(long delta) {
	score += delta;
}

void init(long _score, int _strips[][STRIP_LENGTH], int _strip_length[], int _strip_weight[][STRIP_LENGTH], int _strip_weight_sum[], int _paytable[7][4]) {
	init_weight();
	setlocale(LC_NUMERIC, "en_US.UTF-8");
	strips[0] = _strips[0];
	strips[1] = _strips[1];
	strips[2] = _strips[2];
	strip_length = _strip_length;
	strip_weight[0] = _strip_weight[0];
	strip_weight[1] = _strip_weight[1];
	strip_weight[2] = _strip_weight[2];
	strip_weight_sum = _strip_weight_sum;
	paytable[0] = _paytable[0];
	paytable[1] = _paytable[1];
	paytable[2] = _paytable[2];
	paytable[3] = _paytable[3];
	paytable[4] = _paytable[4];
	paytable[5] = _paytable[5];
	paytable[6] = _paytable[6];
	score = _score;
}

void determine(struct GAME *game) {
	int symbol_count[7] = {0};
	int reel_index[3];
	int sum;
	int symbol;
	int result;
	int result_code = 0;
	int random = get_random();
	reel_index[0] = random & 0b111111;
	random >>= 6;
	reel_index[1] = random & 0b111111;
	random >>= 6;
	reel_index[2] = random & 0b111111;

	sum = 0;
	for(int i = 0; i < strip_length[0]; i++) {
		sum += strip_weight[0][i];
		if (reel_index[0] < sum) {
			reel_index[0] = i;
			break;
		}
	}

	sum = 0;
	for(int i = 0; i < strip_length[1]; i++) {
		sum += strip_weight[1][i];
		if (reel_index[1] < sum) {
			reel_index[1] = i;
			break;
		}
	}

	sum = 0;
	for(int i = 0; i < strip_length[2]; i++) {
		sum += strip_weight[2][i];
		if (reel_index[2] < sum) {
			reel_index[2] = i;
			break;
		}
	}

	for(int i = 0; i < 3; i++) {
		symbol = strips[i][reel_index[i]];
		if (symbol <= SYMBOL_SEVEN3) {
			symbol_count[SYMBOL_SEVEN_ANY]++;
		}
		symbol_count[symbol]++;
	}

	result = 0;
	for(int i = 0; i < 7; i++) {
		game->symbol_count[i] = symbol_count[i];
		if (result < paytable[i][symbol_count[i]]) {
			result = paytable[i][symbol_count[i]];
			result_code = i * 4 + symbol_count[i];
		}
	}

	game->result_code = result_code;
	game->result = result;
	game->targets[0] = (reel_index[0] * 2 - 1 + strip_length[0] * 2) % (strip_length[0] * 2);
	game->targets[1] = (reel_index[1] * 2 - 1 + strip_length[1] * 2) % (strip_length[1] * 2);
	game->targets[2] = (reel_index[2] * 2 - 1 + strip_length[2] * 2) % (strip_length[2] * 2);
}

