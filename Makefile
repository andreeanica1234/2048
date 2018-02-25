
build: 2048.c
	gcc -w  2048.c -o 2048 -lncurses

run: 2048
	./2048

clean: 
	rm -f 2048
