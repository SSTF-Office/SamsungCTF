#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN 100
char USER_DB[MAX_LEN]="./user.db";
char NOTE_DB[MAX_LEN]="./note.db";

char currentUser[MAX_LEN]="";
typedef struct node NODE;
struct node
{
	int idex;
	char USER[MAX_LEN];
	char LINK[MAX_LEN];
	NODE * prev;
	NODE * next;
};
NODE * head = NULL;
void initbuf()
{
	setbuf(stdin,NULL);
	setbuf(stdout,NULL);
	setbuf(stderr,NULL);
}
void init()
{
	alarm(10);
	initbuf();
	//unlink(USER_DB);
	//unlink(NOTE_DB);
}
void usage()
{
	printf("------------------------------\n");
	printf("1. join \n");
	printf("2. login \n");
	printf("3. print notes\n");
	printf("4. save note\n");
	printf("5. exit\n");
	printf("------------------------------\n");

}
void printNote(char * _user)
{
	char user[MAX_LEN]="";
	char link[MAX_LEN]="";
	FILE * fp = fopen(NOTE_DB,"r");
	if(fp == NULL) return;
	while(fp != NULL)
	{
		if(fscanf(fp,"%100s %100s",user,link)<1) break;
		else 
		{
		
			write(1,user,strlen(user));
			write(1,"->",3);
			write(1,link,strlen(link));	
		}
	}
	printf("\n");
	
}

void saveNote( char * _link, char * cref)
{
	char buff[MAX_LEN]="";
	if( strlen(currentUser) > 0 && strlen(_link)>0 )
	{
		if(filter(_link))
		{
			printf("[-] Wrong format.\n");
			return;
		}
		else
		{
			sprintf(buff, "%s %s", currentUser, _link);
			memcpy(cref,buff,MAX_LEN);
			printNote(currentUser);
			return;
		}
	}
	else printf("[!] login please\n");
	return;
}
void join(char *_user , char *_passwd)
{
	FILE * fp = fopen(USER_DB,"r+");
	if(fp==NULL)
	{
		printf("[D] file open error.\n");
		return;
	}
	else
	{
		if(strlen(_user)<=0 || strlen(_passwd) <=0) 
		{
			printf("[E] Check your id, pw\n");
			return;
		}
		else 
		{
			fprintf(fp,"%s %s\n", _user, _passwd);
		}
		fclose(fp);
	}
}
int login(char * _user, char *_passwd)
{
	char user[100]="";
	char pass[100]="";

	if( strlen(currentUser) <=0 && strlen(_user) > 0 && strlen(_passwd)>0 )
	{
		FILE * fp = fopen(USER_DB,"r+");
		if(fp == NULL) 
		{
			printf("[D] oops!? file open error");
			return 0;
		}
		else
		{
			while (fp != NULL)
			{
				if(fscanf(fp,"%100s %100s",user, pass)<1)break;
				if(strcmp(_user,user)==0 && strcmp(_passwd,pass)==0)
				{
					snprintf(currentUser,MAX_LEN,"%s",_user);
					printf("[+] login success\n");
					return 1;
					}
			}
			fclose(fp);
		}
	}
	else
	{
		printf("[E] Login error \n");
	}
	return 0;
}
int filter(char * _link)
{
	
	if(strstr(_link,"note://")==NULL) return 1;	
	else if(strlen(_link)>100) return 1;
	else return 0;
}
int main(int argc, char *argv[])
{
	char _user[MAX_LEN]="";
	char _pass[MAX_LEN]="";
	char link[MAX_LEN]="";
	char tmp[MAX_LEN]="";
	int _case=0;
	FILE * fp = NULL;
	init();
	while(1)
	{
		initbuf();
		usage();
		printf(">>");
		scanf("%d",&_case);
		printf("%d okay... \n",_case);
		switch(_case)
		{	
			case 1:
				printf("[+] Sign up menu\n");
				printf("[+] User: ");
				scanf("%100s",_user);
				printf("[+] Password: ");
				scanf("%100s",_pass);
				if(strstr(_user," ")==NULL && strstr(_pass," ")==NULL) join(_user, _pass);
				else printf("[-] Space string is not allowed\n");
				break;
			case 2:
				printf("[+] Login menu\n");
				printf("[+] User: ");
				scanf("%100s",_user);
				printf("[+] Pass: ");
				scanf("%100s",_pass);
				if(strstr(_user," ")==NULL && strstr(_pass," ")==NULL) 
				{
					login(_user, _pass);
				}
				else printf("[!] Space is not allow.\n");
				break;
			case 3:	
				if (strlen(currentUser)>0)
				{
					printNote(currentUser);
				}
				else printf("[!] Login please.\n");
				break;
			case 4:
				if( strlen(currentUser) > 0 ) 
				{	
					printf("[+] Write data: ");
					scanf("%100s",link);
					fp = fopen(NOTE_DB,"r+");
					if(fp != NULL)
					{
						saveNote( link, tmp );
						fprintf(fp,"%s",tmp);
						fclose(fp);
					}
					else break;
					printf("[I] Save note success\n");
				}
				else printf("[!] Login please.\n");
				break;
			case 5:
				printf("[!] Bye~ \n");
				exit(1);
				break;
			default:
				printf("[!] Select Menu (1-5) \n");
				exit(1);
				break;
		}
	}
return 0;
}
