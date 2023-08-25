#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define NUM_THREADS 2

#define MAX_LIST 32

typedef struct NODE 
{
	char str[16];
	unsigned int inused;
}NODE;

NODE * node[MAX_LIST];

void init()
{	for( int i =0; i< MAX_LIST;i++)
	{
		node[i] = NULL;
	}
}
void add(unsigned int pos, char a1[], unsigned int a1_size )
{
	if(pos < MAX_LIST) // Use after free. cuz not exist inused- null checker 
	{
		if(node[pos] == NULL)
		{
			node[pos] = (NODE * )malloc(sizeof(NODE));
		}
		node[pos]->inused = 1;
		memcpy(node[pos]->str , a1, a1_size );
	}
	#ifdef DEBUG
	printf("add: pos->%d, a1->[%s]\n",pos, a1);
	#endif
}
void del(unsigned int pos)
{
	if(node[pos]!=NULL)
	{
		if(node[pos]->inused==1) 
		{
			node[pos]->inused=0;
			free(node[pos]);
		}
	}
}
void print()
{
	for(int i =0; i<MAX_LIST;i++)
	{
		if(node[i]!= NULL)
		{
			if(strlen(node[i]->str) > 5)
				printf("->%s\n",node[i]->str); // safe_link leak via free chunk 
		}
	}
	printf("----\n");
}
int validation()
{
	char key[]="92eab7870b69dfb99c62db3ca075b222be8822a861bbfbbbc94f4b536682fe52"; // sha256 ('heap_safe_link')a
	#ifdef DEBUG
	printf("key->[%s]\n",key);
	#endif 
	for(int i = 0; i < MAX_LIST; i++)
	{
		if(node[i] != NULL)
		{
			if(node[i]->inused == 1)
			{
				int pos = 0;
				while(node[i]->str[pos]!='\0')
				{
					if(key[i*8+pos] != node[i]->str[pos])
					{
						return 0;
					}
					pos++;
				}
			}
		}
	}
	return 1;
}
int main() 
{
	char * welcome = NULL;
	unsigned int pos = 0;
	int test_case = 0;
	char buf[24];
	int cmd = 0;
	int ret = 0;
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	init();
	while(test_case < 50000)
	{
		printf("cmd: ");
		scanf("%d",&cmd);
		switch(cmd)
		{
			case 1:
				scanf("%d",&pos);
				ret = read(0,buf,16);
				buf[ret-1]='\0'; // remove new line 
				add(pos, buf, ret);
				break;
			case 2:
				scanf("%d",&pos);
				del(pos);
				break;
			case 3:
				print();
				break;
			case 4:
				ret = validation();
				if(ret) printf("success.\n");
				else printf("fail.\n");
				break;
			default:
				return 0;
		}
		fflush(stdin);
		cmd =0;
		test_case++;
	}
	return 0;
}

