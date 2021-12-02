/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 01-1.c 2019 AdventOfCode solution
   Copyright (C) 2015, 2016, 2019 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <string.h>		/* strlen */

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	int matched, increased, depth, previous, current;
	int depths[4];
	size_t i;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	increased = 0;
	for (i = 0; (matched = fscanf(input, "%d", &depth)) != EOF; ++i) {
		if (matched) {
			depths[i % 4] = depth;
			if (i >= 3) {
				current = depths[i % 4]
				    + depths[(i - 1) % 4]
				    + depths[(i - 2) % 4];

				previous = depths[(i - 1) % 4]
				    + depths[(i - 2) % 4]
				    + depths[(i - 3) % 4];

				if (current > previous) {
					++increased;
				}
			}
		}
	}

	fclose(input);

	printf("%d\n", increased);

	return 0;
}
