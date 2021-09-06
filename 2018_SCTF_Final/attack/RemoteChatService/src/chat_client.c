#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

int bTrue = 1;
int bFalse = 0;
int gServerSock = 0;
unsigned char gBuffer[2048 + 4];

void Error(char* str)
{
    perror(str);
    exit(EXIT_FAILURE);
}

void PrintLogo()
{
    puts(" _____                      _                  ");
    puts("|  __ \\                    | |                 ");
    puts("| |__) |___ _ __ ___   ___ | |_ ___            ");
    puts("|  _  // _ \\ '_ ` _ \\ / _ \\| __/ _ \\           ");
    puts("| | \\ \\  __/ | | | | | (_) | ||  __/           ");
    puts("|_|  \\_\\___|_|_|_|_|_|\\___/ \\__\\___|           ");
    puts("             / ____| |         | |             ");
    puts("            | |    | |__   __ _| |_            ");
    puts("            | |    | '_ \\ / _` | __|           ");
    puts("            | |____| | | | (_| | |_            ");
    puts("             \\_____|_| |_|\\__,_|\\__|           ");
    puts("             / ____|                           ");
    puts("            | (___   ___ _ ____   ___  ___ ___ ");
    puts("             \\___ \\ / _ \\ '__\\ \\ / / |/ __/ _ \\");
    puts("             ____) |  __/ |   \\ V /| | (_|  __/");
    puts("            |_____/ \\___|_|    \\_/ |_|\\___\\___|");
}

void PrintMenu()
{
    puts("1. Make room");
    puts("2. Join room");
    puts("3. Exit");
    printf(">>");
}

int ReadLine(unsigned char* buffer, int maxLen)
{
    int readSize = 0;
    int readLen = 0;

    while ((readSize = read(0, buffer, 1)) > 0)
    {
        if (++readLen == maxLen)
            break;
        if (*buffer == '\n')
        {
            *buffer = '\0';
            break;
        }
        ++buffer;
    }

    return readLen;
}

void Connect()
{
    struct sockaddr_in addr;
    int mode = 0; // blocking mode

    if ((gServerSock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        Error("socket failed!");

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;

    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(31337);

    if (connect(gServerSock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        Error("connect failed!");

    ioctl(gServerSock, FIONBIO, &mode);
}

void* ChatReceiver(void* arg)
{
    int readSize = 0;
    unsigned char buffer[256];

    // BOF vulnerability!!!
    while ((readSize = read(gServerSock, buffer, 1024)) > 0)
    {
        if (memcmp(buffer, "\xde\xad\xf0\x0d", 4) == 0)
        {
            break;
        }
        write(1, buffer, readSize);
    }

    return 0;
}

void Chat()
{
    int readLen;
    pthread_t threadId;

    if (pthread_create(&threadId, NULL, ChatReceiver, NULL) < 0)
        Error("pthread_create ChatReceiver failed!");

    puts("==========Chat Start==========");

    while (bTrue)
    {
        readLen = ReadLine(&gBuffer[4], 1024);
        *((int*)&gBuffer[0]) = readLen;
        write(gServerSock, gBuffer, readLen + 4);
        if (memcmp(&gBuffer[4], "/bye", 4) == 0)
            break;
    }

    pthread_join(threadId, NULL);

    // send 0xdeadf00d to server
    write(gServerSock, "\x04\x00\x00\x00\xde\xad\xf0\x0d", 8);

    puts("==========Chat Ended==========");

    close(gServerSock);
    gServerSock = 0;
}

void MakeRoom()
{
    int readLen = 0;

    printf("Room Name>>");
    readLen = ReadLine(&gBuffer[1], 255);

    gBuffer[0] = 1;
    write(gServerSock, gBuffer, readLen + 1);
    read(gServerSock, gBuffer, 1);

    if (gBuffer[0] == bTrue)
    {
        puts("wait 10 seconds...");
        read(gServerSock, gBuffer, 10);
        if (gBuffer[0] == bTrue)
        {
            puts("Someone joined!");
            Chat();
        }
        else
        {
            puts("Anyone not joined");
        }
    }
    else
    {
        puts("Make room failed");
    }
}

void JoinRoom()
{
    int readLen = 0;

    printf("Room Name>>");
    readLen = ReadLine(&gBuffer[1], 255);

    gBuffer[0] = 2;
    write(gServerSock, gBuffer, readLen + 1);
    read(gServerSock, gBuffer, 1);

    if (gBuffer[0] == bTrue)
    {
        puts("Join room succeeded");
        Chat();
    }
    else
    {
        puts("Join room failed");
    }
}

int main()
{
    char buf[10];
    int menu = 0;
    int isExit = bFalse;

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    PrintLogo();

    while (isExit == bFalse)
    {
        PrintMenu();
        ReadLine((unsigned char*)buf, 10);
        menu = atoi(buf);

        if (menu < 3 && gServerSock == 0)
            Connect();

        switch (menu)
        {
            case 1:
                if (gServerSock)
                    MakeRoom();
                else
                    puts("not connected");
                break;
            case 2:
                if (gServerSock)
                    JoinRoom();
                else
                    puts("not connected");
                break;
            case 3:
                isExit = bTrue;
                break;
            default:
                puts("Invalid Menu");
                break;
        }
    }

    puts("Bye");

    return 0;
}
