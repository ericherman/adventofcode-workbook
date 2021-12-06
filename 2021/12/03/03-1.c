/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 03-1.c 2021 AdventOfCode solution */
/* Copyright (C) 2021 Eric Herman <eric@freesa.org> */

#include <libc-headers.h>

#include <eherr.h>

int main(int argc, char **argv)
{
	char str[80];
	memset(str, 0x00, 80);

	unsigned b0[64];
	memset(b0, 0x00, sizeof(unsigned) * 64);

	unsigned b1[64];
	memset(b1, 0x00, sizeof(unsigned) * 64);

	size_t bits = 0;
	int gamma = 0;
	int epsilon = 0;

	int matched = EOF;
	const char *path = (argc > 1) ? argv[1] : "input";
	FILE *input = Fopen_or_die(path, "r");
	while ((matched = fscanf(input, "%s", str)) != EOF) {
		if (matched) {
			bits = strlen(str);
			for (size_t i = 0; i < bits; ++i) {
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

	for (size_t i = 0; i < bits; ++i) {
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
