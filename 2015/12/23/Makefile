PROG=day23

INCLUDES=
A_FILES=

#GNU89 for snprintf
CFLAGS=-std=gnu89 -ggdb -O2 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG).o ./$(PROG).c
	gcc -g -O2 $(PROG).o $(A_FILES) -o $(PROG)

clean:
	rm -fv $(PROG) *.o
