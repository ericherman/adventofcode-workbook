/* day10.c 2015 AdventOfCode solution
   Copyright (C) 2015-2016 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>		/* snprintf printf */
#include <stdlib.h>		/* stdlib */
#include <string.h>		/* strlen strcpy */

#define MAKE_VALGRIND_HAPPY 0

unsigned ctoi(char c)
{
	return (unsigned)(c - '0');
}

int main(int argc, char **argv)
{
	const char *input, *verbose;
	char *buf0, *buf1;
	size_t i, j, k, l, buf0_len, buf1_len;
	unsigned m;
	char lc;

	l = (argc > 1) ? atoi(argv[1]) : 40;
	input = (argc > 2) ? argv[2] : "1113222113";
	verbose = (argc > 3) ? argv[3] : NULL;

	buf0 = malloc(strlen(input) + sizeof(void *));

	strcpy(buf0, input);

	for (i = 0; i < l; ++i) {
		buf0_len = strlen(buf0);
		if (verbose) {
			printf("%lu: buf0 (%lu): %s\n", (unsigned long)i,
			       (unsigned long)buf0_len, buf0);
		}
		buf1_len = buf0_len * 10;
		buf1 = malloc(buf1_len + 1);
		k = 0;
		lc = buf0[0];
		m = 0;
		for (j = 0; j < buf0_len; ++j) {
			if (buf0[j] < '0' || buf0[j] > '9') {
				fprintf(stderr, "bad input:%d\n", buf0[j]);
				continue;
			}
			if (buf0[j] == lc) {
				++m;
			} else {
				k += snprintf(&buf1[k], buf1_len - k, "%u%u", m,
					      ctoi(lc));
				m = 1;
			}
			lc = buf0[j];
		}
		k += sprintf(&buf1[k], "%u%u", m, ctoi(lc));
		free(buf0);
		buf0 = buf1;
	}

	if (verbose) {
		printf("%lu: buf0 (%lu): %s\n", (unsigned long)i,
		       (unsigned long)strlen(buf0), buf0);
	}

	printf("%lu\n", (unsigned long)strlen(buf0));

	if (MAKE_VALGRIND_HAPPY) {
		free(buf0);
	}

	return 0;
}
