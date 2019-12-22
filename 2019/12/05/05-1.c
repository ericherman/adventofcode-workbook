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
	size_t size;
	int *memory;
	int input, output;

	path = (argc > 1) ? argv[1] : "input";
	size = 0;
	memory = load_ints_from_csv(path, &size);

	input = 1;
	output = INT_MIN;
	run_intcodes(memory, size, get_input, &input, put_output, &output);

	printf("%d\n", output);

	free(memory);

	return 0;
}
