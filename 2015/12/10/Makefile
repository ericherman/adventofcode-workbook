PROG=day10

CFLAGS=-std=gnu89 -ggdb -O2 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) -o $(PROG).o ./$(PROG).c
	gcc -g -O2 $(PROG).o -o $(PROG)

clean:
	rm -fv $(PROG) $(PROG2) *.o
