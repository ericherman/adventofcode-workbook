/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 08-1.c 2019 AdventOfCode solution */
/* Copyright (C) 2019 Eric Herman <eric@freesa.org> */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	int i, j, c, len, width, height, tmp;
	int *layer_msg;
	char pixel;
	size_t size;

	if (argc > 1) {
		path = argv[1];
	}

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	width = 25;
	height = 6;
	len = width * height;
	size = len * sizeof(int);
	layer_msg = malloc(size);
	tmp = 2;
	for (i = 0; i < len; ++i) {
		layer_msg[i] = tmp;
	}

	i = 0;
	while ((c = fgetc(input)) != EOF) {
		if (c < '0' || c > '9') {
			continue;
		}
		switch (c) {
		case '0':
			tmp = 0;
			break;
		case '1':
			tmp = 1;
			break;
		case '2':
			tmp = 2;
			break;
		default:
			printf("unexpected '%c'\n", c);
		}
		if (tmp != 2 && layer_msg[i] == 2) {
			layer_msg[i] = tmp;
		}
		++i;
		if (i == len) {
			i = 0;
		}
	}
	fclose(input);

	if (i) {
		fprintf(stderr, "incomplete layer %d\n", i);
	}

	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			c = (i * width) + j;
			pixel = (layer_msg[c] ? 'X' : ' ');
			printf("%c", pixel);
		}
		printf("\n");
	}

	for (i = 0; i < len; ++i) {
		printf("%d", layer_msg[i]);
	}
	printf("\n");

	free(layer_msg);

	return 0;
}
