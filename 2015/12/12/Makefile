PROG=day12part1
PROG2=day12part2

CFLAGS=-std=gnu89 -ggdb -O2 -Wall -Wextra -Wpedantic -Werror

DEQUE_DIR=../../../common/deque

default: $(PROG) $(PROG2)

$(PROG):
	gcc -c $(CFLAGS) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o -o $(PROG)

$(PROG2): deque
	gcc -c $(CFLAGS) -I$(DEQUE_DIR) -o $(PROG2).o ./$(PROG2).c
	gcc -g -O2 $(PROG2).o deque.o -o $(PROG2)

deque: $(DEQUE_DIR)/deque.h $(DEQUE_DIR)/deque.c
	gcc -c $(CFLAGS) -I$(DEQUE_DIR) -o deque.o $(DEQUE_DIR)/deque.c

clean:
	rm -fv $(PROG) $(PROG2) *.o
