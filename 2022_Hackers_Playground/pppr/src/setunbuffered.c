#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	setbuf(stdin, 0);
	setbuf(stdout, 0);
	setbuf(stderr, 0);

	return system(argv[1]);
}
