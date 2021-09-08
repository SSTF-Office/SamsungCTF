# apt-get install gcc-arm-none-eabi
CROSS=arm-none-eabi-

CFLAGS=-mcpu=arm926ej-s
DEBUG=

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: main.bin

debug: DEBUG += -DDEBUG
debug: all

startup.o: startup.s
	$(CROSS)as $(CFLAGS) -o $@ $<

%.o: %.c
	$(CROSS)gcc $(DEBUG) $(CFLAGS) -mthumb -c -o $@ $<

main.elf: main.ld startup.o $(OBJS)
	$(CROSS)ld -T main.ld $(OBJS) /usr/lib/gcc/arm-none-eabi/*/armv6-m/libgcc.a -o $@

main.bin: main.elf
	$(CROSS)objcopy -O binary $< $@

run: main.bin
	echo -n "SCTF{t1ny_tIny_baremetal_firmware}" | \
		qemu-system-arm -M versatilepb -m 128M -nographic -kernel main.bin

objdump: main.elf
	$(CROSS)objdump -d main.elf

clean:
	rm -f *.o *.elf *.bin
