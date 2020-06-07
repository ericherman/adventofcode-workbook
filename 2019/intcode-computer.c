/* SPDX-License-Identifier: GPL-3.0-or-later */
/* intcode-computer.c 2019 AdventOfCode solution */
/* Copyright (C) 2019 Eric Herman <eric@freesa.org> */

/* the original apollo computer was 38K */
/* https://www.ibiblio.org/apollo/assembly_language_manual.html */
/* Memory 15-bit wordlength + 1-bit parity */
/* 36K words ROM (core rope memory) */
/*  2K words RAM (magnetic-core memory) */
#define Intcode_default_memlen ((36 * 1024) + (2 * 1024))

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>

enum intcode_op {
	intcode_op_add = 1,
	intcode_op_mul = 2,
	intcode_op_in = 3,
	intcode_op_out = 4,
	intcode_op_jnz = 5,
	intcode_op_jz = 6,
	intcode_op_lt = 7,
	intcode_op_eq = 8,
	intcode_op_add_to_base = 9,
	intcode_op_halt = 99,
	intcode_op_error = -1
};

struct intcode_cpu;
typedef struct intcode_cpu intcode_cpu_s;

typedef int64_t (*intcode_get_input_func)(void *input_context);
typedef void (*intcode_put_output_func)(void *output_context, int64_t val);

struct intcode_cpu {
	int64_t *memory;
	size_t len;
	size_t code_size;
	size_t code_ptr_register;
	size_t base_ptr_register;
	void (*run)(intcode_cpu_s *cpu,
		    intcode_get_input_func get_input, void *input_context,
		    intcode_put_output_func put_output, void *output_context);
	int64_t (*peek)(intcode_cpu_s *cpu, size_t addr);
	void (*poke)(intcode_cpu_s *cpu, size_t addr, int64_t val);
	intcode_cpu_s *(*copy)(intcode_cpu_s *cpu);
	void (*free)(intcode_cpu_s **cpu_ref);
};

static void intcode_dump_code(intcode_cpu_s *cpu, FILE *log)
{
	size_t i;
	if (!cpu) {
		return;
	}
	fprintf(log, "%p {\n", (void *)cpu);
	fprintf(log, "\tmemory = %p\n", (void *)cpu->memory);
	fprintf(log, "\tlen = %zu\n", cpu->len);
	fprintf(log, "\tcode_size = %zu\n", cpu->code_size);
	fprintf(log, "\tpos = %zu\n", cpu->code_ptr_register);
	fprintf(log, "\tbase = %zu\n", cpu->base_ptr_register);
	fprintf(log, "\tcode:\n");
	for (i = 0; i < cpu->code_size; ++i) {
		fprintf(log, "%" PRIi64 ",", cpu->memory[i]);
		if ((i + 1) % 16 == 0) {
			fprintf(log, "\n");
		}
	}
	fprintf(log, "\n}\n");
}

#ifndef die1
#define die1(cpu, str, a) \
	do { \
		fflush(stdout); \
		intcode_dump_code(cpu, stderr); \
		fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
		fprintf(stderr, str, a); \
		fprintf(stderr, "\n"); \
		exit(EXIT_FAILURE); \
	} while (0)
#endif

#ifndef die2
#define die2(cpu, str, a, b) \
	do { \
		fflush(stdout); \
		intcode_dump_code(cpu, stderr); \
		fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
		fprintf(stderr, str, a, b); \
		fprintf(stderr, "\n"); \
		exit(EXIT_FAILURE); \
	} while (0)
#endif

static size_t addr_for_param(intcode_cpu_s *cpu, int mode, unsigned argc)
{
	ssize_t dest, base_ptr_register;

	if (mode < 0 || mode > 2) {
		die1(cpu, "unsuppored mode: %d", mode);
	}

	dest = (cpu->code_ptr_register + argc);
	if (dest < 0) {
		die1(cpu, "negative dest %zd", dest);
	} else if ((size_t)dest >= cpu->len) {
		die2(cpu, "%zd >= %zu", dest, cpu->len);
	}

	if (mode == 1) {
		return dest;
	}

	base_ptr_register = (mode == 2) ? cpu->base_ptr_register : 0;
	dest = base_ptr_register + cpu->memory[dest];
	if (dest < 0) {
		die1(cpu, "negative dest %zd", dest);
	} else if ((size_t)dest >= cpu->len) {
		die2(cpu, "%zd >= %zu", dest, cpu->len);
	}
	return dest;
}

