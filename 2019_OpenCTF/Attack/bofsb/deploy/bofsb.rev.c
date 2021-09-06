/***************************/
/* source codes from bofmt */
/* decompiled by IDA pro   */
/***************************/

unsigned int showFlag()
{
  FILE *stream; // ST0C_4
  char s; // [esp+4h] [ebp-88h]
  unsigned int v3; // [esp+84h] [ebp-8h]

  v3 = __readgsdword(0x14u);
  stream = fopen("flag", (const char *)&unk_A60);
  fgets(&s, 128, stream);
  puts("Thank you for using the magic code!");
  printf("The flag is %s\n", &s);
  fclose(stream);
  return __readgsdword(0x14u) ^ v3;
}

_BOOL4 __cdecl playOthello(int a1)
{
  printf("\n\nLet the games Begin... Your card is %x\n", a1);
  return a1 == 33;
}

int __cdecl main(int argc, const char **argv, const char **envp)
{
  int v4; // [esp+0h] [ebp-50h]
  int v5; // [esp+4h] [ebp-4Ch]
  char *format; // [esp+44h] [ebp-Ch]
  unsigned int v7; // [esp+48h] [ebp-8h]

  v7 = __readgsdword(0x14u);
  setvbuf(stdout, 0, 2, 0);
  setvbuf(stdin, 0, 2, 0);
  puts("Welcome to Othello game!");
  puts("Please select your color.");
  puts(" 1: Black");
  puts(" 2: White");
  printf(" > ");
  __isoc99_scanf("%d", &v4);
  if ( v4 == 1 )
  {
    format = "Black";
  }
  else
  {
    if ( v4 != 2 )
    {
      puts("You selected a wrong number.");
      exit(0);
    }
    format = "White";
  }
  printf("okay, please remember this magic code: %p\n", &v5);
  printf("Please enter your name: ");
  __isoc99_scanf("%s", &v5);
  printf("%s, your color is ", &v5);
  printf(format);
  if ( playOthello(v4) )
  {
    puts("Congrats, You Win!!");
    showFlag();
  }
  else
  {
    puts("Sorry, you lose.");
  }
  return 0;
}
