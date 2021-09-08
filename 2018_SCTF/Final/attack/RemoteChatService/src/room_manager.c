#include "room_manager.h"
#include <stdlib.h>
#include <string.h>

struct Room gRoomHead;

struct Room* InsertRoom(int sock, char* roomName)
{
    struct Room* room = (struct Room*)malloc(sizeof(struct Room));

    if (room)
    {
        room->ownerSock = sock;
        room->roomName = roomName;
        room->connectionState = 0;

        if (gRoomHead.next)
        {
            gRoomHead.next->prev = room;
        }
        room->next = gRoomHead.next;
        room->prev = &gRoomHead;
        gRoomHead.next = room;
    }

    return room;
}

struct Room* FindRoom(char* roomName)
{
    struct Room* room = gRoomHead.next;

    while (room != NULL)
    {
        if (strcmp(room->roomName, roomName) == 0)
            return room;
        room = room->next;
    }

    return NULL;
}

void RemoveRoom(struct Room* room)
{
    if (room->next)
    {
        room->next->prev = room->prev;
    }
    room->prev->next = room->next;

    free(room);
}
