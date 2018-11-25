/* day5part1.c 2015 AdventOfCode solution
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
	size_t i, len;
	unsigned vowels, doubles, naughty, nice;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	nice = 0;
	while ((matched = fscanf(input, "%40s", buf)) != EOF) {
		if (matched) {
			vowels = 0;
			doubles = 0;
			naughty = 0;

			len = strlen(buf);
			for (i = 0; i < len; ++i) {
				switch (buf[i]) {
				case 'e':
				case 'i':
				case 'o':
				case 'u':
				case 'a':
					vowels++;
					break;
				default:
					break;
				}

				if (buf[i] == buf[i + 1]) {
					doubles++;
				}

				switch (buf[i]) {
				case 'a':
				case 'c':
				case 'p':
				case 'x':
					if (buf[i + 1] == buf[i] + 1) {
						naughty = 1;
					}
					break;
				default:
					break;
				}
			}
			if (vowels >= 3 && doubles && !naughty) {
				nice++;
			}
		}
	}

	fclose(input);

	printf("total=\n");
	printf("%u\n", nice);

	return 0;
}
