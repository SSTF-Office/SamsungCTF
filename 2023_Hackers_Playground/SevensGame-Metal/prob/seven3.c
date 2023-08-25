#include <stdio.h>
#include "slot.h"
#include "seven3.h"

const int FLAG_PRICE = 10000 * 1000 * 100; // Should be larget than MAX_BET * MAXIMUM_SPIN * MAX_PAYTABLE * 10
int MAXIMUM_SPIN;
int MAXIMUM_SEED_CHANGE;
int USE_JACKPOT;

int TIMEOUT = 10;
int bets[] = {100, 1000, 10000};
int bet;
long money;
int animation;
int debug;
int total_spin;
int total_seed;
int spinperplay = 1;

extern void init(long _score, int _strips[][64], int _strip_length[], int _strip_weight[][64], int _strip_weight_sum[], int _paytable[7][4]);
extern void spin(int targets[3]);
extern void add_score(long delta);
extern void print_all(float offsetfs[3], int expectation, int first);
extern void determine(struct GAME *game);
extern void reset_cursor();
extern int get_urandom(int mod);
extern void set_seed(int seed);
extern void set_interval(int _interval);

void init_reel_strips(int _strips[][64]) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 32; j++) {
			_strips[i][2 * j] = get_urandom(3);
			_strips[i][2 * j + 1] = SYMBOL_BLANK;
		}
	}
	if (USE_JACKPOT) {
		for(int i = 0; i < 3; i++) {
			int j = get_urandom(32);
			_strips[i][2 * j] = SYMBOL_BONUS1;
		}
	}
}

void play() {
	struct GAME game;
	float targets[3] = {0, 0, 0};
	if (bet > money) {
		printf("Your score is too low to play. Change your difficulty.\n");
		return;
	}
	if (total_spin >= MAXIMUM_SPIN) {
		printf("You have played too much!\n");
		return;
	}
	total_spin++;
	game.bet = bet;
	add_score(-bet);

	money -= game.bet;
	determine(&game);
	money += (long)game.bet * (long)game.result;

	if (animation == 1) {
		spin(game.targets);
	}
	add_score((long)bet * (long)game.result); // instead result
	targets[0] = game.targets[0];
	targets[1] = game.targets[1];
	targets[2] = game.targets[2];
	print_all(targets, -1, animation == 1);
	if (debug) {
		printf("bet : %d, result : %d, result_code : %d, targets : %d, %d, %d, symbol_count : %d, %d, %d, %d, %d, %d, %d\n", game.bet, game.result, game.result_code, game.targets[0], game.targets[1], game.targets[2], game.symbol_count[0], game.symbol_count[1], game.symbol_count[2], game.symbol_count[3], game.symbol_count[4], game.symbol_count[5], game.symbol_count[6]);
	}
	if (game.result_code % 4 == 3) {
		printf("Win 3 of ");
		switch(game.result_code / 4) {
			case SYMBOL_SEVEN1:
				printf("Blue");
				break;
			case SYMBOL_SEVEN2:
				printf("White");
				break;
			case SYMBOL_SEVEN3:
				printf("Red");
				break;
			case SYMBOL_SEVEN_ANY:
				printf("Mixed");
				break;
		}
		printf(" Sevens - Earn %d times of the difficulty!\n", game.result);
	} else if (game.symbol_count[SYMBOL_BONUS1] == 3) {
		printf("Win the Jackpot!\n");
	}
}

void shop() {
	int menu;
	printf("\n");
	while(1) {
		printf("You can change your score into item.\n");
		printf("Your SCORE : %'15ld\n", money);
		printf("1. Flag : %'15d SCORE\n", FLAG_PRICE);
		printf("Select the item you want to buy, or input any other key to go back into the main menu: \n");

		scanf("%d", &menu);
		while((getchar())!='\n');
		switch(menu) {
			case 1:
				if (money >= FLAG_PRICE) {
					money -= FLAG_PRICE;
					system("cat /flag.txt");
				} else {
					printf("Not enough score.\n");
				}
				break;
			default:
				// Go to main
				return;
				break;
		}
	}
}

