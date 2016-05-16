PROG=day19


CFLAGS=-std=gnu89 -ggdb -O3 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o -o $(PROG) -lehstr -lehht

clean:
	rm -fv $(PROG) *.o
