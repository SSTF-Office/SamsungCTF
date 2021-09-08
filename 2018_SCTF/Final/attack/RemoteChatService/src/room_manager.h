#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

struct Room
{
    struct Room* next;
    struct Room* prev;
    int ownerSock;
    char* roomName;
    int connectionState; // 0 == make, 1 == joined, 2 == established
};

struct Room* InsertRoom(int sock, char* roomName);

struct Room* FindRoom(char* roomName);

void RemoveRoom(struct Room* room);

#endif // ROOM_MANAGER_H
