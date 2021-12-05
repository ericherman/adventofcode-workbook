/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 03-1.c 2019 AdventOfCode solution
   Copyright (C) 2019 Eric Herman <eric@freesa.org>
*/
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ehht.h"		/* github.com/ericherman/libehht */

struct wires_s {
	int dist_from_origin;
	int black;
	int red;
};

static int find_closest(struct ehht_key key, void *each_val, void *context)
{
	struct wires_s *closest = (struct wires_s *)context;
	struct wires_s *val = (struct wires_s *)each_val;
	(void)key;
	if (val->black && val->red) {
		if (val->dist_from_origin < closest->dist_from_origin) {
			closest->dist_from_origin = val->dist_from_origin;
			closest->black = val->black;
			closest->red = val->red;
		}
	}
	return 0;
}

static int null_and_free(struct ehht_key key, void *each_val, void *context)
{
	struct ehht *grid = (struct ehht *)context;
	int err = 0;
	grid->put(grid, key.str, key.len, NULL, &err);
	if (err) {
		fprintf(stderr, "put(%s) failed\n", key.str);
		exit(EXIT_FAILURE);
	}
	free(each_val);
	return 0;
}

static void add_wire(struct ehht *grid, int *x, int *y, int wire)
{
	char buf[80];
	struct wires_s *xy;
	int err;

	if (wire != 0 && wire != 1) {
		fprintf(stderr, "Error wire: '%d'\n", wire);
		exit(EXIT_FAILURE);
	}

	sprintf(buf, "[%d,%d]", *x, *y);
	xy = grid->get(grid, buf, strlen(buf));
	if (!xy) {
		xy = calloc(1, sizeof(struct wires_s));
		if (!xy) {
			exit(EXIT_FAILURE);
		}
		xy->dist_from_origin = abs(*x) + abs(*y);
		err = 0;
		grid->put(grid, buf, strlen(buf), xy, &err);
		if (err) {
			fprintf(stderr, "put(%s) failed\n", buf);
			exit(EXIT_FAILURE);
		}
	}

	switch (wire) {
	case 0:
		(xy->black)++;
		break;
	case 1:
		(xy->red)++;
		break;
	}
}

static void route_wire(struct ehht *grid, int *x, int *y, char dir, int dist,
		       int wire)
{
	int i;

	if (dir != 'U' && dir != 'D' && dir != 'L' && dir != 'R') {
		fprintf(stderr, "Error dir: '%c' (%d)\n", dir, dist);
		return;
	}

	for (i = 0; i < dist; ++i) {
		switch (dir) {
		case 'U':
			++(*y);
			break;
		case 'D':
			--(*y);
			break;
		case 'L':
			--(*x);
			break;
		case 'R':
			++(*x);
			break;
		}
		add_wire(grid, x, y, wire);
	}
}

int main(int argc, char **argv)
{
	const char *path;
	FILE *input;
	char *line;
	size_t len;
	ssize_t read;
	char *token, *rest;
	struct ehht *grid;
	char direction;
	int matched, wire, x, y, distance;
	struct wires_s closest_cross;

	path = (argc > 1) ? argv[1] : "input";

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	grid = ehht_new();
	if (!grid) {
		exit(EXIT_FAILURE);
	}

	line = NULL;
	wire = -1;
	while ((read = getline(&line, &len, input)) != -1) {
		++wire;
		x = 0;
		y = 0;
		rest = line;
		while ((token = strtok_r(rest, ",", &rest))) {
			matched = sscanf(token, "%c%d", &direction, &distance);
			if (matched) {
				route_wire(grid, &x, &y, direction, distance,
					   wire);
			}
		}
	}
	fclose(input);
	if (line) {
		free(line);
	}

	closest_cross.dist_from_origin = INT_MAX;
	closest_cross.black = 0;
	closest_cross.red = 0;

	grid->for_each(grid, find_closest, &closest_cross);

	printf("%d\n", closest_cross.dist_from_origin);

	grid->for_each(grid, null_and_free, grid);
	grid->clear(grid);
	ehht_free(grid);

	return 0;
}
