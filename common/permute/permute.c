/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* permute: functions related to creating permutations of arrays */
/* Copyright (C) 2015-2021 Eric Herman <eric@freesa.org> */

#include "permute.h"
#include <stdint.h>		/* uint64_t */
#include <stddef.h>		/* size_t */
#include <limits.h>		/* ULONG_MAX */
#include <string.h>		/* memcpy */

void memswap(void *a, void *b, void *tmp, size_t size)
{
	memcpy(tmp, b, size);	/* save b */
	memcpy(b, a, size);	/* a -> b */
	memcpy(a, tmp, size);	/* saved b -> a */
}

#if ((SIZE_MAX >= UINT64_MAX) && (ULONG_MAX >= UINT64_MAX))
#define Zfactorial_cache_max 20
#else
#if ((SIZE_MAX >= UINT32_MAX) && (ULONG_MAX >= UINT32_MAX))
#define Zfactorial_cache_max 12
#else
#define Zfactorial_cache_max 8
#endif
#endif
static const size_t zfactorial_cache[] = {
	/*  0: */ 1UL,
	/*  1: */ 1UL,
	/*  2: */ 2UL,
	/*  3: */ 6UL,
	/*  4: */ 24UL,
	/*  5: */ 120UL,
	/*  6: */ 720UL,
	/*  7: */ 5040UL,
	/*  8: */ 40320UL,
#if (Zfactorial_cache_max > 8)
	/*  9: */ 362880UL,
	/* 10: */ 3628800UL,
	/* 11: */ 39916800UL,
	/* 12: */ 479001600UL,
#endif
#if (Zfactorial_cache_max > 12)
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

size_t zfactorial(size_t n)
{
	uint64_t i, result;

	if (n <= Zfactorial_cache_max) {
		return zfactorial_cache[n];
	}

	/* result would exceed UINT64_MAX */
	if (n > 20) {
		return 0;
	}
	/* e.g.: AVR */
	if ((SIZE_MAX <= UINT16_MAX) && (n > 8)) {
		return 0;
	}
	/* e.g.: x86 */
	if ((SIZE_MAX <= UINT32_MAX) && (n > 12)) {
		return 0;
	}
	/* for case where SIZE_MAX > ULONG_MAX, ULONG_MAX < SIZE_MAX */
	/* e.g.: x64 msvc v19.10 (WINE) (AMD64, 32bit ULONG, 64bit ULL) */
	result = zfactorial_cache[Zfactorial_cache_max];
	for (i = Zfactorial_cache_max + 1; i <= n; ++i) {
		result *= i;
	}
	/* Are there any platforms for which this could be true? */
	if ((SIZE_MAX < UINT64_MAX) && (result > SIZE_MAX)) {
		return 0;
	}
	return result;
}

#undef Zfactorial_cache_max

/* allows caller to fetch a specifically indexed permutation */
void permute(size_t permutation_index, void *dest, const void *src,
	     size_t num_elems, size_t elem_size, void *element_buffer)
{
	size_t i, sub_perm, n_sub_perms, next_swap;
	unsigned char *d;
	void *a, *b;

	memcpy(dest, src, num_elems * elem_size);
	sub_perm = permutation_index;
	d = (unsigned char *)dest;	/* allow ptr math */

	for (i = 0; i < num_elems - 1; ++i) {
		n_sub_perms = zfactorial(num_elems - 1 - i);
		next_swap = sub_perm / n_sub_perms;
		sub_perm = sub_perm % n_sub_perms;
		if (next_swap != 0) {
			a = d + i * elem_size;
			b = d + (i + next_swap) * elem_size;
			memswap(a, b, element_buffer, elem_size);
		}
	}
}