static enum intcode_op intcode_op_for(int64_t i)
{
	if (i >= 1 && i <= 9) {
		return (enum intcode_op)i;
	}
	if (i == 99) {
		return intcode_op_halt;
	}
	return intcode_op_error;
}

static void intcode_run(intcode_cpu_s *cpu,
			int64_t (*get_input)(void *input_context),
			void *input_context,
			void (*put_output)(void *output_context, int64_t val),
			void *output_context)
{
	int halt = 0;
	int opnum, mode1, mode2, mode3, advance;
	int64_t code, val;
	size_t a, b, c;
	enum intcode_op op;

	if (cpu->len > SSIZE_MAX) {
		die2(cpu, "len %zu > %zd", cpu->len, SSIZE_MAX);
	}

	while ((cpu->code_ptr_register < cpu->len) && !halt) {
		code = cpu->memory[cpu->code_ptr_register];
		opnum = code % 100;
		code = code / 100;
		mode1 = code % 10;
		code = code / 10;
		mode2 = code % 10;
		code = code / 10;
		mode3 = code;

		op = intcode_op_for(opnum);
		switch (op) {
		case intcode_op_halt:
			halt = 1;
			advance = 0;
			break;
		case intcode_op_add:
			advance = 4;
			a = addr_for_param(cpu, mode1, 1);
			b = addr_for_param(cpu, mode2, 2);
			c = addr_for_param(cpu, mode3, 3);
			cpu->memory[c] = cpu->memory[a] + cpu->memory[b];
			break;
		case intcode_op_mul:
			advance = 4;
			a = addr_for_param(cpu, mode1, 1);
			b = addr_for_param(cpu, mode2, 2);
			c = addr_for_param(cpu, mode3, 3);
			cpu->memory[c] = cpu->memory[a] * cpu->memory[b];
			break;
		case intcode_op_in:
			advance = 2;
			a = addr_for_param(cpu, mode1, 1);
			cpu->memory[a] = get_input(input_context);
			break;
		case intcode_op_out:
			advance = 2;
			a = addr_for_param(cpu, mode1, 1);
			put_output(output_context, cpu->memory[a]);
			break;
		case intcode_op_jnz:
			a = addr_for_param(cpu, mode1, 1);
			b = addr_for_param(cpu, mode2, 2);
			if (cpu->memory[a]) {
				advance = 0;
				val = cpu->memory[b];
				if (val < 0) {
					die1(cpu, "addr %" PRIi64 "?", val);
				}
				cpu->code_ptr_register = val;
			} else {
				advance = 3;
			}
			break;
		case intcode_op_jz:
			a = addr_for_param(cpu, mode1, 1);
			b = addr_for_param(cpu, mode2, 2);
			if (!(cpu->memory[a])) {
				advance = 0;
				val = cpu->memory[b];
				if (val < 0) {
					die1(cpu, "addr %" PRIi64 "?", val);
				}
				cpu->code_ptr_register = val;
			} else {
				advance = 3;
			}
			break;
		case intcode_op_lt:
			advance = 4;
			a = addr_for_param(cpu, mode1, 1);
			b = addr_for_param(cpu, mode2, 2);
			c = addr_for_param(cpu, mode3, 3);
			if (cpu->memory[a] < cpu->memory[b]) {
				cpu->memory[c] = 1;
			} else {
				cpu->memory[c] = 0;
			}
			break;
		case intcode_op_eq:
			advance = 4;
			a = addr_for_param(cpu, mode1, 1);
			b = addr_for_param(cpu, mode2, 2);
			c = addr_for_param(cpu, mode3, 3);
			if (cpu->memory[a] == cpu->memory[b]) {
				cpu->memory[c] = 1;
			} else {
				cpu->memory[c] = 0;
			}
			break;
		case intcode_op_add_to_base:
			advance = 2;
			a = addr_for_param(cpu, mode1, 1);
			val = cpu->base_ptr_register + cpu->memory[a];
			if (val < 0) {
				die1(cpu, "%" PRIi64 "?", val);
			}
			cpu->base_ptr_register = (size_t)val;
			break;
		case intcode_op_error:
		default:
			halt = 1;
			advance = 0;
			die2(cpu,
			     "unsupported opcoded: %" PRIi64 " at %" PRIi64,
			     cpu->memory[cpu->code_ptr_register],
			     cpu->code_ptr_register);
			break;
		}
		if (!halt) {
			cpu->code_ptr_register += advance;
		}
	}
}

