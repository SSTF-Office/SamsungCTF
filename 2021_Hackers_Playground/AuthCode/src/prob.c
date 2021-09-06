#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <openssl/rc4.h>
#include <signal.h>

void printHex(const char* a, unsigned char* b, unsigned int c)
{
	int i;
	printf("%-16s(%02d): ", a, c);
	for(i = 0; i < c; i++)
		printf("%02x", b[i]);
	printf("\n");
	return;
}

void RivestCipher4(unsigned char* data, unsigned char dataLen, unsigned char* key, unsigned char keyLen, unsigned char* result)
{
	RC4_KEY rc4key;

#ifdef __DEBUG__
    printHex("RC4 input", data, dataLen);
    printHex("RC4 key", key, keyLen);
#endif

	RC4_set_key(&rc4key, keyLen, key);
	RC4(&rc4key, dataLen, data, result);
}

#ifdef __DEBUG__
int testRC4(void)
{
	char* data="abcd";
	unsigned char dataLen = 4;
	char* key="abc";
	unsigned char keyLen = 3;
	unsigned int res;
	RivestCipher4(data, dataLen, key, keyLen, &res);
	
	return res == 0x4dbaffac;
}
#endif

void welcome()
{
	puts("\n[Auth code generator]");
	puts("Put your data first.");
	puts("--------------------------");
	puts(" 1. Set team name");
	puts(" 2. Set team size");
	puts(" 3. Issue team code");
	puts(" 4. Exit");
	printf(" > ");
}


struct _ {
	char spc[128 + 32];
	char id[32 - 4];
	int team_size[2];
	char fg;
	void* MasterSecret;
	char AuthKey[32];
	char AuthCode[8];
} name;

void read_file(const char* fn, unsigned char* buf, int len)
{
	int fd = open(fn, O_RDONLY);
	read(fd, buf, len);
	close(fd);
}

long long read_int64()
{
	long long r;
	scanf("%lld", &r);
	getc(stdin);

	return r;
}

void getTeamName(char* id)
{
	printf(" Team Name(max 28 bytes) > ");

	fgets(name.id, 256, stdin);
	name.fg |= 1;

	return;	
}

void getTeamSize(long long* team_size)
{
	long long size;

	printf(" How many members in your team?(max 64) > ");
	size = read_int64();

	if (size > 64)
	{
		puts("Sorry, too many.");
		name.fg &= 1;
	}
	else
	{
		*team_size = size;
		name.fg |= 2;
	}

	return;
}

void issueTeamCode()
{
	struct _ {
		unsigned char buffer[8];
		unsigned char* key;
		long long secret;
		unsigned int tidLen;
	} l;

	l.key = name.AuthKey;

	if ((name.fg & 1) == 0)
	{
		puts("\n[Err] Set the Team Name first.\n");
		return;
	}
	if ((name.fg & 2) == 0)
	{
		puts("\n[Err] Set team size first.\n");
		return;
	}

	l.tidLen = strlen(name.id) > 4 ? 4 : strlen(name.id);
	strncpy(l.buffer, name.id, l.tidLen);
	l.buffer[l.tidLen] = '\0';

	printf(" Put the 4 digit PIN > ");
	l.secret = read_int64();

	if (l.secret >= 10000)
	{
		puts("Only 4 digit secret is necessary.\n");
		return;
	}

	strncat(l.buffer, (char*)&l.secret, sizeof(l.buffer) - l.tidLen);

	RivestCipher4(l.buffer, 8, l.key, *(long long *)&name.team_size + 16, name.AuthCode);

	printHex("Auth Code", name.AuthCode, 8);

	return;
}

void teardown()
{
	void (*sys)(const char*);
	if (memcmp(name.AuthCode, "root", 4) == 0)
	{
		puts("You got the system addr?");
		sys = (void (*)(const char *))read_int64();
		(*sys)("/bin/sh");
	}
	else
	{
		puts("bye");
	}
	return;
}

void mistake()
{
	name.MasterSecret = system;
}

void sig_handler()
{
        printf("Time out.\n");
        exit(0);
}

int main(void)
{
	long long sel = 0;

    signal(SIGALRM, sig_handler);
    alarm(30);

	mistake();

#ifdef __DEBUG__
	printf("system: %08p\n", name.MasterSecret);
	printf("%d\n", testRC4());
	read_file("aaaaaaaa.dat", name.AuthKey, 32);
#else
	read_file("/dev/urandom", name.AuthKey, 32);
#endif

	while(sel != 4)
	{
		welcome();
		sel = read_int64();
		switch(sel)
		{
			case 1:
				getTeamName(name.id);
				break;
			case 2:
				getTeamSize((long long *)&name.team_size);
				break;
			case 3:
				issueTeamCode();
				break;
			case 4:
				teardown();
				break;
			default:
				puts("Wrong input.");
				break;
		}
	}

	return 0;
}
