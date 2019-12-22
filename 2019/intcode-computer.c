/* SPDX-License-Identifier: GPL-3.0-or-later */
/* intcode-computer.c 2019 AdventOfCode solution */
/* Copyright (C) 2019 Eric Herman <eric@freesa.org> */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

void run_intcodes(int *memory, size_t size,
		  int (*get_input)(void *input_context), void *input_context,
		  void (*put_output)(void *output_context, int val),
		  void *output_context)
{
	int pos = 0;
	int halt = 0;
	int code, op, mode1, mode2, mode3;
	int a, b, c, advance;

	if (size > SSIZE_MAX) {
		fprintf(stderr, "size %lu > %lu\n", (unsigned long)size,
			(unsigned long)SSIZE_MAX);
		exit(EXIT_FAILURE);
	}

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
			put_output(output_context, memory[a]);
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

int *load_ints_from_csv(const char *path, size_t *size)
{
	int *memory, *new_mem;
	FILE *input;
	char *line;
	char *rest;
	char *token;
	size_t len, used;
	ssize_t read;
	int matched, val;

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		exit(EXIT_FAILURE);
	}

	/* https://www.ibiblio.org/apollo/assembly_language_manual.html */
	/* Memory 15-bit wordlength + 1-bit parity */
	if ((sizeof(int) * CHAR_BIT) < 15) {
		exit(EXIT_FAILURE);
	}
	/* 36K words ROM (core rope memory) */
	/*  2K words RAM (magnetic-core memory) */
	*size = ((36 * 1024) + (2 * 1024));

	used = 0;
	memory = calloc(*size, sizeof(int));
	if (!memory) {
		exit(EXIT_FAILURE);
	}

	line = NULL;
	len = 0;
	while ((read = getline(&line, &len, input)) != -1) {
		rest = line;
		while ((token = strtok_r(rest, ",", &rest))) {
			matched = sscanf(token, "%d", &val);
			if (matched) {
				memory[used++] = val;
				if (used == *size) {
					*size *= 2;
					new_mem =
					    realloc(memory,
						    (*size) * sizeof(int));
					if (!new_mem) {
						exit(EXIT_FAILURE);
					}
					memory = new_mem;
				}
			}
		}
	}
	fclose(input);

	free(line);

	return memory;
}
