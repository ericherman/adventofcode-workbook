/* SPDX-License-Identifier: GPL-3.0-or-later */
/* intcode-computer.c 2019 AdventOfCode solution */
/* Copyright (C) 2019 Eric Herman <eric@freesa.org> */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

struct intcode_cpu_s {
	int64_t *memory;
	size_t len;
	size_t code_size;
	int64_t pos;		/* code pointer */
	int64_t base;		/* base pointer */
	void (*run)(struct intcode_cpu_s * cpu,
		    int64_t (*get_input)(void *input_context),
		    void *input_context,
		    void (*put_output)(void *output_context, int64_t val),
		    void *output_context);
	int64_t (*peek)(struct intcode_cpu_s * cpu, int64_t addr);
	void (*poke)(struct intcode_cpu_s * cpu, int64_t addr, int64_t val);
	struct intcode_cpu_s *(*copy) (struct intcode_cpu_s * cpu);
	void (*free)(struct intcode_cpu_s ** cpu_ref);
};

static void intcode_dump_code(struct intcode_cpu_s *cpu, FILE *log)
{
	size_t i;
	if (!cpu) {
		return;
	}
	fprintf(log, "%p {\n", (void *)cpu);
	fprintf(log, "\tmemory = %p\n", (void *)cpu->memory);
	fprintf(log, "\tlen = %llu\n", (unsigned long long)cpu->len);
	fprintf(log, "\tcode_size = %llu\n",
		(unsigned long long)cpu->code_size);
	fprintf(log, "\tpos = %lld\n", (long long)cpu->pos);
	fprintf(log, "\tbase = %lld\n", (long long)cpu->base);
	fprintf(log, "\tcode:\n");
	for (i = 0; i < cpu->code_size; ++i) {
		fprintf(log, "%lld,", (long long)cpu->memory[i]);
		if ((i + 1) % 16 == 0) {
			fprintf(log, "\n");
		}
	}
	fprintf(log, "\n}\n");
}

#ifndef die1
#define die1(cpu, str, a) do { \
	intcode_dump_code(cpu, stderr); \
	fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
	fprintf(stderr, str, a); \
	fprintf(stderr, "\n"); \
	exit(EXIT_FAILURE); \
	} while (0)
#endif

#ifndef die2
#define die2(cpu, str, a, b) do { \
	intcode_dump_code(cpu, stderr); \
	fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
	fprintf(stderr, str, a, b); \
	fprintf(stderr, "\n"); \
	exit(EXIT_FAILURE); \
	} while (0)
#endif

static int64_t value_for_param(struct intcode_cpu_s *cpu, int mode, int argc)
{
	ssize_t dest, base;

	if (mode < 0 || mode > 2) {
		die1(cpu, "unsuppored mode: %d", mode);
	}

	dest = (cpu->pos + argc);
	if (dest < 0) {
		die1(cpu, "negative dest %lld", (long long)dest);
	} else if (dest >= ((ssize_t)cpu->len)) {
		die2(cpu, "%lld >= %lld", (long long)dest, (long long)cpu->len);
	}

	if (mode == 1) {
		return dest;
	}

	base = (mode == 2) ? cpu->base : 0;
	dest = base + cpu->memory[dest];
	if (dest < 0) {
		die1(cpu, "negative dest %lld", (long long)dest);
	} else if (dest >= ((ssize_t)cpu->len)) {
		die2(cpu, "%lld >= %lld", (long long)dest, (long long)cpu->len);
	}
	return dest;
}

static void intcode_run(struct intcode_cpu_s *cpu,
			int64_t (*get_input)(void *input_context),
			void *input_context,
			void (*put_output)(void *output_context, int64_t val),
			void *output_context)
{
	int halt = 0;
	int op, mode1, mode2, mode3, advance;
	int64_t code, a, b, c;

	if (cpu->len > SSIZE_MAX) {
		die2(cpu, "len %llu > %lld",
		     (unsigned long long)cpu->len, (long long)SSIZE_MAX);
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
		case 9:	/* add to base  */
			advance = 2;
			a = value_for_param(cpu, mode1, 1);
			cpu->base += cpu->memory[a];
			break;
		default:
			die2(cpu, "unsupported opcoded: %lld at %lld",
			     (long long)cpu->memory[cpu->pos],
			     (long long)cpu->pos);
			break;
		}
		if (!halt) {
			cpu->pos += advance;
		}
	}
}

