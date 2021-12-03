/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 03-1.c 2021 AdventOfCode solution
   Copyright (C) 2021 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	char str[80];
	unsigned b0[64];
	unsigned b1[64];
	int matched, gamma, epsilon;
	size_t i, bits;

	memset(str, 0x00, 80);
	memset(b0, 0x00, sizeof(unsigned) * 64);
	memset(b1, 0x00, sizeof(unsigned) * 64);
	bits = 0;
	gamma = 0;
	epsilon = 0;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}
	while ((matched = fscanf(input, "%s", str)) != EOF) {
		if (matched) {
			bits = strlen(str);
			for (i = 0; i < bits; ++i) {
				switch (str[i]) {
				case '0':
					++b0[i];
					break;
				case '1':
					++b1[i];
					break;
				default:
					fprintf(stderr, "'%s'?\n", str);
					return 1;
				}
			}
		}
	}
	fclose(input);

	for (i = 0; i < bits; ++i) {
		gamma = gamma << 1;
		epsilon = epsilon << 1;
		if (b1[i] > b0[i]) {
			gamma += 1;
		} else {
			epsilon += 1;
		}
	}

	printf("%d\n", gamma * epsilon);

	return 0;
}
