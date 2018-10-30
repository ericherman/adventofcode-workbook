PROG=day24

CFLAGS=-std=c89 -ggdb -O2 -Wall -Wextra -Wpedantic -Werror

EHSTR_DIR=../../../common/ehstr
EHBI_DIR=../../../common/ehbigint
EBA_DIR=../../../common/eba

default: ehbigint ehstr
	gcc -c $(CFLAGS) -I$(EHBI_DIR) -I$(EHSTR_DIR) -o $(PROG).o ./$(PROG).c
	gcc -g -O2 $(PROG).o eba.o ehstr.o \
		ehbigint-eba.o ehbigint-log.o ehbigint-malloc.o \
		ehbigint-priv.o ehbigint-str.o ehbigint-util.o ehbigint.o \
		-o $(PROG)

ehbigint: eba ehstr \
		$(EHBI_DIR)/ehbigint-eba.h $(EHBI_DIR)/ehbigint-eba.c \
		$(EHBI_DIR)/ehbigint-log.h $(EHBI_DIR)/ehbigint-log.c \
		$(EHBI_DIR)/ehbigint-malloc.h $(EHBI_DIR)/ehbigint-malloc.c \
		$(EHBI_DIR)/ehbigint-priv.h $(EHBI_DIR)/ehbigint-priv.c \
		$(EHBI_DIR)/ehbigint-str.h $(EHBI_DIR)/ehbigint-str.c \
		$(EHBI_DIR)/ehbigint-util.h $(EHBI_DIR)/ehbigint-util.c \
		$(EHBI_DIR)/ehbigint.h $(EHBI_DIR)/ehbigint.c
	gcc -c $(CFLAGS) -I$(EHBI_DIR) -I$(EBA_DIR) -o ehbigint-eba.o \
		$(EHBI_DIR)/ehbigint-eba.c
	gcc -c $(CFLAGS) -I$(EHBI_DIR) -o ehbigint-log.o \
		$(EHBI_DIR)/ehbigint-log.c
	gcc -c $(CFLAGS) -I$(EHBI_DIR) -I$(EBA_DIR) -o ehbigint-malloc.o \
		$(EHBI_DIR)/ehbigint-malloc.c
	gcc -c $(CFLAGS) -I$(EHBI_DIR) -I$(EBA_DIR) -o ehbigint-priv.o \
		$(EHBI_DIR)/ehbigint-priv.c
	gcc -c $(CFLAGS) -I$(EHBI_DIR) -I$(EHSTR_DIR) -I$(EBA_DIR) \
		-o ehbigint-str.o $(EHBI_DIR)/ehbigint-str.c
	gcc -c $(CFLAGS) -I$(EHBI_DIR) -o ehbigint-util.o \
		$(EHBI_DIR)/ehbigint-util.c
	gcc -c $(CFLAGS) -I$(EHBI_DIR) -I$(EBA_DIR) -o ehbigint.o \
		$(EHBI_DIR)/ehbigint.c

ehstr: $(EHSTR_DIR)/ehstr.h $(EHSTR_DIR)/ehstr.c
	gcc -c $(CFLAGS) -I$(EHSTR_DIR) -o ehstr.o $(EHSTR_DIR)/ehstr.c

eba: $(EBA_DIR)/eba.h $(EBA_DIR)/eba.c
	gcc -c $(CFLAGS) -I$(EBA_DIR) -o eba.o $(EBA_DIR)/eba.c

clean:
	rm -fv $(PROG) $(PROG2) *.o
