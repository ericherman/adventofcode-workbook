/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 01-1.c 2019 AdventOfCode solution
   Copyright (C) 2015, 2016, 2019 Eric Herman <eric@freesa.org>
*/
#include <libc-headers.h>
#include <eherr.h>

int main(int argc, char **argv)
{
	const char *path = (argc > 1) ? argv[1] : "input";
	FILE *input = Fopen_or_die(path, "r");

	int previous = INT_MAX;
	int depth = 0;
	int increased = 0;
	int matched = EOF;
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
