#include "chat_server.h"
#include "room_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <signal.h>

#define PORT 31337
//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x) (void)0
#endif

void ChildHandler()
{
    int status;
    wait(&status);
}

void Error(char* str)
{
    perror(str);
    exit(EXIT_FAILURE);
}

sem_t gRoomSema;
sem_t gChatSenderSema;
sem_t gChatReceiverSema;
unsigned char gChatData[2048];
unsigned int gDataSize = 0;
int gSender = 0;
char bTrue = 1;
char bFalse = 0;

int Read(int fd, char* buffer, int dataLen)
{
    int readSize = 0;

    while (dataLen > 0)
    {
        readSize = recv(fd, buffer, dataLen, 0);
        if (readSize <= 0)
        {
            return readSize;
        }

        buffer += readSize;
        dataLen -= readSize;
    }

    return bTrue;
}

void* ChatClientHandler(void* arg)
{
    struct ClientInfo* ci = (struct ClientInfo*)arg;
    int clientSock = ci->sock;
    unsigned int dataLen;
    char buffer[2048];
    int ret;

    while ((ret = Read(clientSock, (char*)&dataLen, 4)) == bTrue)
    {
        DEBUG_PRINT(("receive dataLen == %d\n", dataLen));
        if (dataLen > 2048)
            dataLen = 2048;

        ret = Read(clientSock, buffer, dataLen);
        if (ret <= 0 || strncmp(buffer, "\xde\xad\xf0\x0d", 4)  == 0)
            break;

        if (strncmp(buffer, "/bye", 4) == 0) // not exit
        {
            DEBUG_PRINT(("bye received\n"));
            send(clientSock, "\xde\xad\xf0\x0d", 4, 0);
            ci->isClosed = bTrue;
            continue;
        }

        memcpy(gChatData, buffer, dataLen);
        gDataSize = dataLen;
        gSender = clientSock;
        sem_post(&gChatSenderSema);
        sem_wait(&gChatReceiverSema);
    }

    sem_post(&gChatSenderSema);

    // client disconnectted unconditionally
    if (ret == 0)
    {
        ci->isClosed = bTrue;
        DEBUG_PRINT(("socket close chat server\n"));
    }
    // recv error, ret < 0
    else if (ci->isClosed == bFalse)
    {
        perror("recv failed!");
        ci->isClosed = bTrue;
    }
    // else: client sent '/bye'

    return 0;
}

int ChatHandler(int* sock1, int* sock2)
{
    char name[128];
    unsigned char chat[1024];
    struct ClientInfo ci1;
    struct ClientInfo ci2;
    pthread_t threadId;
    sem_init(&gChatSenderSema, 0, 0);
    sem_init(&gChatReceiverSema, 0, 0);

    ci1.sock = *sock1;
    ci1.isClosed = bFalse;
    ci2.sock = *sock2;
    ci2.isClosed = bFalse;

    DEBUG_PRINT(("ChatHandler %ls %d %ls %d\n", sock1, *sock1, sock2, *sock2));
    if (pthread_create(&threadId, NULL, ChatClientHandler, (void*)&ci1) < 0)
        Error("pthread_create ChatClient 1 failed!");

    if (pthread_create(&threadId, NULL, ChatClientHandler, (void*)&ci2) < 0)
        Error("pthread_create ChatClient 2 failed!");

    while (ci1.isClosed == bFalse || ci2.isClosed == bFalse) // while sock1, sock2 alive
    {
        sem_wait(&gChatSenderSema);
        memcpy(chat, gChatData, gDataSize);
        DEBUG_PRINT(("gDataSize = %d\n", gDataSize));

        if (gSender == ci1.sock)
        {
            // check socket alive
            if (ci2.isClosed == bFalse)
            {
                sprintf(name, "Remote%d: ", ci1.sock);
                send(ci2.sock, name, strlen(name), 0);
                send(ci2.sock, chat, gDataSize, 0);
                send(ci2.sock, "\n", 1, 0);
            }
        }
        else if (gSender == ci2.sock)
        {
            // check socket alive
            if (ci1.isClosed == bFalse)
            {
                sprintf(name, "Remote%d: ", ci2.sock);
                send(ci1.sock, name, strlen(name), 0);
                send(ci1.sock, chat, gDataSize, 0);
                send(ci1.sock, "\n", 1, 0);
            }
        }
        sem_post(&gChatReceiverSema);
    }

    DEBUG_PRINT(("chat ended!\n"));

    sem_destroy(&gChatSenderSema);
    sem_destroy(&gChatReceiverSema);

    return 0;
}

