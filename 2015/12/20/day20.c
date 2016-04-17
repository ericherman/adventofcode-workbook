#include <stdio.h>		/* printf */
#include <stdlib.h>		/* atoi */
#include <math.h>		/* lrint */

#define BUF_LEN 255

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	char buf[BUF_LEN];
	int matched, verbose, max50;
	unsigned long target, house, elf, found, to_deliver, count;
	unsigned long *houses;
	size_t len;

	verbose = (argc > 1) ? atoi(argv[1]) : 0;
	max50 = (argc > 2) ? atoi(argv[2]) : 0;
	input_file_name = (argc > 3) ? argv[3] : "input";
	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	matched = 0;
	while (!matched && fgets(buf, BUF_LEN, input)) {
		matched = sscanf(buf, "%lu", &target);
	}
	fclose(input);
	if (!matched) {
		fprintf(stderr, "target not found\n");
		return 1;
	}
	if (verbose) {
		printf("target: %lu\n", target);
	}

	len = 1 + (target / 10);
	if (verbose) {
		printf("houses len: %lu\n", (unsigned long)len);
	}

	to_deliver = max50 ? 11 : 10;
	houses = calloc(sizeof(unsigned long), len);
	for (elf = 1; elf < len; ++elf) {
		count = 0;
		for (house = elf; house < len; house += elf) {
			houses[house] += (elf * to_deliver);
			if(max50 && ++count > 50) {
				break;
			}
		}
	}

	found = 0;
	for (house = 1; (!found || verbose > 1) && (house < len); ++house) {
		if (!found && houses[house] > target) {
			found = house;
		}
		if (verbose) {
			printf("House %lu got %lu presents.\n", house,
			       houses[house]);
		}
	}

	printf("%lu\n", found);

	return 0;
}
