CC=gcc
CFLAG=-g -Wall 
TARGET=prob
OBJS= prob.o file.o
BAKS=*.tar *.bak 

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) -lssl -lcrypto -ldl
	strip $(TARGET)
	cp $(TARGET) ./../deploy/
	chmod +x ./../deploy/$(TARGET) 
	cp flag ./../deploy/

file.o : file.c
	$(CC) -c -o file.o file.c -lssl -lcrypto

main.o : prob.c
	$(CC) -c -o prob.o prob.c -lssl -lcrypto

clean:
	rm $(OBJS) $(TARGET) $(BAKS)