static int64_t intcode_peek(intcode_cpu_s *cpu, size_t addr)
{
	if (addr >= cpu->len) {
		die2(cpu, "%zu > %zu", addr, cpu->len);
	}
	return cpu->memory[addr];
}

static void intcode_poke(intcode_cpu_s *cpu, size_t addr, int64_t val)
{
	if (addr >= cpu->len) {
		die2(cpu, "%zu > %zu", addr, cpu->len);
	}
	cpu->memory[addr] = val;
}

static void intcode_free(intcode_cpu_s **cpu_ref)
{
	intcode_cpu_s *cpu;
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

static intcode_cpu_s *intcode_copy(intcode_cpu_s *cpu)
{
	size_t size;
	intcode_cpu_s *copy;
	size = sizeof(intcode_cpu_s);
	copy = malloc(size);
	if (!copy) {
		die1(cpu, "could not allocat %zu bytes?", size);
	}
	copy->code_size = cpu->code_size;
	copy->code_ptr_register = cpu->code_ptr_register;
	copy->base_ptr_register = cpu->base_ptr_register;
	copy->run = intcode_run;
	copy->peek = intcode_peek;
	copy->poke = intcode_poke;
	copy->copy = intcode_copy;
	copy->free = intcode_free;

	size = cpu->len * sizeof(int64_t);
	copy->memory = malloc(size);
	if (!(copy->memory)) {
		die1(cpu, "could not allocat %zu bytes?", size);
	}
	copy->len = cpu->len;
	memcpy(copy->memory, cpu->memory, cpu->len);
	return copy;
}

intcode_cpu_s *intcode_new_from_csv(const char *path)
{
	FILE *input;
	char *line;
	size_t line_len, size;
	char *rest;
	char *token;
	ssize_t read;
	int matched;
	int64_t val;
	intcode_cpu_s *cpu;

	input = fopen(path, "r");
	if (!input) {
		die1(NULL, "could not open %s\n", path);
	}

	size = sizeof(intcode_cpu_s);
	cpu = malloc(size);
	if (!cpu) {
		die1(cpu, "could not allocat %zu bytes?", size);
	}

	size = (sizeof(int64_t) * CHAR_BIT);	/* word bit length */
	if (size != 64) {
		die1(cpu, "int64_t bit length %zu != 64?", size);
	}

	cpu->len = Intcode_default_memlen;

	cpu->base_ptr_register = 0;
	cpu->code_ptr_register = 0;

	cpu->run = intcode_run;
	cpu->peek = intcode_peek;
	cpu->poke = intcode_poke;
	cpu->copy = intcode_copy;
	cpu->free = intcode_free;

	cpu->code_size = 0;
	cpu->memory = calloc(cpu->len, sizeof(int64_t));
	if (!cpu->memory) {
		size = cpu->len * sizeof(int64_t);
		die1(cpu, "could not allocat %zu bytes?", size);
	}

	line = NULL;
	line_len = 0;
	while ((read = getline(&line, &line_len, input)) != -1) {
		rest = line;
		while ((token = strtok_r(rest, ",", &rest))) {
			matched = sscanf(token, "%" PRIi64, &val);
			if (matched) {
				cpu->memory[cpu->code_size++] = val;
				if (cpu->code_size == cpu->len) {
					die1(cpu, "%zu", cpu->code_size);
				}
			}
		}
	}
	fclose(input);

	free(line);

	return cpu;
}
