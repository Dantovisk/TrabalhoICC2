all: 
	gcc Trabalho2.c -o trabalho -std=c99 -Wall -Werror -lm

run1: clean all
	valgrind ./trabalho < teste1.txt

run2: clean all
	valgrind ./trabalho < teste2.txt

run3: clean all
	valgrind ./trabalho < teste3.txt

run4: clean all
	valgrind ./trabalho < teste4.txt

clean:
	rm -f trabalho

zip:
	zip trabalho2.zip *.c *.txt Makefile