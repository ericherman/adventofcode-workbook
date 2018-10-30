PROG=day11

CFLAGS=-std=c89 -ggdb -O2 -Wall -Wextra -Wpedantic -Werror

# extracted from https://github.com/torvalds/linux/blob/master/scripts/Lindent
LINDENT=indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0

default:
	gcc -c $(CFLAGS) $(INCLUDES) -o $(PROG).o ./$(PROG).c
	gcc -g -O2 $(PROG).o $(A_FILES) -o $(PROG)

tidy:
	$(LINDENT) \
		-T FILE \
		-T size_t \
		*.c

clean:
	rm -fv $(PROG) $(PROG2) *.o
