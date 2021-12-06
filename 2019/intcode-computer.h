/* SPDX-License-Identifier: GPL-3.0-or-later */
/* intcode-computer.c 2019 AdventOfCode solution */
/* Copyright (C) 2019 Eric Herman <eric@freesa.org> */

#ifndef INTCODE_COMPUTER_H
#define INTCODE_COMPUTER_H 1

#include <stddef.h>
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

intcode_cpu_s *intcode_new_from_csv(const char *path);

#endif /* INTCODE_COMPUTER_H */
