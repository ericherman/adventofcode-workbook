/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 07-1.c 2019 AdventOfCode solution */
/* Copyright (C) 2019-2021 Eric Herman <eric@freesa.org> */

#include <libc-headers.h>
#include <permute.h>

#include <intcode-computer.h>

struct two_inputs_context_s {
	size_t cnt;
	int64_t inputs[2];
};

static int64_t get_in(void *input_context)
{
	struct two_inputs_context_s *ctx;

	ctx = input_context;

	if (ctx->cnt > 1) {
		exit(EXIT_FAILURE);
	}
	return ctx->inputs[ctx->cnt++];
}

static void put_out(void *output_context, int64_t x)
{
	*((int64_t *)output_context) = x;
}

int main(int argc, char **argv)
{
	const char *path;
	intcode_cpu_s *orig, *cpu;
	int *amps_orig, *amps_temp;
	size_t i, j, amps_len, combos, swap;
	struct two_inputs_context_s input_ctx;
	int64_t output, max_out;

	path = (argc > 1) ? argv[1] : "input";
	orig = intcode_new_from_csv(path);

	amps_len = 5;
	amps_orig = calloc(amps_len, sizeof(int));
	amps_temp = calloc(amps_len, sizeof(int));
	for (i = 0; i < amps_len; ++i) {
		amps_orig[i] = (int)i;
	}

	max_out = 0;
	combos = zfactorial(amps_len);
	for (i = 0; i < combos; ++i) {
		permute(i, amps_temp, amps_orig, amps_len, sizeof(int), &swap);

		output = 0;
		for (j = 0; j < amps_len; ++j) {
			input_ctx.cnt = 0;
			input_ctx.inputs[0] = (int64_t)amps_temp[j];
			input_ctx.inputs[1] = output;
			cpu = orig->copy(orig);
			cpu->run(cpu, get_in, &input_ctx, put_out, &output);
			cpu->free(&cpu);
			if (output > max_out) {
				max_out = output;
			}
		}
	}

	printf("%" PRIi64 "\n", max_out);

	free(amps_temp);
	free(amps_orig);
	orig->free(&orig);

	return 0;
}
