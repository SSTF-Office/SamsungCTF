#include "heap.h"
#include "md5.h"

#define COMMENT_MAX 500
#define MSG_MAX     0x100
#define HASH_MAX    0x90
void init_buf()
{
	setvbuf(stdout,0,_IONBF,0);
	setvbuf(stdin,0,_IONBF,0);
	tcflush(0, TCIFLUSH);
	tcflush(1, TCIFLUSH);
}
void initNode(NODE * v1)
{
	memset(v1->Hash,0,HASH_MAX);
	memset(v1->Msg ,0,MSG_MAX);
	v1->Func    = HashFunction;
	v1->no      = 0;
	v1->cmt     = NULL;
	v1->next    = NULL;
}
void FREE(void *v1)
{
	if(v1 !=NULL)
	{
		free(v1);
		v1 = NULL;
	}
}
void WELCOME()
{
	printf("[!] WELCOME. System is Ubuntu 16.04 \n");
	printf("[!] 1 : Create NODE with MSG \n");
	printf("[!] 2 : Delete NODE\n");
	printf("[!] 3 : Write Comment in NODE\n");
	printf("[!] 4 : Delete Comment in NODE\n");
	printf("[!] 5 : Update Comment in NODE\n");
	printf("[!] 6 : Make hash value with MSG\n");
	printf("[!] 7 : Show All Data \n");
	printf("[!] 8 : SET PASSWORD \n");
	printf("[!] alarm set 10s. good luck\n");
	alarm(10);
}
void SET_COMMENT(NODE *v1 , unsigned int v2)
{
	printf("\n[+] SET COMMENT> ");
	if(v1 != NULL)
	{
		if(v1->cmt == NULL && v2 >= 0x30 && v2 < COMMENT_MAX)
		{
			v1->cmt = (char *)malloc(v2);
			if(v1->cmt != NULL)
			{
				memset(v1->cmt,0,v2);
				read(0,v1->cmt,v2-1);
			}
		}
		else
		{
			printf("[!] You already set comment\n");
		}
	}
	else
	{
		printf("NODE is NULL\n");
	}
}
void UNSET_COMMENT(NODE *v1)
{
	printf("[-] UNSET COMMENT\n");
	if( v1 != NULL )
	{
		if( v1->cmt != NULL )
		{
			memset(v1->cmt,0,strlen(v1->cmt));
			FREE(v1->cmt);
			v1->cmt=NULL;
		}
		else
		{
			printf("[!] COMMENT is NULL \n");
		}
	}
	else
	{
		printf("[!] NODE is NULL \n");
	}
}
void CHANGE_COMMENT(NODE *v1)
{
	int len = 0;
	char buf[MSG_MAX+1];
	if(v1 != NULL)
	{
		len = strlen(v1->cmt);
		if( (v1->cmt != NULL) && (len > 0) && (len < MSG_MAX) )
		{
			printf("\n[+]CHANGE COMMENT> ");
			memset(v1->cmt,0,len);
			memset(buf,0,MSG_MAX+1);
			read(0,buf,MSG_MAX);
			buf[len]='\0';
			memcpy(v1->cmt,buf,len);
			//read(0,v1->cmt,len);
		}
		else
		{
			printf("[!] COMMENT is NULL \n");
		}
	}
	else
	{
		printf("[!] Node is NULL\n");
	}
}
void SET_PASSWORD()
{
	if(password == NULL)
	{
		printf("\n[+] Input password (length : 6~55)\n");
		password = ( char *) malloc(56); // 56
		if(password != NULL)
		{
			memset(password,0,56);
			scanf("%56s",password);
			printf("PW: %s\n",password);
		}
	}
}
void CREATE_NODE()
{
	NODE * A1 = (NODE *) malloc(sizeof(NODE));
	if(A1 != NULL)
	{
		initNode(A1);
		printf("\nNOTE : ");
		read(0, A1->Msg, MSG_MAX-1);
		if(head->next == NULL)
		{
			head->next  = A1;
			A1->no      = 1;
		}
		else
		{
			A1->no      = head->next->no +1;
			A1->next    = head->next;
			head->next  = A1;
		}
		if(A1->no > 10)exit(0);
	}
}
void DELETE_NODE( NODE *v1 )
{
	if(v1 != NULL)
	{
		printf("[-] DELETE NODE : %d \n",v1->no);
		NODE * tmp = NULL;
		for(tmp=head; tmp!=NULL; tmp=tmp->next)
		{
			if(tmp->next == v1)
			{
				tmp->next = v1->next;
				UNSET_COMMENT(v1);
				memset(v1->Hash,0,HASH_MAX);
				memset(v1->Msg,0,MSG_MAX);
				v1->next = NULL;
				v1->Func = NULL;
				FREE(v1);
				v1=NULL;
				break;
			}
		}
		tmp=NULL;
	}
	else
	{
		printf("[!] NODE is NULL\n");
	}
}
void MAKE_HASH(NODE * node1)
{
	if(node1 != NULL && strlen(node1->Msg)>0)
	{
			if(password != NULL && strstr(password,"ToE_heap") !=NULL)
			{
				char * v1 = node1->Func(node1->Msg); //0x30
				if( v1 != NULL ,strlen(v1) < HASH_MAX )
				{
					memcpy(node1->Hash,v1,HASH_MAX);
					printf("HASH : [%s]",node1->Hash);
					memset(v1,0,strlen(v1));
					FREE(v1);
					v1=NULL;
				}
			}
			else
			{
				printf("[-] Wrong password\n");
			}
	}
	else
	{
		printf("[!] NODE is NULL\n");
	}
}
void SHOW_ALL()
{
	NODE * tmp = NULL;
	if(head->next !=NULL)
	{
		for(tmp=head->next; tmp!=NULL; tmp= tmp->next)
		{
			printf("-----------------------------------\n");
			printf("[!] NODE NUM : [%d]\n"  ,tmp->no);
			printf("[!] NODE HASH : %s\n" ,tmp->Hash);
			printf("[!] NODE MSG: %s"   ,tmp->Msg);
			printf("[!] NODE COMMENT: %s",tmp->cmt);
			printf("\n");
		}
	}
}
NODE * FIND_NODE(unsigned int no)
{
	NODE * tmp = NULL;
	for(tmp=head->next; tmp!=NULL; tmp= tmp->next)
	{
		if(tmp->no == no)return tmp;
	}
	return NULL;
}
int BANNER()
{
	system("cat ./banner.txt");
	return 0;
}

