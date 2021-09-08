TARGET  = abc

SRCDIR  = src
INCDIR  = inc
OBJDIR  = obj
BINDIR  = bin

SRCS    := $(wildcard $(SRCDIR)/*.c)
INCS    := $(wildcard $(INCDIR)/*.h)
OBJS    := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CC      = gcc
CFLAGS  = -fPIE -Wall -I$(INCDIR) -DREAD_INT_BUFFER_SIZE=0x10 -DREAD_INT_REAL_SIZE=0xf -DSTAGE3
LFLAGS  = -pie -Wall -z relro -z now


.PHONY: clean

default: all

all: $(BINDIR)/$(TARGET)
	@strip $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJS)
	@echo
	@echo "[*] Compilation finished. Build executable:" $@
	@echo
	@mkdir -p $(BINDIR)
	$(CC) $(LFLAGS) $^ -o $@
	@echo
	@echo "[*] Successfully build!"

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)
