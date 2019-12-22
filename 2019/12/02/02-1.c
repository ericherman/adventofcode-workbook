/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 02-1.c 2019 AdventOfCode solution
   Copyright (C) 2019 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <stdlib.h>		/* malloc */
#include <string.h>		/* memcpy */

#include "intcode-computer.c"

int main(int argc, char **argv)
{
	const char *path;
	int *memory;
	int set_1202_alarm;
	size_t size;

	path = (argc > 1) ? argv[1] : "input";

	set_1202_alarm = (argc > 2) ? atoi(argv[2]) : 1;

	size = 0;
	memory = load_ints_from_csv(path, &size);

	if (set_1202_alarm) {
		memory[1] = 12;
		memory[2] = 2;
	}

	run_intcodes(memory, size, NULL, NULL, NULL, NULL);

	printf("memory[0]=\n");
	printf("%d\n", memory[0]);

	free(memory);

	return 0;
}
