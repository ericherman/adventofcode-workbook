/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* permute: functions related to creating permutations of arrays */
/* Copyright (C) 2015-2021 Eric Herman <eric@freesa.org> */

#ifndef PERMUTE_H
#define PERMUTE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>		/* size_t */

void memswap(void *a, void *b, void *tmp, size_t size);

size_t zfactorial(size_t n);

/* allows caller to fetch a specifically indexed permutation */
/* dest and src arrays are assumed to be of size (num_elems * elem_size) */
/* the element_bufffer is assumed to be of at least elem_size */
/* the dest buf will be filled with a permutation of the values in src */
void permute(size_t permutation_index, void *dest, const void *src,
	     size_t num_elems, size_t elem_size, void *element_buffer);

#ifdef __cplusplus
}
#endif
#endif				/* PERMUTE_H */
