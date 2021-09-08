CC=gcc
LDFLAGS=-lcrypto

all: magic

magic: magic.c
	$(CC) -o magic magic.c $(LDFLAGS)

clean:
	rm -f magic
