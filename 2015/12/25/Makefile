PROG=day25

#CFLAGS=-std=c89 -ggdb -O3 -Wall -Wextra -Wpedantic -Werror
CFLAGS=-std=c11 -ggdb -O3 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o -o $(PROG) -lehstr -lehbigint

clean:
	rm -fv $(PROG) *.o
