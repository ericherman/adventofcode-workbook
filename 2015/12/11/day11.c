/* day11.c 2015 AdventOfCode solution
   Copyright (C) 2015-2016 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>		/* printf */
#include <stdlib.h>		/* atoi */
#include <string.h>		/* strlen strcpy */

#define PW_LEN 8
#define BUF_LEN (PW_LEN + 1)

static void inc_buf(char *c, size_t len)
{
	size_t i = len;

	while (i) {
		--i;
		c[i]++;
		switch (c[i]) {
		case ('z' + 1):
			c[i] = 'a';
			break;
		case 'i':
		case 'o':
		case 'l':
			c[i]++;
		default:
			return;
		}
	}
}

int contains_3_letter_strait(char *c, size_t len, int print)
{
	size_t i;

	for (i = 0; i < len - 2; ++i) {
		if (((c[i] + 1) == c[i + 1]) && ((c[i] + 2) == c[i + 2])) {
			if (print) {
				printf("%c%c%c\n", c[i], c[i + 1], c[i + 2]);
			}
			return 1;
		}
	}
	return 0;
}

int contains_2_non_overlapping_pairs(char *c, size_t len, int print)
{
	size_t i, pairs;

	pairs = 0;
	for (i = 0; i < len - 1; ++i) {
		if (c[i] == c[i + 1]) {
			++pairs;
			if (print) {
				printf("%c%c", c[i], c[i + 1]);
			}
			if (pairs == 2) {
				if (print) {
					printf("\n");
				}
				return 1;
			}
			++i;
		}
	}
	if (pairs && print) {
		printf("\n");
	}
	return 0;
}

int main(int argc, char **argv)
{
	const char *input;
	char buf[BUF_LEN];
	size_t len, verbose;

	verbose = (argc > 1) ? atoi(argv[1]) : 0;
	input = (argc > 2) ? argv[2] : "cqjxjnds";

	len = strlen(input);
	if (len > PW_LEN) {
		fprintf(stderr, "input too long\n");
		return 1;
	} else if (len == 0) {
		fprintf(stderr, "empty string\n");
		return 1;
	}

	strcpy(buf, input);
	if (verbose) {
		printf("%s\n", buf);
	}

	for (;;) {
		inc_buf(buf, len);
		if (verbose) {
			printf("%s\n", buf);
		}
		if (contains_3_letter_strait(buf, len, verbose)
		    && contains_2_non_overlapping_pairs(buf, len, verbose)) {
			break;
		}
	}

	printf("%s\n", buf);
	contains_3_letter_strait(buf, len, 1);
	contains_2_non_overlapping_pairs(buf, len, 1);
	return 0;
}
