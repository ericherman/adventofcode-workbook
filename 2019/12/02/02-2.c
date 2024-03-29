/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 02-2.c 2019 AdventOfCode solution
   Copyright (C) 2019 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <stdlib.h>		/* malloc */
#include <string.h>		/* memcpy */

#include <intcode-computer.h>

int main(int argc, char **argv)
{
	const char *path;
	intcode_cpu_s *orig, *cpu;
	int n, v;

	path = (argc > 1) ? argv[1] : "input";
	orig = intcode_new_from_csv(path);

	for (n = 0; n <= 99; ++n) {
		for (v = 0; v <= 99; ++v) {
			cpu = orig->copy(orig);
			cpu->poke(cpu, 1, n);
			cpu->poke(cpu, 2, v);
			cpu->run(cpu, NULL, NULL, NULL, NULL);
			if (cpu->peek(cpu, 0) == 19690720) {
				printf("%d%02d\n", n, v);
			}
			cpu->free(&cpu);
		}
	}

	orig->free(&orig);

	return 0;
}
