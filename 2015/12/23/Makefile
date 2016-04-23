PROG=day23

INCLUDES=
A_FILES=

#GNU89 for snprintf
CFLAGS=-std=gnu89 -ggdb -O0 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o $(A_FILES) -o $(PROG)

clean:
	rm -fv $(PROG) *.o
