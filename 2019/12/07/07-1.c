/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 07-1.c 2019 AdventOfCode solution */
/* Copyright (C) 2019 Eric Herman <eric@freesa.org> */

#include <misc.h>

#include "intcode-computer.c"

struct two_inputs_context_s {
	size_t cnt;
	int inputs[2];
};

static int get_in(void *input_context)
{
	struct two_inputs_context_s *ctx;

	ctx = input_context;

	if (ctx->cnt > 1) {
		exit(EXIT_FAILURE);
	}
	return ctx->inputs[ctx->cnt++];
}

static void put_out(void *output_context, int x)
{
	*((int *)output_context) = x;
}

int main(int argc, char **argv)
{
	const char *path;
	int *orig_mem, *memory, *amps_orig, *amps_temp;
	size_t i, j, size, amps_len, combos, swap;
	struct two_inputs_context_s input_ctx;
	int output, max_out;

	path = (argc > 1) ? argv[1] : "input";
	size = 0;
	orig_mem = load_ints_from_csv(path, &size);
	memory = malloc(size * sizeof(int));

	amps_len = 5;
	amps_orig = calloc(amps_len, sizeof(int));
	amps_temp = calloc(amps_len, sizeof(int));
	for (i = 0; i < amps_len; ++i) {
		amps_orig[i] = (int)i;
	}

	max_out = 0;
	combos = factorial(amps_len);
	for (i = 0; i < combos; ++i) {
		permute(i, amps_orig, amps_temp, amps_len, &swap, sizeof(int));

		memcpy(memory, orig_mem, size * sizeof(int));

		output = 0;
		for (j = 0; j < amps_len; ++j) {
			input_ctx.cnt = 0;
			input_ctx.inputs[0] = (int)amps_temp[j];
			input_ctx.inputs[1] = output;
			run_intcodes(memory, size,
				     get_in, &input_ctx, put_out, &output);

			if (output > max_out) {
				max_out = output;
			}
		}
	}

	printf("%d\n", max_out);

	free(amps_temp);
	free(amps_orig);
	free(memory);
	free(orig_mem);

	return 0;
}
