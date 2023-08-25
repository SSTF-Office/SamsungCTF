all: clean escape
.PHONY: clean

escape: escape.c
	gcc -Wl,-z,relro,-z,now -Wno-format-security -s -o escape escape.c

clean:
	rm -f escape
