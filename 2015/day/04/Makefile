PROG=day4

INCLUDES=
A_FILES=
LDFLAGS=-lcrypto

#gnu89 includes snprintf
CFLAGS=-std=gnu89 -ggdb -O3 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o $(A_FILES) -o $(PROG) $(LDFLAGS)

clean:
	rm -fv $(PROG) $(PROG2) *.o
