/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 05-1.c 2019 AdventOfCode solution
   Copyright (C) 2019 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <stdlib.h>		/* malloc */
#include <string.h>		/* memcpy */

#include "intcode-computer.c"

static int get_input(void *input_context)
{
	return *((int *)input_context);
}

static void put_output(void *ctx, int x)
{
	*((int *)ctx) = x;
}

int main(int argc, char **argv)
{
	const char *path;
	struct intcode_cpu_s *cpu;
	int input, output;

	path = (argc > 1) ? argv[1] : "input";
	cpu = intcode_new_from_csv(path);

	input = 1;
	output = INT_MIN;

	cpu->run(cpu, get_input, &input, put_output, &output);

	printf("%d\n", output);

	cpu->free(&cpu);

	return 0;
}
