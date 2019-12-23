/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 08-1.c 2019 AdventOfCode solution */
/* Copyright (C) 2019 Eric Herman <eric@freesa.org> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	int i, c, len, width, height, few_zeros, zeros, ones, twos;
	int *layer_few, *layer_tmp;
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
	layer_tmp = calloc(len, sizeof(int));
	layer_few = calloc(len, sizeof(int));

	i = 0;
	few_zeros = len;
	while ((c = fgetc(input)) != EOF) {
		if (c < '0' || c > '9') {
			continue;
		}
		layer_tmp[i++] = (c - '0');
		if (i == len) {
			zeros = 0;
			for (i = 0; i < len; ++i) {
				if (layer_tmp[i] == 0) {
					++zeros;
				}
			}
			if (zeros < few_zeros) {
				few_zeros = zeros;
				size = len * sizeof(int);
				memcpy(layer_few, layer_tmp, size);
			}
			i = 0;
		}
	}
	fclose(input);

	if (i) {
		fprintf(stderr, "incomplete layer %d\n", i);
	}

	ones = 0;
	twos = 0;
	for (i = 0; i < len; ++i) {
		switch (layer_few[i]) {
		case 1:
			++ones;
			break;
		case 2:
			++twos;
			break;
		default:
			break;
		}
	}

	printf("%d\n", ones * twos);

	free(layer_few);
	free(layer_tmp);

	return 0;
}
