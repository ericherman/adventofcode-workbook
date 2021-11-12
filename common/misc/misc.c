/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* misc.c: a few handy functions */
/* Copyright (C) 2018, 2019 Eric Herman <eric@freesa.org> */

#include "misc.h"
#include <stdint.h>		/* uint64_t */
#include <stddef.h>		/* size_t */
#include <limits.h>		/* ULONG_MAX */
#include <string.h>		/* memcpy */

void swap(void *a, void *b, void *buf, size_t element_size)
{

	memcpy(buf, b, element_size);	/* save b */
	memcpy(b, a, element_size);	/* a -> b */
	memcpy(a, buf, element_size);	/* saved b -> a */
}

#if ((SIZE_MAX >= UINT64_MAX) && (ULONG_MAX >= UINT64_MAX))
#define Factorial_cache_max 20
#else
#if ((SIZE_MAX >= UINT32_MAX) && (ULONG_MAX >= UINT32_MAX))
#define Factorial_cache_max 12
#else
#define Factorial_cache_max 8
#endif
#endif
static const size_t factorial_cache[] = {
	/*  0: */ 1UL,
	/*  1: */ 1UL,
	/*  2: */ 2UL,
	/*  3: */ 6UL,
	/*  4: */ 24UL,
	/*  5: */ 120UL,
	/*  6: */ 720UL,
	/*  7: */ 5040UL,
	/*  8: */ 40320UL,
	#if (Factorial_cache_max > 8)
	/*  9: */ 362880UL,
	/* 10: */ 3628800UL,
	/* 11: */ 39916800UL,
	/* 12: */ 479001600UL,
	#endif
	#if (Factorial_cache_max > 12)
	/* 13: */ 6227020800UL,
	/* 14: */ 87178291200UL,
	/* 15: */ 1307674368000UL,
	/* 16: */ 20922789888000UL,
	/* 17: */ 355687428096000UL,
	/* 18: */ 6402373705728000UL,
	/* 19: */ 121645100408832000UL,
	/* 20: */ 2432902008176640000UL,
	#endif
	/* NUL */ 0 /* value would exceed ULONG_MAX or SIZE_MAX */
};
size_t factorial(size_t n)
{
	uint64_t i, result;

	if (n <= Factorial_cache_max) {
		return factorial_cache[n];
	}

	/* result would exceed UINT64_MAX */
	if (n > 20) {
		return 0;
	}

	result = factorial_cache[Factorial_cache_max];
	for (i = Factorial_cache_max + 1; i <= n; ++i) {
		result *= i;
	}
	return (result <= SIZE_MAX) ? result : 0;
}
#undef Factorial_cache_max

/* allows caller to fetch a specifically indexed permutation */
void permute(size_t permutation, const void *src, void *dest, size_t len,
	     void *buf, size_t elem_size)
{
	size_t i, sub_perm, n_sub_perms, next_swap;
	unsigned char *d;
	void *a, *b;

	memcpy(dest, src, len * elem_size);
	sub_perm = permutation;
	d = (unsigned char *)dest;	/* allow ptr math */

	for (i = 0; i < len - 1; ++i) {
		n_sub_perms = factorial(len - 1 - i);
		next_swap = sub_perm / n_sub_perms;
		sub_perm = sub_perm % n_sub_perms;
		if (next_swap != 0) {
			a = d + i * elem_size;
			b = d + (i + next_swap) * elem_size;
			swap(a, b, buf, elem_size);
		}
	}
}

