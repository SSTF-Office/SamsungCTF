CC = g++
CFLAG =-O3 -march=native -fPIC -s -Wall 
TARGET = prob

server : prob
	$(CC) $(CFLAG) -o prob prob.cpp

clean:
	rm $(TARGET) 
