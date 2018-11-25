/* day5part2.c 2015 AdventOfCode solution
   Copyright (C) 2015-2016 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <string.h>		/* strlen */

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	char buf[41];
	int matched;
	size_t i, j, len;
	unsigned pairs, skips, nice;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	nice = 0;
	while ((matched = fscanf(input, "%40s", buf)) != EOF) {
		if (matched) {
			pairs = 0;
			skips = 0;
			len = strlen(buf);
			for (i = 0; (pairs == 0 || skips == 0) && i < len; ++i) {
				for (j = i + 2; (pairs == 0) && (j < (len - 1));
				     ++j) {
					if (buf[i] == buf[j]
					    && buf[i + 1] == buf[j + 1]) {
						pairs = 1;
					}
				}
				if ((skips == 0) && (i < len - 2)
				    && (buf[i] == buf[i + 2])) {
					skips = 1;
				}
			}
			if (pairs && skips) {
				nice++;
			}
		}
	}
	fclose(input);

	printf("total=\n");
	printf("%u\n", nice);

	return 0;
}
