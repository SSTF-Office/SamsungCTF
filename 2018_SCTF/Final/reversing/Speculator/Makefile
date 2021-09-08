CC=g++
CCFLAGS=-std=c++11 -Wl,-z,now -Wl,-z,relro -Wall
CCFLAGS+=-DBTB -DCACHE
POST=strip
SRC_DIR=src
TARGET=Speculator

SRC=$(wildcard $(SRC_DIR)/Speculator/*.cc)
SRC+=$(SRC_DIR)/main.cc

all: cleandist bin prog

bin: $(TARGET)
	$(POST) dist/$(TARGET)

debug: CCFLAGS += -DDEBUG
debug: $(TARGET)

cleandist:
	rm -rf dist
	mkdir dist

prog:
	./util/compiler.py exploit/cipher.txt exploit/correct.bin >/dev/null
	./util/compiler.py prob/empty.txt dist/kernel.bin >/dev/null
	./util/compiler.py prob/prob.txt dist/prob.bin >/dev/null
	cd dist && \
	zip Speculator.zip Speculator kernel.bin prob.bin

$(TARGET):
		$(CC) $(CCFLAGS) $(SRC) -o dist/$(TARGET)

run: bin prog
	./util/compiler.py exploit/cipher.txt exploit/correct.bin >/dev/null
	./util/compiler.py prob/empty.txt dist/kernel.bin >/dev/null
	./dist/$(TARGET) dist/kernel.bin exploit/correct.bin
	./util/compiler.py prob/prob.txt dist/prob.bin >/dev/null

clean:
		rm -f $(TARGET)
