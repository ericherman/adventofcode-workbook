PROG=day12part1
PROG2=day12part2
#LIBDEQUE=$(HOME)/src/libdeque/.libs

#INCLUDES=-I$(LIBDEQUE)
#A_FILES=$(LIBDEQUE)/libdeque.a

CFLAGS=-std=gnu89 -ggdb -O3 -Wall -Wextra -Wpedantic -Werror
LDADD=-ldeque

default:
	gcc -c $(CFLAGS) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o -o $(PROG)
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG2).o ./$(PROG2).c
	gcc $(PROG2).o $(A_FILES) -o $(PROG2) $(LDADD)

clean:
	rm -fv $(PROG) $(PROG2) *.o
