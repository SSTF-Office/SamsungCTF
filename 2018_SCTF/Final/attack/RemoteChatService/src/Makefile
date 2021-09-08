CC=gcc
CCFLAGS=-fno-stack-protector -static -Wall
LIBS=-lpthread

EXECUTABLES=chat_server chat_client
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: $(EXECUTABLES)
	strip -Nmain -NMakeRoom -NError -NChatReceiver -NJoinRoom -NChat -NReadLine -NConnect -NPrintLogo -NPrintMenu -NgServerSock -NbFalse -NbTrue -NgBuffer chat_client
	strip -Nmain -NChildHandler -NError -NRead -NChatClientHandler -NChatHandler -NMakeRoom -NJoinRoom -NClientHandler -NInsertRoom -NFindRoom -NRemoveRoom -NgRoomHead -NgRoomSema -NgChatSenderSema -NgChatReceiverSema -NgChatData -NgDataSize -NgSender -NbTrue -NbFalse chat_server

chat_server: chat_server.o room_manager.o
	$(CC) $(CCFLAGS) -o chat_server chat_server.o room_manager.o $(LIBS)

chat_client: chat_client.o
	$(CC) $(CCFLAGS) -o chat_client chat_client.o $(LIBS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm $(EXECUTABLES) *.o
