#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void showFlag()
{
	char buffer[128];
	FILE* fp = fopen("flag", "r");
 	fgets(buffer, sizeof(buffer), fp);
 	printf("Thank you for using the magic code!\n");
    printf("The flag is %s\n", buffer);
    fclose(fp);
}

int playOthello(unsigned int result)
{
	printf("\n\nLet the games Begin... Your card is %x\n", result);
	return result == '!';
}

int main(void)
{
	struct {
		unsigned int select;
		char name[64];
		char *color;
	} v;

	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);

	printf("Welcome to Othello game!\n");
	printf("Please select your color.\n");
	printf(" 1: Black\n");
	printf(" 2: White\n");
	printf(" > ");
	scanf("%d", &v.select);

	if (v.select == 1)
	{
		v.color = "Black";
	}
	else if (v.select == 2)
	{
		v.color = "White";
	}
	else
	{
		printf("You selected a wrong number.\n");
		exit(0);
	}

	printf("okay, please remember this magic code: %p\n", v.name);

	printf("Please enter your name: ");
	scanf("%s", v.name);

	printf("%s, your color is ", v.name);
	printf(v.color);

	if (playOthello(v.select))
	{
		printf("Congrats, You Win!!\n");
		showFlag();
	}
	else
	{
		printf("Sorry, you lose.\n");
	}

	return 0;
}
