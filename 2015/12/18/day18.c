/* day18.c 2015 AdventOfCode solution
   Copyright (C) 2015-2016 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>

static int neighbors_on(char *from, int grid_size, int y, int x)
{
	int ny, nx, n_on;

	n_on = 0;
	for (ny = (y - 1); ny <= (y + 1); ++ny) {
		for (nx = (x - 1); nx <= (x + 1); ++nx) {
			if (!((ny == y && nx == x) || (ny < 0 || nx < 0)
			      || (ny >= grid_size || nx >= grid_size))) {
				if (from[(ny * grid_size) + nx] == '#') {
					++n_on;
				}
			}
		}
	}
	return n_on;
}

static int report(char *grid, int grid_size, int print)
{
	char c;
	int x, y, total;

	total = 0;
	for (y = 0; y < grid_size; ++y) {
		for (x = 0; x < grid_size; ++x) {
			c = grid[(y * grid_size) + x];
			if (c == '#') {
				++total;
			}
			if (print) {
				printf("%c", c);
			}
		}
		if (print) {
			printf("\n");
		}
	}
	return total;
}

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	int c;

	unsigned int total;
	char *from, *to, *last;
	char from_c, to_c;
	int x, y, i, grid_size, steps, verbose, n_on, corners_stuck;

	corners_stuck = (argc > 1) ? atoi(argv[1]) : 0;
	steps = (argc > 2) ? atoi(argv[2]) : 100;
	input_file_name = (argc > 3) ? argv[3] : "input";
	grid_size = (argc > 4) ? atoi(argv[4]) : 100;
	verbose = (argc > 5) ? atoi(argv[5]) : 0;

	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	from = alloca(sizeof(char) * (grid_size * grid_size));
	to = alloca(sizeof(char) * (grid_size * grid_size));
	last = NULL;
	if (!from || !to) {
		fprintf(stderr, "could not allocate arrays\n");
		return 1;
	}
	for (y = 0; y < grid_size; ++y) {
		for (x = 0; x < grid_size; ++x) {
			from[(y * grid_size) + x] = '\0';
			to[(y * grid_size) + x] = '\0';
		}
	}

	x = 0;
	y = 0;
	while ((c = fgetc(input)) != EOF) {
		switch (c) {
		case '.':
		case '#':
			if (y >= grid_size || x >= grid_size) {
				fprintf(stderr, "too much input\n");
				return 1;
			}
			from[(y * grid_size) + x] = c;
			++x;
			if (x == grid_size) {
				x = 0;
				++y;
			}
		}
	}
	fclose(input);
	if (y != grid_size || x != 0) {
		fprintf(stderr, "Expected: %ux0 but was: %ux%u\n", grid_size, y,
			x);
		return 1;
	}

	if (corners_stuck) {
		from[0 + 0] = '#';
		from[0 + (grid_size - 1)] = '#';
		from[((grid_size - 1) * grid_size) + 0] = '#';
		from[((grid_size - 1) * grid_size) + (grid_size - 1)] = '#';
	}

	if (verbose) {
		printf("Initial state:\n");
		report(from, grid_size, verbose);
	}

	for (i = 0; i < steps; ++i) {
		for (y = 0; y < grid_size; ++y) {
			for (x = 0; x < grid_size; ++x) {
				from_c = from[(y * grid_size) + x];
				n_on = neighbors_on(from, grid_size, y, x);
				if (from_c == '#') {
					switch (n_on) {
					case 2:
					case 3:
						to_c = '#';
						break;
					default:
						to_c = '.';
					}
				} else {
					to_c = (n_on == 3) ? '#' : '.';
				}
				to[(y * grid_size) + x] = to_c;
			}
		}
		if (corners_stuck) {
			to[0 + 0] = '#';
			to[0 + (grid_size - 1)] = '#';
			to[((grid_size - 1) * grid_size) + 0] = '#';
			to[((grid_size - 1) * grid_size) + (grid_size - 1)] =
			    '#';
		}

		last = to;
		to = from;
		from = last;

		if (verbose) {
			printf("After %d steps:\n", i + 1);
			report(last, grid_size, verbose);
		}
	}

	total = report(last, grid_size, 0);
	printf("after %d steps, lights on: %d\n", i, total);

	return 0;
}
