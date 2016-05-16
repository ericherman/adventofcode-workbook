PROG=day10

CFLAGS=-std=gnu89 -ggdb -O3 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o -o $(PROG)

clean:
	rm -fv $(PROG) $(PROG2) *.o
