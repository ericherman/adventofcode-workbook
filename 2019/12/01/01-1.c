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
	int matched, mass, fuel, total_fuel;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	total_fuel = 0;
	while ((matched = fscanf(input, "%d", &mass)) != EOF) {
		if (matched) {
			fuel = (mass / 3) - 2;
			total_fuel += fuel;
		}
	}

	fclose(input);

	printf("total=\n");
	printf("%d\n", total_fuel);

	return 0;
}
