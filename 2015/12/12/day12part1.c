/* day12part1.c 2015 AdventOfCode solution
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
	const char *input_file_name;
	FILE *input;
	int val, total, i, c, lc;

	input_file_name = (argc > 1) ? argv[1] : "input";
	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	val = 0;
	total = 0;
	lc = '\0';
	while ((c = fgetc(input)) != EOF) {
		if (c >= '0' && c <= '9') {
			i = (c - '0');;
			if (val) {
				val *= 10;
				if (val < 0) {
					val -= i;
				} else {
					val += i;
				}
			} else {
				val = i;
				if (lc == '-') {
					val *= -1;
				}
			}
		} else {
			if (val) {
				total += val;
				val = 0;
			}
		}
		lc = c;
	}
	if (val) {
		total += val;
	}

	printf("%d\n", total);

	return 0;
}
