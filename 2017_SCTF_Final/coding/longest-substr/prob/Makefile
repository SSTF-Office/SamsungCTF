all:
	python download-ebook.py
	gcc -o ../exploit/solve.c.bin ../exploit/solve.c
	g++ -o ../exploit/solve.cpp.bin ../exploit/solve.cpp
	./../exploit/solve.c.bin < ebook0.txt
	./../exploit/solve.cpp.bin < ebook0.txt
	python2 ../exploit/solve.py2 < ebook0.txt
	python3 ../exploit/solve.py3 < ebook0.txt
	./../exploit/solve.c.bin < ebook1.txt
	./../exploit/solve.cpp.bin < ebook1.txt
	python2 ../exploit/solve.py2 < ebook1.txt
	python3 ../exploit/solve.py3 < ebook1.txt

clean:
	rm -rf ebook* ../exploit/solve.*.bin
