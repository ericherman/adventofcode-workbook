/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 07-2.c 2019 AdventOfCode solution */
/* Copyright (C) 2019 Eric Herman <eric@freesa.org> */

#include <misc.h>
#include <pthread.h>
#include <sched.h>		/* sched_yield */
#include <unistd.h>		/* sleep */

#include "intcode-computer.c"

struct exec_context_s {
	char id;
	pthread_mutex_t lock;
	int *memory;
	size_t mem_len;
	int phase_retreived;
	int phase;
	int atomic_have_input;
	int input;
	struct exec_context_s *output_ctx;
};

static int get_in(void *input_context)
{
	struct exec_context_s *ctx;
	int input;

	ctx = input_context;

	if (!ctx->phase_retreived) {
		ctx->phase_retreived = 1;
		return ctx->phase;
	}

	while (!ctx->atomic_have_input) {
		sched_yield();
	}

	pthread_mutex_lock(&(ctx->lock));
	if (!ctx->atomic_have_input) {
		exit(EXIT_FAILURE);
	}
	ctx->atomic_have_input = 0;
	input = ctx->input;
	pthread_mutex_unlock(&(ctx->lock));

	return input;
}

static void put_out(void *output_context, int x)
{
	struct exec_context_s *ctx, *in;
	in = output_context;
	ctx = in->output_ctx;

	while (ctx->atomic_have_input) {
		/* printf("%c already has input\n", ctx->id); */
		sched_yield();
	}
	pthread_mutex_lock(&(ctx->lock));
	if (ctx->atomic_have_input) {
		exit(EXIT_FAILURE);
	}
	ctx->atomic_have_input = 1;
	ctx->input = x;
	pthread_mutex_unlock(&(ctx->lock));
	sched_yield();
}

void *start_routine_run_intcodes(void *context)
{
	struct exec_context_s *ctx;

	ctx = context;
	run_intcodes(ctx->memory, ctx->mem_len, get_in, ctx, put_out, ctx);

	return NULL;
}

int main(int argc, char **argv)
{
	const char *path;
	int *orig_mem, *amps_orig, *amps_temp;
	size_t i, j, size, amps_len, combos, swap;
	struct exec_context_s *exec_ctxs;
	pthread_t *threads;
	pthread_attr_t *attr;
	void *res;
	int err, output, max_out;

	path = (argc > 1) ? argv[1] : "input";
	size = 0;
	orig_mem = load_ints_from_csv(path, &size);

	amps_len = 5;
	amps_orig = calloc(amps_len, sizeof(int));
	amps_temp = calloc(amps_len, sizeof(int));
	for (i = 0; i < amps_len; ++i) {
		amps_orig[i] = (int)(i + 5);
	}
	max_out = 0;
	combos = factorial(amps_len);
	for (i = 0; i < combos; ++i) {
		permute(i, amps_orig, amps_temp, amps_len, &swap, sizeof(int));

		threads = calloc(amps_len, sizeof(pthread_t));
		if (!threads) {
			exit(EXIT_FAILURE);
		}

		exec_ctxs = calloc(amps_len, sizeof(struct exec_context_s));
		if (!exec_ctxs) {
			exit(EXIT_FAILURE);
		}
		for (j = 0; j < amps_len; ++j) {
			exec_ctxs[j].id = 'A' + j;
			if (j + 1 == amps_len) {
				exec_ctxs[j].output_ctx = &(exec_ctxs[0]);
			} else {
				exec_ctxs[j].output_ctx = &(exec_ctxs[j + 1]);
			}

			exec_ctxs[j].mem_len = size;
			exec_ctxs[j].memory = calloc(size, sizeof(int));
			if (!exec_ctxs[j].memory) {
				exit(EXIT_FAILURE);
			}
			memcpy(exec_ctxs[j].memory, orig_mem,
			       size * sizeof(int));
			err = pthread_mutex_init(&(exec_ctxs[j].lock), NULL);
			if (err) {
				exit(EXIT_FAILURE);
			}
		}

		for (j = 0; j < amps_len; ++j) {
			exec_ctxs[j].phase = amps_temp[j];
		}
		exec_ctxs[0].atomic_have_input = 1;
		exec_ctxs[0].input = 0;

		for (j = 0; j < amps_len; ++j) {
			attr = NULL;
			err = pthread_create(&(threads[j]), attr,
					     start_routine_run_intcodes,
					     &(exec_ctxs[j]));
			if (err) {
				exit(EXIT_FAILURE);
			}
		}

		for (j = 0; j < amps_len; ++j) {
			res = NULL;
			pthread_join(threads[j], &res);
			free(res);
			res = NULL;
		}
		free(threads);

		if (!exec_ctxs[0].atomic_have_input) {
			exit(EXIT_FAILURE);
		}
		output = exec_ctxs[0].input;
		if (output > max_out) {
			max_out = output;
		}

		for (j = 0; j < amps_len; ++j) {
			free(exec_ctxs[j].memory);
			pthread_mutex_destroy(&(exec_ctxs[j].lock));
		}
		free(exec_ctxs);
	}

	printf("%d\n", max_out);

	free(amps_temp);
	free(amps_orig);
	free(orig_mem);

	return 0;
}
