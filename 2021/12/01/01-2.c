/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 01-2.c 2019 AdventOfCode solution */
/* Copyright (C) 2021 Eric Herman <eric@freesa.org> */

#include <libc-headers.h>

#include <eherr.h>

int main(int argc, char **argv)
{
	const char *path = (argc > 1) ? argv[1] : "input";

	int depths[4] = { 0, 0, 0, 0 };
	int matched = 0;
	int increased = 0;
	int depth = 0;

	FILE *input = Fopen_or_die(path, "r");
	for (size_t i = 0; (matched = fscanf(input, "%d", &depth)) != EOF; ++i) {
		if (matched) {
			depths[i % 4] = depth;
			if (i >= 3) {
				int current = depths[i % 4]
				    + depths[(i - 1) % 4]
				    + depths[(i - 2) % 4];

				int previous = depths[(i - 1) % 4]
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
