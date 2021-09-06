CC = gcc
RM = rm -rf
STRIP = strip

OPTION=-fstack-protector

default:
	$(CC) $(OPTION) -c ./md5.c -o ./md5.o
	$(CC) $(OPTION) -c ./heap.c -o ./heap.o
	$(CC) $(OPTION) -o ./heap ./*.o -lssl -lcrypto
	$(STRIP) -s ./heap
clean :
	$(RM) ./*.o

remove :
	$(RM) ./*.c
	$(RM) ./*.h
