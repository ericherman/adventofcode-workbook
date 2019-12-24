/* SPDX-License-Identifier: GPL-3.0-or-later */
/* intcode-computer.c 2019 AdventOfCode solution */
/* Copyright (C) 2019 Eric Herman <eric@freesa.org> */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

struct intcode_cpu_s {
	int *memory;
	size_t len;
	int pos; /* code pointer */
	void (*run)(struct intcode_cpu_s * cpu,
		    int (*get_input)(void *input_context), void *input_context,
		    void (*put_output)(void *output_context, int val),
		    void *output_context);
	char *(*peek)(struct intcode_cpu_s * cpu, int addr, char *buf,
		      size_t buf_len);
	void (*poke)(struct intcode_cpu_s * cpu, int addr, int val);
	struct intcode_cpu_s *(*copy) (struct intcode_cpu_s * cpu);
	void (*free)(struct intcode_cpu_s ** cpu_ref);
};

static int value_for_param(struct intcode_cpu_s *cpu, int mode, int argc)
{
	if (mode == 1) {
		return cpu->pos + argc;
	}
	if (mode == 0) {
		if ((cpu->pos + argc) >= ((ssize_t)cpu->len)) {
			exit(EXIT_FAILURE);
		}
		return cpu->memory[cpu->pos + argc];
	}
	exit(EXIT_FAILURE);
}

static void intcode_run(struct intcode_cpu_s *cpu,
			int (*get_input)(void *input_context),
			void *input_context,
			void (*put_output)(void *output_context, int val),
			void *output_context)
{
	int halt = 0;
	int code, op, mode1, mode2, mode3;
	int a, b, c, advance;

	cpu->pos = 0;

	if (cpu->len > SSIZE_MAX) {
		fprintf(stderr, "len %lu > %ld\n", (unsigned long)cpu->len,
			(long)SSIZE_MAX);
		exit(EXIT_FAILURE);
	}

	while ((cpu->pos < (ssize_t)cpu->len) && !halt) {
		code = cpu->memory[cpu->pos];
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
			a = value_for_param(cpu, mode1, 1);
			b = value_for_param(cpu, mode2, 2);
			c = value_for_param(cpu, mode3, 3);
			cpu->memory[c] = cpu->memory[a] + cpu->memory[b];
			break;
		case 2:	/* mul */
			advance = 4;
			a = value_for_param(cpu, mode1, 1);
			b = value_for_param(cpu, mode2, 2);
			c = value_for_param(cpu, mode3, 3);
			cpu->memory[c] = cpu->memory[a] * cpu->memory[b];
			break;
		case 3:	/* in */
			advance = 2;
			a = value_for_param(cpu, mode1, 1);
			cpu->memory[a] = get_input(input_context);
			break;
		case 4:	/* out */
			advance = 2;
			a = value_for_param(cpu, mode1, 1);
			put_output(output_context, cpu->memory[a]);
			break;
		case 5:	/* jnz */
			a = value_for_param(cpu, mode1, 1);
			b = value_for_param(cpu, mode2, 2);
			if (cpu->memory[a]) {
				advance = 0;
				cpu->pos = cpu->memory[b];
			} else {
				advance = 3;
			}
			break;
		case 6:	/* jz */
			a = value_for_param(cpu, mode1, 1);
			b = value_for_param(cpu, mode2, 2);
			if (!(cpu->memory[a])) {
				advance = 0;
				cpu->pos = cpu->memory[b];
			} else {
				advance = 3;
			}
			break;
		case 7:	/* lt */
			advance = 4;
			a = value_for_param(cpu, mode1, 1);
			b = value_for_param(cpu, mode2, 2);
			c = value_for_param(cpu, mode3, 3);
			if (cpu->memory[a] < cpu->memory[b]) {
				cpu->memory[c] = 1;
			} else {
				cpu->memory[c] = 0;
			}
			break;
		case 8:	/* eq */
			advance = 4;
			a = value_for_param(cpu, mode1, 1);
			b = value_for_param(cpu, mode2, 2);
			c = value_for_param(cpu, mode3, 3);
			if (cpu->memory[a] == cpu->memory[b]) {
				cpu->memory[c] = 1;
			} else {
				cpu->memory[c] = 0;
			}
			break;
		default:
			fprintf(stderr, "unsupported opcoded: %d at %d\n",
				cpu->memory[cpu->pos], cpu->pos);
			advance = 4;
			break;
		}
		if (!halt) {
			cpu->pos += advance;
		}
	}
}

