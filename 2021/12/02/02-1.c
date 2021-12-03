/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 02-1.c 2019 AdventOfCode solution
   Copyright (C) 2015, 2016, 2019, 2021 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	char direction[40];
	int matched, dist, x, z;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	memset(direction, 0x00, 40);
	x = 0;
	z = 0;
	while ((matched = fscanf(input, "%s %d", direction, &dist)) != EOF) {
		if (matched == 2) {
			if (strcmp("forward", direction) == 0) {
				x += dist;
			} else if (strcmp("down", direction) == 0) {
				z += dist;
			} else if (strcmp("up", direction) == 0) {
				z -= dist;
			} else {
				return 1;
			}
		}
	}

	fclose(input);

	printf("%d\n", x * z);

	return 0;
}
