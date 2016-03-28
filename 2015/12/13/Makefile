PROG=day13
LIBEHHT=$(HOME)/src/libehht

INCLUDES=-I$(LIBEHHT)
A_FILES=$(LIBEHHT)/libehht.a

CFLAGS=-std=gnu89 -ggdb -O3 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o $(A_FILES) -o $(PROG)

clean:
	rm -fv $(PROG) $(PROG2) *.o
