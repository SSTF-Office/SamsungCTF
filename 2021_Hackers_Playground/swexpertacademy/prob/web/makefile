test: test.c
	gcc -ansi test.c -o test -std=c99
pre:
	gcc -ansi -E -std=c99 test.c -o test

clean:
	rm ./test
run:
	./test < input.txt