PROG=day4

INCLUDES=
A_FILES=
LDFLAGS=
LDADD=-lcrypto

#gnu89 includes snprintf
CFLAGS=-std=gnu89 -ggdb -O2 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG).o ./$(PROG).c
	gcc -g -O2 $(LDFLAGS) $(PROG).o $(A_FILES) -o $(PROG) $(LDADD)

clean:
	rm -fv $(PROG) $(PROG2) *.o
