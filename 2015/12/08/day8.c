/* day8.c 2015 AdventOfCode solution
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
	const char *path;
	FILE *input;
	char buf[255];
	size_t i;
	unsigned total, chars, escaped, instr, double_encode;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

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

	return 0;
}
