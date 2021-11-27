/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* permute: functions related to creating permutations of arrays */
/* Copyright (C) 2015-2021 Eric Herman <eric@freesa.org> */

#ifndef PERMUTE_H
#define PERMUTE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>		/* size_t */

void swap(void *a, void *b, void *buf, size_t element_size);

size_t zfactorial(size_t n);

/* allows caller to fetch a specifically indexed permutation */
/* src array and dest array are assumed to be of size (len * elem_size) */
/* the buf is assumed to be of at least elem_size */
/* the dest buf will be filled with a permutation of the values in src */
void permute(size_t permutation_index, const void *src, void *dest, size_t len,
	     void *buf, size_t elem_size);

#ifdef __cplusplus
}
#endif
#endif				/* PERMUTE_H */
