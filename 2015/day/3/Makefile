PROG=day3part1
PROG2=day3part2
LIBEHHT=$(HOME)/src/libehht

INCLUDES=-I$(LIBEHHT)
A_FILES=$(LIBEHHT)/libehht.a

#gnu89 includes snprintf
CFLAGS=-std=gnu89 -ggdb -O3 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o $(A_FILES) -o $(PROG)
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG2).o ./$(PROG2).c
	gcc $(PROG2).o $(A_FILES) -o $(PROG2)

clean:
	rm -fv $(PROG) $(PROG2) *.o
