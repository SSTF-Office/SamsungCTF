#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <openssl/md5.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

char * password = NULL;

typedef struct struct_v2 NODE;
struct struct_v2
{
	char Hash[0x90];
	char * (*Func)( char * v1);
	char Msg[0x100];
	char * cmt;
	unsigned int no;
	NODE * next;
};

NODE * head;
void Setpassword();