static char *intcode_peek(struct intcode_cpu_s *cpu, int addr, char *buf,
			  size_t buf_len)
{
	if (addr >= (ssize_t)cpu->len) {
		exit(EXIT_FAILURE);
	}
	snprintf(buf, buf_len, "%d", cpu->memory[addr]);
	return buf;
}

static void intcode_poke(struct intcode_cpu_s *cpu, int addr, int val)
{
	if (addr >= (ssize_t)cpu->len) {
		exit(EXIT_FAILURE);
	}
	cpu->memory[addr] = val;
}

static void intcode_free(struct intcode_cpu_s **cpu_ref)
{
	struct intcode_cpu_s *cpu;
	if (!cpu_ref) {
		exit(EXIT_FAILURE);
	}
	cpu = *cpu_ref;
	if (!cpu) {
		exit(EXIT_FAILURE);
	}
	free(cpu->memory);
	free(cpu);
	*cpu_ref = NULL;
}

static struct intcode_cpu_s *intcode_copy(struct intcode_cpu_s *cpu)
{
	struct intcode_cpu_s *copy;
	copy = malloc(sizeof(struct intcode_cpu_s));
	if (!copy) {
		exit(EXIT_FAILURE);
	}
	copy->pos = 0;
	copy->run = intcode_run;
	copy->peek = intcode_peek;
	copy->poke = intcode_poke;
	copy->copy = intcode_copy;
	copy->free = intcode_free;

	copy->memory = malloc(cpu->len * sizeof(int));
	if (!(copy->memory)) {
		exit(EXIT_FAILURE);
	}
	copy->len = cpu->len;
	memcpy(copy->memory, cpu->memory, cpu->len);
	return copy;
}

struct intcode_cpu_s *intcode_new_from_csv(const char *path)
{
	FILE *input;
	char *line;
	size_t line_len;
	char *rest;
	char *token;
	size_t used;
	ssize_t read;
	int matched, val;
	struct intcode_cpu_s *cpu;

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		exit(EXIT_FAILURE);
	}

	cpu = malloc(sizeof(struct intcode_cpu_s));
	if (!cpu) {
		exit(EXIT_FAILURE);
	}

	/* https://www.ibiblio.org/apollo/assembly_language_manual.html */
	/* Memory 15-bit wordlength + 1-bit parity */
	if ((sizeof(int) * CHAR_BIT) < 15) {
		exit(EXIT_FAILURE);
	}
	/* 36K words ROM (core rope memory) */
	/*  2K words RAM (magnetic-core memory) */
	cpu->len = ((36 * 1024) + (2 * 1024));
	cpu->pos = 0;

	cpu->run = intcode_run;
	cpu->peek = intcode_peek;
	cpu->poke = intcode_poke;
	cpu->copy = intcode_copy;
	cpu->free = intcode_free;

	used = 0;
	cpu->memory = calloc(cpu->len, sizeof(int));
	if (!cpu->memory) {
		exit(EXIT_FAILURE);
	}

	line = NULL;
	line_len = 0;
	while ((read = getline(&line, &line_len, input)) != -1) {
		rest = line;
		while ((token = strtok_r(rest, ",", &rest))) {
			matched = sscanf(token, "%d", &val);
			if (matched) {
				cpu->memory[used++] = val;
				if (used == cpu->len) {
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	fclose(input);

	free(line);

	return cpu;
}
