/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 05-2.c 2019 AdventOfCode solution
   Copyright (C) 2019 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <stdlib.h>		/* malloc */
#include <string.h>		/* memcpy */

static int get_input(void *input_context)
{
	int input;
	input = *((int *)input_context);
	return input;
}

static void output(int x)
{
	printf("%d\n", x);
}

void run_intcodes(int *memory, size_t size, void *input_context)
{
	int pos = 0;
	int halt = 0;
	int code, op, mode1, mode2, mode3;
	int a, b, c, advance;

	while ((pos < (ssize_t)size) && !halt) {
		code = memory[pos];
		op = code % 100;
		code = code / 100;
		mode1 = code % 10;
		code = code / 10;
		mode2 = code % 10;
		code = code / 10;
		mode3 = code;

		switch (op) {
		case 99:
			halt = 1;
			advance = 0;
			break;
		case 1:	/* add */
			advance = 4;
			a = mode1 ? pos + 1 : memory[pos + 1];
			b = mode2 ? pos + 2 : memory[pos + 2];
			c = mode3 ? pos + 3 : memory[pos + 3];
			memory[c] = memory[a] + memory[b];
			break;
		case 2:	/* mul */
			advance = 4;
			a = mode1 ? pos + 1 : memory[pos + 1];
			b = mode2 ? pos + 2 : memory[pos + 2];
			c = mode3 ? pos + 3 : memory[pos + 3];
			memory[c] = memory[a] * memory[b];
			break;
		case 3:	/* in */
			advance = 2;
			a = mode1 ? pos + 1 : memory[pos + 1];
			memory[a] = get_input(input_context);
			break;
		case 4:	/* out */
			advance = 2;
			a = mode1 ? pos + 1 : memory[pos + 1];
			output(memory[a]);
			break;
		case 5:	/* jnz */
			a = mode1 ? pos + 1 : memory[pos + 1];
			b = mode2 ? pos + 2 : memory[pos + 2];
			if (memory[a]) {
				advance = 0;
				pos = memory[b];
			} else {
				advance = 3;
			}
			break;
		case 6:	/* jz */
			a = mode1 ? pos + 1 : memory[pos + 1];
			b = mode2 ? pos + 2 : memory[pos + 2];
			if (!memory[a]) {
				advance = 0;
				pos = memory[b];
			} else {
				advance = 3;
			}
			break;
		case 7:	/* lt */
			advance = 4;
			a = mode1 ? pos + 1 : memory[pos + 1];
			b = mode2 ? pos + 2 : memory[pos + 2];
			c = mode3 ? pos + 3 : memory[pos + 3];
			memory[c] = (memory[a] < memory[b]) ? 1 : 0;
			break;
		case 8:	/* eq */
			advance = 4;
			a = mode1 ? pos + 1 : memory[pos + 1];
			b = mode2 ? pos + 2 : memory[pos + 2];
			c = mode3 ? pos + 3 : memory[pos + 3];
			memory[c] = (memory[a] == memory[b]) ? 1 : 0;
			break;
		default:
			fprintf(stderr, "unsupported opcoded: %d at %d\n",
				memory[pos], pos);
			advance = 4;
			break;
		}
		if (!halt) {
			pos += advance;
		}
	}
}

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	char *line;
	char *rest;
	char *token;
	size_t len, size, used;
	ssize_t read;

	int *orig_mem, *memory, *new_mem;
	int matched, val, input_data;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	size = 4096;
	used = 0;
	memory = calloc(size, sizeof(int));
	if (!memory) {
		exit(EXIT_FAILURE);
	}

	line = NULL;
	while ((read = getline(&line, &len, input)) != -1) {
		rest = line;
		while ((token = strtok_r(rest, ",", &rest))) {
			matched = sscanf(token, "%d", &val);
			if (matched) {
				memory[used++] = val;
				if (used == size) {
					size *= 2;
					new_mem =
					    realloc(memory, size * sizeof(int));
					if (!new_mem) {
						return EXIT_FAILURE;
					}
					memory = new_mem;
				}
			}
		}
	}
	fclose(input);

	orig_mem = memory;
	new_mem = malloc(size * sizeof(int));
	memory = new_mem;
	memcpy(memory, orig_mem, size * sizeof(int));

	input_data = 5;
	run_intcodes(memory, size, &input_data);

	free(memory);
	free(orig_mem);
	free(line);

	return 0;
}
