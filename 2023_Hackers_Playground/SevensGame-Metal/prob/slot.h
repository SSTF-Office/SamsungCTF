/*
 * SEVEN1 = blue
 * SEVEN2 = white
 * SEVEN3 = red
 */

#define SYMBOL_SEVEN1 0
#define SYMBOL_SEVEN2 1
#define SYMBOL_SEVEN3 2
#define SYMBOL_BONUS1 3
#define SYMBOL_BONUS2 4
#define SYMBOL_BLANK 5
#define SYMBOL_SEVEN_ANY 6

#define PART_UPPER 0
#define PART_LOWER 1

#define F_BLUE "\033[34m"
#define F_WHITE "\033[37m"
#define F_RED "\033[31m"
#define F_YELLOW "\033[33m"

#define B_CYAN "\033[46m"
#define B_GREEN "\033[42m"
#define B_BLACK "\033[40m"

struct GAME {
	int bet;
	int result;
	int result_code;
	int targets[3];
	int symbol_count[7];
};

