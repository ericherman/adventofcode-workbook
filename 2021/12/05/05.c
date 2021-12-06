/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 05.c 2021 AdventOfCode solution */
/* Copyright (C) 2021 Eric Herman <eric@freesa.org> */

#include <libc-headers.h>

#include <eherr.h>		/* Calloc_or_die */

void *create_2d_array(size_t element_size, const size_t rows, const size_t cols)
{
	size_t pointers_space = sizeof(void *) * rows;
	size_t row_space = element_size * cols;
	size_t values_space = rows * row_space;
	size_t total_space = pointers_space + values_space;

	void **array2d = Calloc_or_die(1, total_space);

	/* set ptr to the first element past the pointers space */
	char *ptr = (char *)(array2d + rows);

	/* set all ptrs to point in to the values space */
	for (size_t i = 0; i < rows; ++i) {
		array2d[i] = ptr + (element_size * cols * i);
	}

	return array2d;
}

struct vent_line {
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;
};

char *vent_line_to_string(struct vent_line *line, char *buf, size_t size)
{
	snprintf(buf, size, "%u,%u -> %u,%u",
		 line->x0, line->y0, line->x1, line->y1);
	return buf;
}

static int between(size_t a, size_t b, size_t c)
{
	if (a <= c) {
		return (a <= b) && (b <= c);
	} else {
		return (a >= b) && (b >= c);
	}
}

void add_line_to_map(uint16_t **ocean_floor, struct vent_line *line,
		     int include_diagonals)
{
	if (line->x0 == line->x1 && line->y0 == line->y1) {
		++(ocean_floor[line->y0][line->x0]);
	} else if (include_diagonals ||
		   (line->x0 == line->x1 || line->y0 == line->y1)) {
		size_t x_delta = 0;
		if (line->x0 < line->x1) {
			x_delta = 1;
		} else if (line->x0 > line->x1) {
			x_delta = -1;
		}
		size_t y_delta = 0;
		if (line->y0 < line->y1) {
			y_delta = 1;
		} else if (line->y0 > line->y1) {
			y_delta = -1;
		}
		size_t x = line->x0;
		size_t y = line->y0;
		do {
			++(ocean_floor[y][x]);
			x += x_delta;
			y += y_delta;
		} while (between(line->y0, y, line->y1) &&
			 between(line->x0, x, line->x1));
	}
}

size_t print_ocean_floor(uint16_t **ocean_floor, size_t rows, size_t cols)
{
	size_t crossings = 0;
	for (size_t y = 0; y < rows; ++y) {
		for (size_t x = 0; x < cols; ++x) {
			if (ocean_floor[y][x] == 0) {
				printf(".");
			} else {
				printf("%u", (unsigned)ocean_floor[y][x]);
			}
			if (ocean_floor[y][x] >= 2) {
				++crossings;
			}
		}
		printf("\n");
	}
	printf("%zu\n", crossings);
	return crossings;
}

size_t list_ocean_floor(uint16_t **ocean_floor, size_t rows, size_t cols)
{
	size_t crossings = 0;
	for (size_t x = 0; x < cols; ++x) {
		for (size_t y = 0; y < rows; ++y) {
			if (ocean_floor[y][x] >= 2) {
				++crossings;
				printf("%zu: %zu, %zu: %u\n", crossings, x, y,
				       (unsigned)ocean_floor[y][x]);
			}
		}
	}
	return crossings;
}

static size_t max3(size_t a, size_t b, size_t c)
{
	size_t max = 0;
	if (a >= b) {
		max = (a >= c) ? a : c;
	} else {
		max = (b >= c) ? b : c;
	}
	return max;
}

int main(int argc, char **argv)
{
	const char *path = (argc > 1) ? argv[1] : "input";
	const int include_diagonals = (argc > 2) ? atoi(argv[2]) : 0;

	size_t lines_len = 0;
	const size_t lines_max = 1000;
	struct vent_line *lines =
	    Calloc_or_die(lines_max, sizeof(struct vent_line));

	int matched = 0;
	unsigned x0 = 0;
	unsigned y0 = 0;
	unsigned x1 = 0;
	unsigned y1 = 0;
	size_t max_x = 0;
	size_t max_y = 0;
	FILE *input = Fopen_or_die(path, "r");
	while ((matched =
		fscanf(input, "%u,%u -> %u,%u", &x0, &y0, &x1, &y1)) != EOF) {
		if (matched) {
			struct vent_line *line = &lines[lines_len];
			line->x0 = x0;
			line->y0 = y0;
			line->x1 = x1;
			line->y1 = y1;
			if (line->x0 == line->x1 && line->y0 == line->y1) {
				char buf[40];
				printf("%zu: %s\n", lines_len,
				       vent_line_to_string(line, buf, 40));
			}

			max_x = max3(max_x, line->x0, line->x1);
			max_y = max3(max_y, line->y0, line->y1);
			++lines_len;
		}
	}
	fclose(input);

	const size_t element_size = sizeof(uint16_t);
	const size_t rows = 1 + max_y;
	const size_t cols = 1 + max_x;
	uint16_t **ocean_floor = create_2d_array(element_size, rows, cols);

	for (size_t i = 0; i < lines_len; ++i) {
		struct vent_line *line = &lines[i];
		add_line_to_map(ocean_floor, line, include_diagonals);
	}

	print_ocean_floor(ocean_floor, rows, cols);

	size_t crossings = list_ocean_floor(ocean_floor, rows, cols);

	printf("%zu\n", crossings);

	free(lines);
	free(ocean_floor);

	return 0;
}