int MakeRoom(int clientSock, char* roomName)
{
    char result = bTrue;
    struct Room* room = NULL;
    DEBUG_PRINT(("make room %s\n", roomName));

    sem_wait(&gRoomSema);
    if (FindRoom(roomName) != NULL)
    {
        DEBUG_PRINT(("room already exist!\n"));
        result = bFalse;
    }
    else
    {
        DEBUG_PRINT(("InsertRoom %s\n", roomName));
        if ((room = InsertRoom(clientSock, roomName)) == NULL)
            result = bFalse;
    }
    sem_post(&gRoomSema);

    send(clientSock, &result, 1, 0);
    if (result == bTrue)
    {
        // wait 10 secs until someone join my room
        for (int i = 0; i < 10; i++)
        {
            DEBUG_PRINT(("wait joiner\n"));
            sleep(1);

            sem_wait(&gRoomSema);
            if (room->connectionState == 1)
            {
                DEBUG_PRINT(("joined!\n"));
                room->connectionState = 2;
                sem_post(&gRoomSema);
                break;
            }

            if (i == 9)
            {
                DEBUG_PRINT(("No jointer... remove room\n"));
                RemoveRoom(room);
                room = NULL;
                result = bFalse;
            }

            sem_post(&gRoomSema);
        }

        send(clientSock, &result, 1, 0);
    }

    return result;
}

int JoinRoom(int clientSock, char* roomName)
{
    struct Room* room;
    char result = bFalse;
    DEBUG_PRINT(("join room %s\n", roomName));

    sem_wait(&gRoomSema);
    if ((room = FindRoom(roomName)) != NULL)
    {
        DEBUG_PRINT(("room found!\n"));
        if (room->connectionState == 0)
        {
            DEBUG_PRINT(("checked!\n"));
            room->connectionState = 1;
            result = bTrue;
        }
    }
    sem_post(&gRoomSema);

    send(clientSock, &result, 1, 0);

    if (result == bTrue)
    {
        DEBUG_PRINT(("found room\n"));
        result = bFalse;
        // wait owner state
        for (int i = 0; i < 3; i++)
        {
            sleep(1);
            if (room->connectionState == 2)
            {
                result = bTrue;
                break;
            }
        }

        if (result == bTrue)
        {
            int ownerSock = room->ownerSock;
            RemoveRoom(room);
            room = NULL;

            if (fork() == 0)
            {
                DEBUG_PRINT(("forkd with sockets %ls %d %ls %d\n", &ownerSock, ownerSock, &clientSock, clientSock));
                ChatHandler(&ownerSock, &clientSock);

                if (ownerSock != -1)
                {
                    DEBUG_PRINT(("ownerSock close\n"));
                    close(ownerSock);
                }
                if (clientSock != -1)
                {
                    DEBUG_PRINT(("clientSock close\n"));
                    close(clientSock);
                }
            }
            else
            {
                close(ownerSock);
                close(clientSock);
            }
        }
    }
    else
    {
        DEBUG_PRINT(("find room failed!\n"));
    }

    return result;
}

void* ClientHandler(void* argv)
{
    int clientSock = *(int*)argv;
    char buffer[256];
    int readSize;
    int result;

    while ((readSize = recv(clientSock, &buffer, 255, 0)) > 0)
    {
        buffer[readSize] = '\0';
        if (buffer[0] == 1)
        {
            DEBUG_PRINT(("make socket address %ls %d \n", &clientSock, clientSock));
            if ((result = MakeRoom(clientSock, &buffer[1])) == bTrue)
            {
                DEBUG_PRINT(("make success\n"));
                break;
            }
        }
        else if (buffer[0] == 2)
        {
            DEBUG_PRINT(("join socket address %ls %d\n", &clientSock, clientSock));
            if ((result = JoinRoom(clientSock, &buffer[1])) == bTrue)
            {
                DEBUG_PRINT(("join success\n"));
                break;
            }
        }
    }

    DEBUG_PRINT(("escape while!\n"));

    if (readSize == 0)
    {
        DEBUG_PRINT(("socket close %ls\n", &clientSock));
        close(clientSock);
        clientSock = -1;
    }
    else if (result != bTrue)
    {
        perror("recv failed!");
    }

    return 0;
}

int main()
{
    int optval = 0x31337;
    int serverSock, clientSock;
    struct sockaddr_in addr;
    int addrLen = sizeof(addr);
    pthread_t threadId;
    int mode = 0; // blocking mode

    signal(SIGCHLD, (void*)ChildHandler);
    sem_init(&gRoomSema, 0, 1);

    if ((serverSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        Error("socket failed!");

    if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) < 0)
        Error("setsockopt failed!");

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(serverSock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        Error("bind failed!");

    if (listen(serverSock, 10) < 0)
        Error("listen failed!");

    while (1)
    {
        DEBUG_PRINT(("wait client!!!\n"));
        if ((clientSock = accept(serverSock, (struct sockaddr*)&addr, (socklen_t*)&addrLen)) < 0)
            Error("accept failed!");

        DEBUG_PRINT(("client accepted!\n"));

        ioctl(clientSock, FIONBIO, &mode);

        if (pthread_create(&threadId, NULL, ClientHandler, (void*)&clientSock) < 0)
            Error("pthread_create failed!");
    }

    sem_destroy(&gRoomSema);

    return 0;
}
