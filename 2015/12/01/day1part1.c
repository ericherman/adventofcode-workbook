/* SPDX-License-Identifier: GPL-3.0-or-later */
/* day1part1.c 2015 AdventOfCode solution
   Copyright (C) 2015, 2016, 2019 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	int floor, c;

	if (argc > 1) {
		path = argv[1];
	}

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	floor = 0;
	while ((c = fgetc(input)) != EOF) {
		switch (c) {
		case '(':
			++floor;
			break;
		case ')':
			--floor;
			break;
		default:	/*ignore */
			break;
		}
	}

	fclose(input);

	printf("floor=\n");
	printf("%d\n", floor);

	return 0;
}
