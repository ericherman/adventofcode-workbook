/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 02-2.c 2021 AdventOfCode solution */
/* Copyright (C) 2021 Eric Herman <eric@freesa.org> */

#include <libc-headers.h>

#include <eherr.h>

int main(int argc, char **argv)
{

	const char *path = (argc > 1) ? argv[1] : "input";

	char direction[40];
	memset(direction, 0x00, 40);

	int aim = 0;
	int x = 0;
	int z = 0;
	int matched = 0;
	int dist = 0;

	FILE *input = Fopen_or_die(path, "r");
	while ((matched = fscanf(input, "%s %d", direction, &dist)) != EOF) {
		if (matched == 2) {
			if (strcmp("forward", direction) == 0) {
				x += dist;
				z += (aim * dist);
			} else if (strcmp("down", direction) == 0) {
				aim += dist;
			} else if (strcmp("up", direction) == 0) {
				aim -= dist;
			} else {
				return 1;
			}
		}
	}
	fclose(input);

	printf("%d\n", x * z);

	return 0;
}
