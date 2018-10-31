/*
misc: a few handy functions
Copyright (C) 2018 Eric Herman <eric@freesa.org>

This work is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This work is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License and the GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
(COPYING) and the GNU General Public License (COPYING.GPL3).  If not, see
<http://www.gnu.org/licenses/>.
*/

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
