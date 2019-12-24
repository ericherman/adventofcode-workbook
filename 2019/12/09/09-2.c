/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 09-2.c 2019 AdventOfCode solution */
/* Copyright (C) 2019 Eric Herman <eric@freesa.org> */

#include <stdio.h>
#include <stdlib.h>		/* malloc */
#include <string.h>		/* memcpy */

#include "intcode-computer.c"

static int64_t get_input(void *input_context)
{
	return *((int64_t *)input_context);
}

static void put_output(void *ctx, int64_t x)
{
	*((int64_t *)ctx) = x;
	printf("%lld\n", (long long)x);
}

int main(int argc, char **argv)
{
	const char *path;
	struct intcode_cpu_s *cpu;
	int64_t input, output;

	path = (argc > 1) ? argv[1] : "input";
	cpu = intcode_new_from_csv(path);

	input = 2;
	output = INT_MIN;

	cpu->run(cpu, get_input, &input, put_output, &output);

	cpu->free(&cpu);

	return 0;
}
