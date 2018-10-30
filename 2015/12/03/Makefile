PROG=day3part1
PROG2=day3part2

EHHT_DIR=../../../common/ehht

#gnu89 includes snprintf
CFLAGS=-std=gnu89 -ggdb -O2 -Wall -Wextra -Wpedantic -Werror


default: ehht $(PROG).c $(PROG2).c
	gcc -c $(CFLAGS) -I$(EHHT_DIR) -o $(PROG).o ./$(PROG).c
	gcc -g -O2 $(PROG).o ehht.o -o $(PROG)
	gcc -c $(CFLAGS) -I$(EHHT_DIR) -o $(PROG2).o ./$(PROG2).c
	gcc -g -O2 $(PROG2).o ehht.o -o $(PROG2)

ehht: $(EHHT_DIR)/ehht.h $(EHHT_DIR)/ehht.c $(EHHT_DIR)/ehht-interface.h
	gcc -c $(CFLAGS) -I$(EHHT_DIR) -o ehht.o $(EHHT_DIR)/ehht.c

clean:
	rm -fv $(PROG) $(PROG2) *.o
