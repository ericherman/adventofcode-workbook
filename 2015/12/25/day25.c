/* day25.c 2015 AdventOfCode solution
   Copyright (C) 2015-2016 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ehbigint.h>

#define DIE(format, arg) \
  fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
  fprintf(stderr, format, arg); \
  exit(EXIT_FAILURE)

#define DIE2(format, arg1, arg2) \
  fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
  fprintf(stderr, format, arg1, arg2); \
  exit(EXIT_FAILURE)

#define DIE3(format, arg1, arg2, arg3) \
  fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
  fprintf(stderr, format, arg1, arg2, arg3); \
  exit(EXIT_FAILURE)

#define DIE4(format, arg1, arg2, arg3, arg4) \
  fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
  fprintf(stderr, format, arg1, arg2, arg3, arg4); \
  exit(EXIT_FAILURE)

#ifdef __STDC_VERSION__
#if __STDC_VERSION__ >= 199901L
#define ULONGEST unsigned long long
#define ULONGESTFMT "lld"
#define DEFAULT_VERBOSE 0
#define SPEED 4096
#endif
#endif
#ifndef ULONGEST
#define ULONGEST unsigned long
#define ULONGESTFMT "ld"
#define DEFAULT_VERBOSE 1
#define SPEED 16
#endif

#define BILEN 10
struct context_s {
	ULONGEST inmul;
	ULONGEST inmod;
	struct ehbigint in;
	unsigned char in_bytes[BILEN];
	struct ehbigint mul;
	unsigned char mul_bytes[BILEN];
	struct ehbigint res;
	unsigned char res_bytes[BILEN];
	struct ehbigint mod;
	unsigned char mod_bytes[BILEN];
	struct ehbigint quot;
	unsigned char quot_bytes[BILEN];
	struct ehbigint rem;
	unsigned char rem_bytes[BILEN];
	int verbose;
};

static void init_context(struct context_s *ctx, ULONGEST inmul, ULONGEST inmod,
			 int verbose)
{
	ctx->inmul = inmul;
	ctx->inmod = inmod;

	ctx->mul.bytes = ctx->mul_bytes;
	ctx->mod.bytes = ctx->mod_bytes;
	ctx->quot.bytes = ctx->quot_bytes;
	ctx->rem.bytes = ctx->rem_bytes;
	ctx->in.bytes = ctx->in_bytes;
	ctx->res.bytes = ctx->res_bytes;

	ctx->mul.bytes_len = BILEN;
	ctx->mod.bytes_len = BILEN;
	ctx->quot.bytes_len = BILEN;
	ctx->rem.bytes_len = BILEN;
	ctx->in.bytes_len = BILEN;
	ctx->res.bytes_len = BILEN;

	ctx->mul.bytes_used = 0;
	ctx->mod.bytes_used = 0;
	ctx->quot.bytes_used = 0;
	ctx->rem.bytes_used = 0;
	ctx->in.bytes_used = 0;
	ctx->res.bytes_used = 0;

	ehbi_set_l(&ctx->in, 0);
	ehbi_set_l(&ctx->mul, ctx->inmul);
	ehbi_set_l(&ctx->res, 0);
	ehbi_set_l(&ctx->mod, ctx->inmod);
	ehbi_set_l(&ctx->quot, 0);
	ehbi_set_l(&ctx->rem, 0);

	ctx->verbose = verbose;
}

static ULONGEST mul_mod(struct context_s *ctx, size_t i, ULONGEST x)
{
	ULONGEST result;
	int err, base;
	char dec[80];
	char *endptr;

	result = x * ctx->inmul;
	if ((x != 0) && ((result / x) == ctx->inmul)) {
		result = result % ctx->inmod;
		return result;
	}

	if (ctx->verbose > 1) {
		fprintf(stderr,
			"mul_mod((%10lu): %10" ULONGESTFMT ", %10" ULONGESTFMT
			", %10" ULONGESTFMT ")\n", (unsigned long)i, x,
			ctx->inmul, ctx->inmod);
	}

	err = ehbi_set_l(&ctx->in, x);
	if (err) {
		DIE("ehbi_set_ul: %d\n", err);
	}

	if (ctx->verbose > 3) {
		printf("mul start\n");
	}
	err = ehbi_mul(&ctx->res, &ctx->in, &ctx->mul);
	if (ctx->verbose > 3) {
		printf("mul done\n");
	}
	if (err) {
		DIE("ehbi_mul: %d\n", err);
	}
	if (ctx->verbose > 2) {
		ehbi_to_decimal_string(&ctx->in, dec, 80, &err);
		if (err) {
			DIE3("%d from ehbi_to_decimal_string(%p, %p, 80)\n",
			     err, (void *)&ctx->in, (void *)dec);
		}
		printf("%s", dec);
		ehbi_to_decimal_string(&ctx->mul, dec, 80, &err);
		if (err) {
			DIE3("%d from ehbi_to_decimal_string(%p, %p, 80)\n",
			     err, (void *)&ctx->mul, (void *)dec);
		}
		printf(" * %s = ", dec);
		ehbi_to_decimal_string(&ctx->res, dec, 80, &err);
		if (err) {
			DIE3("%d from ehbi_to_decimal_string(%p, %p, 80)\n",
			     err, (void *)&ctx->res, (void *)dec);
		}
		printf("%s\n", dec);
		printf("ehbi_div(&cxt->quot, &ctx->rem, %s,", dec);
		ehbi_to_decimal_string(&ctx->mod, dec, 80, &err);
		if (err) {
			DIE3("%d from ehbi_to_decimal_string(%p, %p, 80)\n",
			     err, (void *)&ctx->mod, (void *)dec);
		}
		printf("%s)\n", dec);
	}

	err = ehbi_div(&ctx->quot, &ctx->rem, &ctx->res, &ctx->mod);
	if (ctx->verbose > 3) {
		printf("done\n");
	}
	if (err) {
		DIE("ehbi_div: %d\n", err);
	}
	if ((ctx->rem.bytes_used > (1 + sizeof(unsigned long)))
	    || ((ctx->rem.bytes_used == (1 + sizeof(unsigned long)))
		&& (ctx->rem.bytes[0] != 0x00))) {
		DIE("bx.bytes_used > sizeof(unsigned long)(%lu)\n",
		    (unsigned long)ctx->rem.bytes_used);
	}

	ehbi_to_decimal_string(&ctx->rem, dec, 80, &err);
	if (err) {
		DIE("%d from ehbi_to_decimal_string\n", err);
	}

	base = 10;
	errno = 0;
	result = strtoul(dec, &endptr, base);
	err = errno;
	if (err || (endptr && strlen(endptr) > 0)) {
		DIE4("strtoul('%s'); (invalid:'%s') (%d: %s)\n", dec, endptr,
		     err, strerror(err));
	}

	return result;
}

static size_t find_pos(size_t col, size_t row)
{
	size_t x, y, maxy, i;
	for (maxy = 1, x = 1, y = 1, i = 1; x < col || y < row; ++i) {
		if (y == 1) {
			x = 1;
			maxy++;
			y = maxy;
		} else {
			++x;
			--y;
		}
	}
	return i;
}

int main(int argc, char **argv)
{
	int verbose;
	size_t row, col, x, y, i, pos;
	ULONGEST first, next;
	struct context_s ctx;

	verbose = (argc > 1) ? atoi(argv[1]) : DEFAULT_VERBOSE;
	row = (argc > 2) ? atoi(argv[2]) : 0;
	col = (argc > 3) ? atoi(argv[3]) : 0;

	init_context(&ctx, 252533, 33554393, verbose);

	if (row == 0) {
		row = 3010;
	}
	if (col == 0) {
		col = 3019;
	}

	pos = find_pos(col, row);

	first = 20151125;

	next = first;

	if (ctx.verbose) {
		printf("row: %lu, col: %lu\n", (unsigned long)row,
		       (unsigned long)col);
		printf("pos:%lu\n", (unsigned long)pos);
	}
	if (ctx.verbose) {
		printf("   |");
		for (x = 1; x <= 6; ++x) {
			printf("    %lu     ", (unsigned long)x);
		}
		printf("\n");
		printf("---+");
		for (x = 1; x <= 6; ++x) {
			printf("---------+");
		}
		printf("\n");
		fflush(stdout);
		for (y = 1; y <= 6; ++y) {
			printf(" %lu |", (unsigned long)y);
			for (x = 1; x <= 6; ++x) {
				pos = find_pos(x, y);
				next = first;
				for (i = 1; i < pos; ++i) {
					next = mul_mod(&ctx, i, next);
				}
				printf("%9" ULONGESTFMT " ", next);
			}
			printf("\n");
			fflush(stdout);
		}
	}
	fflush(stdout);
	pos = find_pos(col, row);
	next = first;
	for (i = 1; i < pos; ++i) {
		next = mul_mod(&ctx, i, next);
		if ((ctx.verbose == 1) && ((i % SPEED) == 0)) {
			printf("\r%10lu of %lu ", (unsigned long)i,
			       (unsigned long)pos);
			fflush(stdout);
		}
	}
	if (ctx.verbose) {
		printf("\n");
	}

	printf("%" ULONGESTFMT "\n", next);

	return 0;
}
