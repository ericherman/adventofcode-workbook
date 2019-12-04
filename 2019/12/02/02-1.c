/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 02-1.c 2019 AdventOfCode solution
   Copyright (C) 2019 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <stdlib.h>		/* malloc */
#include <string.h>		/* memcpy */

void run_intcodes(int *memory, size_t size)
{
	size_t pos = 0;
	int halt = 0;

	while (pos < size && !halt) {
		switch (memory[pos]) {
		case 99:
			halt = 1;
			break;
		case 1:
			memory[memory[pos + 3]] =
			    memory[memory[pos + 1]] + memory[memory[pos + 2]];
			break;
		case 2:
			memory[memory[pos + 3]] =
			    memory[memory[pos + 1]] * memory[memory[pos + 2]];
			break;
		default:
			fprintf(stderr, "unsupported opcoded: %d at %lu\n",
				memory[pos], pos);
			break;
		}
		if (!halt) {
			pos += 4;
		}
	}
}

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	int *orig_mem, *memory, *new_mem;
	int matched, val, set_1202_alarm;
	size_t size, used;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	set_1202_alarm = (argc > 2) ? atoi(argv[2]) : 1;

	size = 255;
	memory = malloc(size * sizeof(int));
	if (!memory) {
		return EXIT_FAILURE;
	}

	used = 0;
	while ((matched = fscanf(input, "%d", &val)) != EOF) {
		if (matched) {
			memory[used++] = val;
			if (used == size) {
				size *= 2;
				new_mem = realloc(memory, size * sizeof(int));
				if (!new_mem) {
					return EXIT_FAILURE;
				}
				memory = new_mem;
			}
		}
	}
	fclose(input);

	orig_mem = memory;
	new_mem = malloc(size * sizeof(int));
	memcpy(memory, orig_mem, size * sizeof(int));

	if (set_1202_alarm) {
		memory[1] = 12;
		memory[2] = 2;
	}

	run_intcodes(memory, used);

	printf("memory[0]=\n");
	printf("%d\n", memory[0]);

	return 0;
}
