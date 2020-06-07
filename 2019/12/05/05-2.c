/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 05-2.c 2019 AdventOfCode solution
   Copyright (C) 2019 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <stdlib.h>		/* malloc */
#include <string.h>		/* memcpy */

#include "intcode-computer.c"

static int64_t get_input(void *input_context)
{
	return *((int64_t *)input_context);
}

static void put_output(void *output_context, int64_t x)
{
	*((int64_t *)output_context) = x;
}

int main(int argc, char **argv)
{
	const char *path;
	int64_t input, output;
	intcode_cpu_s *cpu;

	path = (argc > 1) ? argv[1] : "input";

	cpu = intcode_new_from_csv(path);

	input = 5;
	output = 0;

	cpu->run(cpu, get_input, &input, put_output, &output);

	printf("%" PRIi64 "\n", output);

	cpu->free(&cpu);

	return 0;
}
