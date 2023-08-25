#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>

void showFuncAddr(void* hLibc)
{
	char buf[32];
	void* func;
	printf(" > ");
	fgets(buf, sizeof(buf), stdin);
	if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\x00';

	func = dlsym(hLibc, buf);
	if (func)
		printf(" Libc function '%s' is at %p.\n", buf, func);
	else
		printf(" Libc doesn't have function '%s'.\n", buf);

	return;
}

char* pitches = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(void)
{
	struct {
		void* hLibc;
		char buf[8];
		int (*printout)(const char *, ...);
		int hits;
		int runs;
	} l;

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	alarm(10);

	l.printout = printf;

	l.hits = 0;
	l.runs = 0;

	l.hLibc = dlopen("libc.so.6", RTLD_NOW);

	puts("You have only 3 chances to win the game.");

	puts("\nThe 1st chacne: Get libc symbol info.");
	showFuncAddr(l.hLibc);

	puts("\nThe 2nd chacne: Get libc symbol info.");
	showFuncAddr(l.hLibc);

	puts("\nTwo strikes against you. Now that you have only ONE chance.");
	printf("Can you guess the pitcher's selection?\n > ");
	fgets(l.buf, 16, stdin);

	if (l.buf[0] == pitches[rand() % 26])
		l.printout( " You hit the ball! It's an amazing walk-off homerun. Your team won the game.\n");
	else
		l.printout(" Struck out. Game Over - You got %d hits and %d runs.\n", l.hits, l.runs);

	dlclose(l.hLibc);

	exit(0);

	return 0;
}
