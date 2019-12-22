/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 05-2.c 2019 AdventOfCode solution
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

static void put_output(void *output_context, int x)
{
	*((int *)output_context) = x;
}

int main(int argc, char **argv)
{
	const char *path;
	size_t size;
	int input, output;
	int *memory;

	path = (argc > 1) ? argv[1] : "input";

	size = 0;
	memory = load_ints_from_csv(path, &size);

	input = 5;
	output = 0;
	run_intcodes(memory, size, get_input, &input, put_output, &output);
	printf("%d\n", output);

	free(memory);

	return 0;
}
