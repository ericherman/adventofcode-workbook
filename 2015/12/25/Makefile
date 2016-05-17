PROG=day25

LIBEHBIGINT=$(HOME)/src/libehbigint

INCLUDES=-I$(LIBEHBIGINT)
A_FILES=$(LIBEHBIGINT)/libehbigint.a

#CFLAGS=-std=c89 -ggdb -O3 -Wall -Wextra -Wpedantic -Werror
CFLAGS=-std=c11 -ggdb -O3 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o $(A_FILES) -o $(PROG) -lehstr

clean:
	rm -fv $(PROG) *.o
