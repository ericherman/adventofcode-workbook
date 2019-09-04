/* SPDX-License-Identifier: GPL-3.0-or-later */
/* day8.c 2015 AdventOfCode solution
   Copyright (C) 2015, 2016, 2019 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	const char *path;
	FILE *input;
	char buf[255];
	size_t i;
	int print_double_encode;
	unsigned total, chars, escaped, instr, double_encode;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	print_double_encode = (argc > 2) ? atoi(argv[2]) : 0;

	total = 0;
	chars = 0;
	escaped = 0;
	instr = 0;
	double_encode = 0;
	while (fgets(buf, 255, input)) {
		for (i = 0; buf[i] != '\0' && i < 255; ++i) {
			if (!instr) {
				if (buf[i] == '"') {
					instr = 1;
					++total;
					double_encode += 4;
				}
			} else {
				switch (buf[i]) {
				case '"':
				case '\\':
					double_encode += 2;
					break;
				default:
					++double_encode;
					break;
				}
				++total;
				if (escaped) {
					++chars;
					escaped = 0;
				} else if (buf[i] == '\\') {
					escaped = 1;
					if (buf[i + 1] == 'x') {
						i += 2;
						double_encode += 2;
						total += 2;
					}
				} else if (buf[i] == '"') {
					instr = 0;
				} else {
					++chars;
				}
			}
		}
		if (instr != 0 || escaped != 0) {
			fprintf(stderr, "Error with input '%s'\n", buf);
		}
	}

	fclose(input);

	printf("total: %u, mem chars: %u\n", total, chars);
	printf("overhead: %u, (double_encode: %u)\n", total - chars,
	       double_encode);

	if (print_double_encode) {
		printf("%u\n", double_encode - total);
	} else {
		printf("%u\n", total - chars);
	}

	return 0;
}
