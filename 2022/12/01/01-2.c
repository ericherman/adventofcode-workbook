/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 01-1.c 2022 AdventOfCode solution */
/* Copyright (C) 2022 Eric Herman <eric@freesa.org> */

#include <libc-headers.h>

#include <eherr.h>
#include <ehstr.h>

static void set_top(int top[3], int val)
{
	if (val >= top[0]) {
		top[2] = top[1];
		top[1] = top[0];
		top[0] = val;
	} else if (val >= top[1]) {
		top[2] = top[1];
		top[1] = val;
	} else if (val >= top[2]) {
		top[2] = val;
	}
}

int main(int argc, char **argv)
{
	const char *path = (argc > 1) ? argv[1] : "input";

	int calories = 0;
	int total = 0;
	int top[3] = { 0, 0, 0 };
	char line[80];

	FILE *input = Fopen_or_die(path, "r");
	while (fgets(line, 80, input)) {
		chomp_crlf(line, 80);
		if (strlen(line)) {
			calories = atoi(line);
			total += calories;
		} else {
			set_top(top, total);
			total = 0;
		}
	}
	fclose(input);
	set_top(top, total);

	printf("%d\n", top[0] + top[1] + top[2]);

	return 0;
}
