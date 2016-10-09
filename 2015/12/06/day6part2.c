/* day6part2.c 2015 AdventOfCode solution
   Copyright (C) 2015-2016 Eric Herman <eric@freesa.org>

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
	unsigned int offset, x, y, ax, ay, bx, by;
	unsigned int total;
	int func;
	unsigned int lights[1000][1000];
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
				switch (func) {
				case 2:
					lights[x][y] += 2;
					break;
				case 1:
					lights[x][y] += 1;
					break;
				case 0:
					if (lights[x][y] > 0) {
						lights[x][y] -= 1;
					}
					break;
				}
			}
		}
	}

	fclose(input);

	total = 0;
	for (x = 0; x < 1000; ++x) {
		for (y = 0; y < 1000; ++y) {
			total += lights[x][y];
		}
	}
	printf("%u\n", total);

	return 0;
}
