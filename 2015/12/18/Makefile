PROG=day18

CFLAGS=-std=c89 -ggdb -O0 -Wall -Wextra -Wpedantic -Werror

default:
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG).o ./$(PROG).c
	gcc $(PROG).o $(A_FILES) -o $(PROG)

clean:
	rm -fv $(PROG) $(PROG2) *.o
