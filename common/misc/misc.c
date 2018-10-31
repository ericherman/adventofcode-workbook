/*
misc.c: a few handy functions
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

#include "misc.h"
#include <string.h>		/* memcpy */

void swap(void *a, void *b, void *buf, size_t element_size)
{

	memcpy(buf, b, element_size);	/* save b */
	memcpy(b, a, element_size);	/* a -> b */
	memcpy(a, buf, element_size);	/* saved b -> a */
}

size_t factorial(size_t n)
{
	size_t result = n;
	if (n < 2) {
		return 1;
	}
	do {
		result *= --n;
	} while (n > 1);
	return result;
}

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