int main( int argc, char *argv[] )
{
	BANNER();
	WELCOME();
	init_buf();
	head = (NODE *)malloc(sizeof(NODE));
	initNode(head);
	unsigned int cnt        = 0;
	unsigned int node_num   = 0;
	unsigned int case_num   = 0;
	node_num = 0x0000000000000000;
#ifndef DEBUG
	while(cnt < 30)
	{
		printf("MENU> ");
		node_num    = 0;
		case_num	= 0;
		init_buf();
		unsigned int case_num = 0;
		scanf("%d",&case_num);
		if( case_num < 9 && case_num > 0 )
		{
			printf("select : %d\n",case_num);
			switch (case_num)
			{
				case 1:
				{
					CREATE_NODE();
					break;
				}
				case 2:
				{
					printf("[-] DELETE NODE NUMBER> ");
					scanf("%d",&node_num);
					DELETE_NODE(FIND_NODE(node_num));
					break;
				}
				case 3:
				{
					printf("[+] NODE NUMBER> ");
					scanf("%d",&node_num);
					printf("[+] COMMENT SIZE> ");
					unsigned int comment_size   = 0;
					scanf("%d",&comment_size);
					SET_COMMENT(FIND_NODE(node_num),comment_size);
					comment_size    = 0;
					break;
				}
				case 4:
				{
					printf("[-] DELETE COMMENT\n");
					printf("[-] NODE NUMBER> ");
					scanf("%d",&node_num);
					UNSET_COMMENT(FIND_NODE(node_num));
					break;
				}
				case 5:
				{
					printf("[+] CHANGE NODE NUMBER> ");
					scanf("%d",&node_num);
					printf("node:%d",node_num);
					CHANGE_COMMENT(FIND_NODE(node_num));
					break;
				}
				case 6:
				{
					printf("[+] MAKE MSG HASH\n");
					printf("[+] NODE NUM> ");
					scanf("%d",&node_num);
					MAKE_HASH(FIND_NODE(node_num));
					break;
				}
				case 7:
				{
					SHOW_ALL();
					break;
				}
				case 8:
				{
					SET_PASSWORD();
					break;
				}
				default :
				{
					WELCOME();
					break;
				}
			}
			printf("\n");
			cnt++;
		}
	}
#endif
	return 0;

}
