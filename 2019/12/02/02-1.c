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
	struct intcode_cpu_s *cpu;
	int set_1202_alarm;

	path = (argc > 1) ? argv[1] : "input";

	set_1202_alarm = (argc > 2) ? atoi(argv[2]) : 1;

	cpu = intcode_new_from_csv(path);

	if (set_1202_alarm) {
		cpu->poke(cpu, 1, 12);
		cpu->poke(cpu, 2, 2);
	}

	cpu->run(cpu, NULL, NULL, NULL, NULL);

	printf("memory[0]=\n");
	printf("%lld\n", (long long)cpu->peek(cpu, 0));

	cpu->free(&cpu);

	return 0;
}