static int64_t intcode_peek(struct intcode_cpu_s *cpu, int64_t addr)
{
	if (addr >= (ssize_t)cpu->len) {
		die2(cpu, "%lld > %lld\n", (long long)addr,
		     (long long)(cpu->len));
	}
	return cpu->memory[addr];
}

static void intcode_poke(struct intcode_cpu_s *cpu, int64_t addr, int64_t val)
{
	if (addr >= (ssize_t)cpu->len) {
		die2(cpu, "%lld > %lld\n", (long long)addr,
		     (long long)(cpu->len));
	}
	cpu->memory[addr] = val;
}

static void intcode_free(struct intcode_cpu_s **cpu_ref)
{
	struct intcode_cpu_s *cpu;
	if (!cpu_ref) {
		die1(NULL, "%s is NULL", "cpu_ref");
	}
	cpu = *cpu_ref;
	if (!cpu) {
		die1(cpu, "%s is NULL", "cpu");
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
		die1(cpu, "could not allocat %llu bytes?",
		     (unsigned long long)sizeof(struct intcode_cpu_s));
	}
	copy->code_size = cpu->code_size;
	copy->pos = cpu->pos;
	copy->base = cpu->base;
	copy->run = intcode_run;
	copy->peek = intcode_peek;
	copy->poke = intcode_poke;
	copy->copy = intcode_copy;
	copy->free = intcode_free;

	copy->memory = malloc(cpu->len * sizeof(int64_t));
	if (!(copy->memory)) {
		die1(cpu, "could not allocat %llu bytes?",
		     (unsigned long long)cpu->len * sizeof(int64_t));
	}
	copy->len = cpu->len;
	memcpy(copy->memory, cpu->memory, cpu->len);
	return copy;
}

struct intcode_cpu_s *intcode_new_from_csv(const char *path)
{
	FILE *input;
	char *line;
	size_t line_len, size;
	char *rest;
	char *token;
	ssize_t read;
	int matched;
	long long val;
	struct intcode_cpu_s *cpu;

	input = fopen(path, "r");
	if (!input) {
		die1(NULL, "could not open %s\n", path);
	}

	cpu = malloc(sizeof(struct intcode_cpu_s));
	if (!cpu) {
		die1(cpu, "could not allocat %llu bytes?",
		     (unsigned long long)sizeof(struct intcode_cpu_s));
	}

	/* the original apollo computer was 38K */
	/* https://www.ibiblio.org/apollo/assembly_language_manual.html */
	/* Memory 15-bit wordlength + 1-bit parity */
	/* 36K words ROM (core rope memory) */
	/*  2K words RAM (magnetic-core memory) */
	size = (sizeof(int64_t) * CHAR_BIT);	/* word bit length */
	if (size != 64) {
		die1(cpu, "word bit length %llu != 64?",
		     (unsigned long long)size);
	}
	cpu->len = ((36 * 1024) + (2 * 1024));

	cpu->base = 0;
	cpu->pos = 0;

	cpu->run = intcode_run;
	cpu->peek = intcode_peek;
	cpu->poke = intcode_poke;
	cpu->copy = intcode_copy;
	cpu->free = intcode_free;

	cpu->code_size = 0;
	cpu->memory = calloc(cpu->len, sizeof(int64_t));
	if (!cpu->memory) {
		size = cpu->len * sizeof(int64_t);
		die1(cpu, "could not allocat %llu bytes?",
		     (unsigned long long)size);
	}

	line = NULL;
	line_len = 0;
	while ((read = getline(&line, &line_len, input)) != -1) {
		rest = line;
		while ((token = strtok_r(rest, ",", &rest))) {
			matched = sscanf(token, "%lld", &val);
			if (matched) {
				cpu->memory[cpu->code_size++] = val;
				if (cpu->code_size == cpu->len) {
					die1(cpu, "%llu", (unsigned long long)
					     cpu->code_size);
				}
			}
		}
	}
	fclose(input);

	free(line);

	return cpu;
}
