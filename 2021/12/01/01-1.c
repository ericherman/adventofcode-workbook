/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 01-1.c 2019 AdventOfCode solution
   Copyright (C) 2015, 2016, 2019 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <string.h>		/* strlen */
#include <limits.h>		/* INT_MAX */

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	int matched, depth, increased, previous;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	previous = INT_MAX;
	depth = 0;
	increased = 0;
	while ((matched = fscanf(input, "%d", &depth)) != EOF) {
		if (matched) {
			if (previous < depth) {
				++increased;
			}
			previous = depth;
		}
	}

	fclose(input);

	printf("%d\n", increased);

	return 0;
}
