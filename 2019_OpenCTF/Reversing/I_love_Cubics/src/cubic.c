#include <stdio.h>

//a = 1986817875
//b = 1935754597
//c = 1936803193
//a + b + c = 5859375665
//ab + bc + ca = 11443242723476524471
//abc = 7448929266150204740237250375

__int128 k[] = {((__int128)0x9ece90a0cb << 24) + 0x644db7, -5859375665LL, 1LL, ((__int128)0x18119c20a70d35a7 << 32) + 0x41f3cf47};

int getval(int src)
{
	unsigned char* s = (unsigned char*)&src;
	unsigned char dest[4];

	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 256; j++)
		{
			if (s[i] == j)
			{
				dest[i] = j;
				break;
			}
		}
	}

	return *(int*)dest;
}

int main(void)
{
	int v[4] = {0};
	__int128 temp;
	__int128 res;

	int i, j;

	for (i = 0; i < 3; i++)
	{
		scanf("%d", &v[i]);
	}

	if ((v[1] >= v[2]) || v[2] >= v[0])
	{
			printf("Wrong Input.");
			return -1;
	}

	for (i = 0; i < 3; i++)
	{
		temp = getval(v[i]);
		res = 0;
		for (j = 0; j < 3; j++)
		{
			res += temp * k[j];
			temp *= v[i];
		}
		if (res != k[j])
		{
			printf("Wrong Input.");
			return -1;
		}
	}

	printf("Congrats! The flag is: SCTF{%s}\n", (char*)v);

	return 0;
}
