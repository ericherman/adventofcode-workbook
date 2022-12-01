/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 01-1.c 2022 AdventOfCode solution */
/* Copyright (C) 2022 Eric Herman <eric@freesa.org> */

#include <libc-headers.h>

#include <eherr.h>
#include <ehstr.h>

int main(int argc, char **argv)
{
	const char *path = (argc > 1) ? argv[1] : "input";

	int calories = 0;
	int total = 0;
	int most = 0;
	char line[80];

	FILE *input = Fopen_or_die(path, "r");
	while (fgets(line, 80, input)) {
		chomp_crlf(line, 80);
		if (strlen(line)) {
			calories = atoi(line);
			total += calories;
		} else {
			if (total > most) {
				most = total;
			}
			total = 0;
		}
	}
	fclose(input);
	if (total > most) {
		most = total;
	}

	printf("%d\n", most);

	return 0;
}
