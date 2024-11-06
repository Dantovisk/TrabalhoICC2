all: 
	gcc Trabalho2.c -o trabalho -std=c99 -Wall -Werror -lm

run1: all
	valgrind ./trabalho < teste1.txt

run2: all
	valgrind ./trabalho < teste2.txt

clean:
	rm -f trabalho

zip:
	zip trabalho2.zip *.c *.txt Makefile