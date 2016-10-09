/* day2part1.c 2015 AdventOfCode solution
   Copyright (C) 2015-2016 Eric Herman <eric@freesa.org>

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
	unsigned int l, w, h, area, smallest, total;
	int matched;

	if (argc > 1) {
		path = argv[1];
	}

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	l = w = h = area = smallest = 0;
	while ((matched = fscanf(input, "%ux%ux%u", &l, &w, &h)) != EOF) {
		if (matched == 3) {
			smallest = area = l * w;
			total += (2 * area);
			area = (w * h);
			total += (2 * area);
			if (area < smallest) {
				smallest = area;
			}
			area = l * h;
			total += (2 * area);
			if (area < smallest) {
				smallest = area;
			}
			total += smallest;
		}
		l = w = h = area = smallest = 0;
	}

	fclose(input);

	printf("total=%u\n", total);

	return 0;
}
