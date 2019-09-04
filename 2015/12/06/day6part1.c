/* SPDX-License-Identifier: GPL-3.0-or-later */
/* day6part1.c 2015 AdventOfCode solution
   Copyright (C) 2015, 2016, 2019 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <string.h>		/* strcmp */

int main(int argc, char **argv)
{
	const char *path;
	FILE *input;
	unsigned int offset, x, y, ax, ay, bx, by, total;
	int func;
	unsigned char lights[1000][1000];
	char buf[80];

	path = (argc > 1) ? argv[1] : "input";

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	for (x = 0; x < 1000; ++x) {
		for (y = 0; y < 1000; ++y) {
			lights[x][y] = 0;
		}
	}

	while (fgets(buf, 80, input)) {
		offset = 0;
		func = -1;
		if (strncmp("turn on", buf, 7) == 0) {
			func = 1;
			offset = 8;
		} else if (strncmp("turn off", buf, 8) == 0) {
			func = 0;
			offset = 9;
		} else if (strncmp("toggle", buf, 6) == 0) {
			func = 2;
			offset = 7;
		} else {
			if ((strnlen(buf, 80) > 0) && (buf[0] != '\n')) {
				fprintf(stderr, "Odd: %s\n", buf);
			}
			continue;
		}

		ax = ay = bx = by = 0;
		sscanf(&buf[offset], "%u,%u through %u,%u", &ax, &ay, &bx, &by);
		for (x = ax; x <= bx; ++x) {
			for (y = ay; y <= by; ++y) {
				if (func == 2) {
					lights[x][y] = (lights[x][y]) ? 0 : 1;
				} else {
					lights[x][y] = (unsigned char)func;
				}
			}
		}
	}

	fclose(input);

	total = 0;
	for (x = 0; x < 1000; ++x) {
		for (y = 0; y < 1000; ++y) {
			if (lights[x][y]) {
				total++;
			}
		}
	}
	printf("%u\n", total);

	return 0;
}
