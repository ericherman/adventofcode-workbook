/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 02-2.c 2019 AdventOfCode solution
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
	int n, v, matched, val;
	size_t size, used;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

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
	memory = malloc(size * sizeof(int));

	for (n = 0; n <= 99; ++n) {
		for (v = 0; v <= 99; ++v) {
			memcpy(memory, orig_mem, size * sizeof(int));
			memory[1] = n;
			memory[2] = v;
			run_intcodes(memory, used);
			if (memory[0] == 19690720) {
				printf("%d%02d\n", n, v);
			}
		}
	}

	return 0;
}
