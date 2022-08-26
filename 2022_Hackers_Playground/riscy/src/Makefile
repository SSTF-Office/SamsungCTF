CC := riscv64-linux-gnu-gcc
CFLAGS := -static -fno-stack-protector
ALL := target

all: $(ALL)

target: crackme0x00.c
	$(CC) $(CFLAGS) -o $@ $<
	checksec --file $@

clean:
	rm -f $(ALL)

.PHONY: all clean