void settings() {
	int menu;
	int seed;
	int frame_interval;
	int spin_count_per_play;
	printf("\n");
	while(1) {
		printf("Change settings\n");
		printf("1. Turn on animation during playing\n");
		printf("2. Turn off animation during playing\n");
		printf("3. Please set timeout as 30 seconds\n");
		printf("4. Set random seed\n");
		printf("5. Turn on debug mode\n");
		printf("6. Turn off debug mode\n");
		printf("7. Set spin count per play\n");
		printf("9. Set spin animation frame interval\n");
		printf("0. Back to the main menu\n");
		printf("Select the settings menu you want to change, or input any other key to go back into the main menu: \n");

		scanf("%d", &menu);
		while((getchar())!='\n');
		switch(menu) {
			case 1:
				animation = 1;
				break;
			case 2:
				animation = 0;
				break;
			case 3:
				TIMEOUT = 30;
				break;
			case 4:
				if (total_seed >= MAXIMUM_SEED_CHANGE) {
					printf("You have changed seed too much!\n");
					break;
				}
				printf("You changed the seed %d time(s). What is your lucky seed?\n", total_seed);
				scanf("%d", &seed);
				while((getchar())!='\n');
				set_seed(seed);
				total_seed++;
				break;
			case 5:
				debug = 1;
				break;
			case 6:
				debug = 0;
				break;
			case 7:
			case 8:
				printf("Input how many times you spin per each play (default : 1)\n");
				scanf("%d", &spin_count_per_play);
				while((getchar())!='\n');
				if (spin_count_per_play < 0 || spin_count_per_play > 1000) {
					printf("Wrong\n");
					return;
				}
				spinperplay = spin_count_per_play;
				break;
			case 9:
				printf("Input the spinning animation frame interval (default : 20)\n");
				scanf("%d", &frame_interval);
				while((getchar())!='\n');
				if (frame_interval < 0 || frame_interval > 1000) {
					printf("Wrong\n");
					return;
				}
				set_interval(frame_interval);
				break;
			default:
				// Go to main
				return;
				break;
		}
	}
}

void help() {
	printf("\n");
	printf("How to play Seven's Game Meta\n");
	printf("Once you play, you get the combination of three symbols on the LINE.\n");
	if (USE_JACKPOT) {
		printf("Each symbol can be SEVEN, BLANK or DIAMOND symbol.\n");
	} else {
		printf("Each symbol can be SEVEN or BLANK symbol.\n");
	}
	printf("If you get three of SEVEN symbols, you get reward by the following score table.\n");
	printf("\n");
	printf("SCORE\n");
	if (USE_JACKPOT) {
		printf(" - Three DIAMONDs gives you 100000 times of the difficulty score.\n");
	}
	printf(" - Three Blue SEVENs gives you 100 times of the difficulty score.\n");
	printf(" - Three White SEVENs gives you 10 times of the difficulty score.\n");
	printf(" - Three Red SEVENs gives you 5 times of the difficulty score.\n");
	printf(" - Three Mixed SEVENs gives you 2 times of the difficulty score.\n");
	printf("\n");
}

void changebet() {
	int bet_input;
	printf("\n");
	while(1) {
		printf("Select the difficulty\n");
		printf("[ %d | %d | %d ]\n", bets[0], bets[1], bets[2]);
		if (scanf("%d", &bet_input) == 1) {
			if (bet_input > money) {
				printf("Your score is too low to choose that difficulty.\n");
				continue;
			} else {
				int wrongflag = bet_input != bets[0] && bet_input != bets[1] && bet_input != bets[2];
				if (wrongflag == 1) {
					printf("Wrong difficulty.\n");
					continue;
				} else {
					bet = bet_input;
					printf("Difficulty is changed.\n");
					break;
				}
			}
		} else {
			while((getchar())!='\n');
		}
	}
}

int main(int argc, char **argv) {
	int menu;
	MAXIMUM_SPIN = atoi(argv[1]);
	MAXIMUM_SEED_CHANGE = atoi(argv[2]);
	USE_JACKPOT = atoi(argv[3]);
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	animation = 1;
	money = 100000;
	bet = bets[0];
	total_spin = 0;
	total_seed = 0;
	init_reel_strips(STRIPS);
	init(money, STRIPS, STRIP_LENGTH, STRIP_WEIGHT, STRIP_WEIGHT_SUM, PAYTABLE);
	while(1) {
		alarm(TIMEOUT);
		printf("\n");
		printf("Welcome to Seven's Game Meta!\n");
		printf("1. Play\n");
		printf("2. Change the difficulty\n");
		printf("3. Shop\n");
		printf("4. How to play\n");
		printf("5. Settings\n");
		printf("6. Exit\n");
		printf("Select the menu: \n");

		scanf("%d", &menu);
		while((getchar())!='\n');
		switch(menu) {
			case 1:
				for(int i = 0; i < spinperplay; i++) {
					play();
				}
				break;
			case 2:
				changebet();
				break;
			case 3:
				shop();
				break;
			case 4:
				help();
				break;
			case 5:
				settings();
				break;
			case 6:
				return 0;
		}
	}
	return 0;
}

