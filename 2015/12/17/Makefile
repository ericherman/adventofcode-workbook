PROG=day17

CFLAGS=-std=gnu89 -ggdb -O0 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o -o $(PROG) -lehht

clean:
	rm -fv $(PROG) $(PROG2) *.o
