CC=gcc
LDFLAGS=-lgmp
HEADERS=yay.h
SOURCES=yo.c yay.c
EXECUTABLE=my_secure_encryptor

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) -o $(EXECUTABLE) $(SOURCES) $(LDFLAGS)
	strip $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean
