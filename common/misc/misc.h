/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* misc: a few handy functions */
/* Copyright (C) 2018, 2019 Eric Herman <eric@freesa.org> */

#ifndef MISC_H
#define MISC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>	/* size_t */

void swap(void *a, void *b, void *buf, size_t element_size);

size_t factorial(size_t n);

/* allows caller to fetch a specifically indexed permutation */
void permute(size_t permutation, const void *src, void *dest, size_t len,
	     void *buf, size_t elem_size);

#ifdef __cplusplus
}
#endif

#endif				/* MISC_H */
