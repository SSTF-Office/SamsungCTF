#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rsa.h"

char* cmds[] = {
	"ls -la /",
	"pwd -P",
	"cat /etc/os-release",
	"cat /etc/lsb-release",
	"ls -l /lib/x86_64-linux-gnu/libgmp*",
	"cat /flag"
};

char *heap;

int menu()
{
	int i;
	puts("\n [[ RSA Command Signer 2 ]]");
	puts("=============================");
	puts(" 1. Select Command");
	puts(" 2. Show parameters");
	puts(" 3. Generate Signature");
	puts(" 4. Execute signed command");
	puts(" 9. Exit");
	puts("=============================");
	printf(" > ");
	scanf("%d%*c", &i);

	return i;
}

int getCmd(int cmd)
{
	int i, r;

	do {
		puts("\n Possible Commands:");

		for (i = 0; i < 6; i++)
		{
			printf(" %d: %s\n", i, cmds[i]);
		}
		printf("Select command(current is %d) > ", cmd);
		scanf("%d%*c", &r);
	} while(r < 0 || r > 5);

	return r;
}

void verifyAndRun(RSA *rsa)
{
	char cmd[65];
	char sign[641];

	printf("Command to execute > ");
	fgets(cmd, 65, stdin);
	cmd[strcspn(cmd, "\n")] = 0;

	printf("Signature of the command, in decimal > ");
	fgets(sign, 641, stdin);
	cmd[strcspn(cmd, "\n")] = 0;

	if (verifySign(cmd, sign, rsa))
	{
		system(cmd);
	}
	else
	{
		puts("Wrong signature.");
	}
}

void onechance(void)
{
	static int usedFlag;
	char* target = heap;
	int idx;
	char buf[5];	//%9$n\0

	if (usedFlag != 0) return;

	usedFlag = 1;

	scanf("%d%*c", &idx);
	target += idx;

	fgets(buf, sizeof(buf), stdin);
	if (!strchr(buf, 's')) printf(buf);
}

int main(void)
{
	int i, cmd = 0, signs = 0;
	RSA rsa;

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	initRSA(&rsa);
	heap = (char*)malloc(256);

	generateKey(&rsa);

#ifdef _DEBUG_
	printf("rsa.n addr: %p\n", rsa.n->_mp_d);
	printf("heap addr:  %p\n", heap);
	printf("%lld\n", (long long)rsa.n->_mp_d - (long long)heap);
	printf("rsa.q addr: %p\n", rsa.q->_mp_d);
	printf("heap addr:  %p\n", heap);
	printf("%lld\n", (long long)rsa.q->_mp_d - (long long)heap);
#endif

	do
	{
		i = menu();

		switch(i)
		{
			case 0:
				describeRSAKeyGen(&rsa);
				break;
			case 1:
				cmd = getCmd(cmd);
				break;
			case 2:
				printRSA(&rsa);
				break;
			case 3:
				if (signs >= 12)
					puts("Too many signature generations!");
				else
					generateSign(cmds[cmd], &rsa);
					signs++;
				break;
			case 4:
				verifyAndRun(&rsa);
				break;
			case 9999:
				onechance();
				break;
			default:
				i = 9;
				break;
		}
	} while (i != 9);

	deleteRSA(&rsa);

	return 0;
}
