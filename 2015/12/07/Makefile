PROG=day7

CFLAGS=-std=gnu89 -ggdb -O2 -Wall -Wextra -Wpedantic -Werror

EHHT_DIR=../../../common/ehht
EHSTR_DIR=../../../common/ehstr

default: ehht ehstr
	gcc -c $(CFLAGS) -I$(EHHT_DIR) -I$(EHSTR_DIR) -o $(PROG).o ./$(PROG).c
	gcc -O2 $(PROG).o ehht.o ehstr.o -o $(PROG)

ehht: $(EHHT_DIR)/ehht.h $(EHHT_DIR)/ehht.c $(EHHT_DIR)/ehht-interface.h
	gcc -c $(CFLAGS) -I$(EHHT_DIR) -o ehht.o $(EHHT_DIR)/ehht.c

ehstr: $(EHSTR_DIR)/ehstr.h $(EHSTR_DIR)/ehstr.c
	gcc -c $(CFLAGS) -I$(EHSTR_DIR) -o ehstr.o $(EHSTR_DIR)/ehstr.c

clean:
	rm -fv $(PROG) $(PROG2) *.o
