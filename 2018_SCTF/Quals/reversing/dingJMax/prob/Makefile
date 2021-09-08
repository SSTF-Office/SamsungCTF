CC=gcc
LDFLAGS=-lncursesw
HEADERS=yay.h yo.h stage.h
SOURCES=yo.c yay.c
EXECUTABLES=dingJMax

all: $(EXECUTABLES)

$(EXECUTABLES): $(SOURCES) $(HEADERS)
	$(CC) -o $(EXECUTABLES) $(SOURCES) $(LDFLAGS) -DRELEASE
	$(CC) -o $(EXECUTABLES)_solver $(SOURCES) $(LDFLAGS) -DSOLUTION
	strip $(EXECUTABLES)

clean:
	rm -f $(EXECUTABLES)
	rm -f $(EXECUTABLES)_solver
