PROG=day24

#LIBEHBIGINT=$(HOME)/src/libehbigint
#INCLUDES=-I$(LIBEHBIGINT)
#A_FILES=$(LIBEHBIGINT)/libehbigint.a

CFLAGS=-std=c89 -ggdb -O0 -Wall -Wextra -Wpedantic -Werror
LDADD=-lehbigint -lehstr

default:
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o $(A_FILES) -o $(PROG) $(LDADD)

clean:
	rm -fv $(PROG) *.o
