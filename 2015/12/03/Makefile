PROG=day3part1
PROG2=day3part2

LD_ADD=-lehht

#gnu89 includes snprintf
CFLAGS=-std=gnu89 -ggdb -O3 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o -o $(PROG) $(LD_ADD)
	gcc -c $(CFLAGS) -o $(PROG2).o ./$(PROG2).c
	gcc $(PROG2).o -o $(PROG2) $(LD_ADD)

clean:
	rm -fv $(PROG) $(PROG2) *.o
