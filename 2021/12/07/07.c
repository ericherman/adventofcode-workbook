/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 07.c 2021 AdventOfCode solution */
/* Copyright (C) 2021 Eric Herman <eric@freesa.org> */

#include <libc-headers.h>

#include <eherr.h>		/* Fopen_or_die */

int *crab_positions_init(size_t *crab_positions_len, const char *path)
{
	*crab_positions_len = 0;
	size_t elements = 100;
	int *crab_positions = Calloc_or_die(elements, sizeof(int));

	FILE *input = Fopen_or_die(path, "r");
	int c = EOF;
	int written = 0;
	while ((c = fgetc(input)) != EOF) {
		if (c >= '0' && c <= '9') {
			int8_t n = (c - '0');
			crab_positions[*crab_positions_len] *= 10;
			crab_positions[*crab_positions_len] += n;
			written = 1;
		} else {
			++(*crab_positions_len);
			written = 0;
			if (*crab_positions_len == elements) {
				size_t old_elements = elements;
				elements += 100;
				crab_positions = Recalloc_or_die(crab_positions,
								 old_elements,
								 elements,
								 sizeof(int));
			}
		}
	}
	if (written) {
		++(*crab_positions_len);
		written = 0;
	}
	fclose(input);
	return crab_positions;
}

int main(int argc, char **argv)
{
	const char *path = (argc > 1) ? argv[1] : "input";
	int progressive_cost = (argc > 2) ? atoi(argv[2]) : 0;

	size_t crab_positions_len = 0;
	int *crab_positions = crab_positions_init(&crab_positions_len, path);

	int min = INT_MAX;
	int max = INT_MIN;
	for (size_t i = 0; i < crab_positions_len; ++i) {
		int pos = crab_positions[i];
		if (pos < min) {
			min = pos;
		}
		if (pos > max) {
			max = pos;
		}
	}
	printf("min: %d max: %d\n", min, max);

	int best_fuel = INT_MAX;
	int best_candidate = INT_MAX;
	for (int candidate = min; candidate <= max; ++candidate) {
		printf("candidate: %d\n", candidate);
		int fuel = 0;
		for (size_t i = 0; fuel < best_fuel && i < crab_positions_len;
		     ++i) {
			int distance = abs(crab_positions[i] - candidate);
			int cost = 0;
			if (progressive_cost) {
				for (int i = 1; i <= distance; ++i) {
					cost += i;
				}
			} else {
				cost = distance;
			}
			printf("Move from %d to %d: %d fuel\n",
			       crab_positions[i], candidate, cost);
			fuel += cost;
		}
		if (fuel < best_fuel) {
			best_fuel = fuel;
			best_candidate = candidate;
		}
		printf("fuel: %d, bestfuel: %d\n", fuel, best_fuel);
	}

	free(crab_positions);

	printf("%d\n", best_candidate);
	printf("%d\n", best_fuel);

	return 0;
}
