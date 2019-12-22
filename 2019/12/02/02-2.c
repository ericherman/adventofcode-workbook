/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 02-2.c 2019 AdventOfCode solution
   Copyright (C) 2019 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <stdlib.h>		/* malloc */
#include <string.h>		/* memcpy */

#include "intcode-computer.c"

int main(int argc, char **argv)
{
	const char *path;
	int *orig_mem, *memory;
	int n, v;
	size_t size;

	path = (argc > 1) ? argv[1] : "input";
	size = 0;
	orig_mem = load_ints_from_csv(path, &size);

	memory = malloc(size * sizeof(int));

	for (n = 0; n <= 99; ++n) {
		for (v = 0; v <= 99; ++v) {
			memcpy(memory, orig_mem, size * sizeof(int));
			memory[1] = n;
			memory[2] = v;
			run_intcodes(memory, size, NULL, NULL, NULL, NULL);
			if (memory[0] == 19690720) {
				printf("%d%02d\n", n, v);
			}
		}
	}

	free(memory);
	free(orig_mem);

	return 0;
}
