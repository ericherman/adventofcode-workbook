PROG=day3part1
LIBEHHT=$(HOME)/src/libehht

INCLUDES=-I$(LIBEHHT)
A_FILES=$(LIBEHHT)/libehht.a

#gnu89 includes snprintf
CFLAGS=-std=gnu89 -ggdb -O3 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o $(A_FILES) -o $(PROG)

clean:
	rm -fv $(PROG) *.o
