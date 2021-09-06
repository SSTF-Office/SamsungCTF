CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf
STRIP   = $(CROSS_COMPILE)strip

OBJS = main.o

CFLAGS += -Wall -I../ta/include -I$(TEEC_EXPORT)/include -I./include -z relro -fstack-protector
#Add/link other required libraries here
LDADD += -lteec -L$(TEEC_EXPORT)/lib

BINARY = optee_rss

.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) -o $@ $< $(LDADD)
	$(STRIP) -s $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(BINARY)
