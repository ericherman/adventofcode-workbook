#include <stdio.h>		/* printf */
#include <stdlib.h>		/* atoi */
#include <limits.h>		/* INT_MIN INT_MAX */
#include <string.h>		/* strcmp */

#define BUF_LEN 255

#define BAD 0
#define HLF 1
#define TPL 2
#define INC 3
#define JMP 4
#define JIE 5
#define JIO 6
#define END 255

struct instruction_s {
	unsigned char op;
	char reg;
	int val;
};

struct machine_s {
	int a;
	int b;
	size_t line;
};

static const char *opcode_to_str(unsigned char op)
{
	char *str;
	switch (op) {
	case HLF:
		str = "hlf";
		break;
	case TPL:
		str = "tpl";
		break;
	case INC:
		str = "inc";
		break;
	case JMP:
		str = "jmp";
		break;
	case JIE:
		str = "jie";
		break;
	case JIO:
		str = "jio";
		break;
	case END:
		str = "end";
		break;
	default:
		str = "BAD";
	}
	return str;
}

int parse_op(struct instruction_s *c, const char *buf, int verbose)
{
	char inst[BUF_LEN], arg1[BUF_LEN], arg2[BUF_LEN];
	int matched;

	matched = sscanf(buf, "%s %s %s", inst, arg1, arg2);
	if (matched < 2) {
		if (verbose && matched == 1) {
			fprintf(stderr, "bad match: %s\n", buf);
		}
		return BAD;
	}

	if (strcmp("hlf", inst) == 0) {
		c->op = HLF;
	} else if (strcmp("tpl", inst) == 0) {
		c->op = TPL;
	} else if (strcmp("inc", inst) == 0) {
		c->op = INC;
	} else if (strcmp("jmp", inst) == 0) {
		c->op = JMP;
	} else if (strcmp("jie", inst) == 0) {
		c->op = JIE;
	} else if (strcmp("jio", inst) == 0) {
		c->op = JIO;
	} else {
		if (verbose) {
			fprintf(stderr, "bad match: %s\n", buf);
		}
		c->op = BAD;
	}

	if (c->op == JMP) {
		c->val = atoi(arg1);
		c->reg = '\0';
	} else {
		c->reg = arg1[0];
		if (c->reg != 'a' && c->reg != 'b') {
			if (verbose) {
				fprintf(stderr, "bad match: %s\n", buf);
			}
			return BAD;
		}
		if (c->op == JIE || c->op == JIO) {
			c->val = atoi(arg2);
		} else {
			c->val = 0;
		}
	}

	return c->op;
}

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	char buf[BUF_LEN];
	struct instruction_s *code;
	size_t code_len, code_size, bytes;
	struct machine_s cpu;
	int verbose, *r, init_a;

	verbose = (argc > 1) ? atoi(argv[1]) : 0;
	init_a = (argc > 2) ? atoi(argv[2]) : 0;
	input_file_name = (argc > 3) ? argv[3] : "input";
	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	code_size = 0;
	code_len = 0;
	code = NULL;
	while (fgets(buf, BUF_LEN, input)) {
		if (code_size <= code_len) {
			code_size += BUF_LEN;
			bytes = sizeof(struct instruction_s) * code_size;
			code = realloc(code, bytes);
			if (!code) {
				fprintf(stderr, "could not alloc %lu bytes\n",
					(unsigned long)bytes);
				exit(EXIT_FAILURE);
			}
		}
		if (parse_op(&code[code_len], buf, verbose)) {
			++code_len;
		}
	}
	fclose(input);
	code[code_len].op = END;

	cpu.a = init_a;
	cpu.b = 0;
	cpu.line = 0;

	while (code[cpu.line].op != END) {
		if (verbose) {
			fprintf(stderr, "cpu.a: %d, cpu.b: %d, cpu.line: %d\n",
				cpu.a, cpu.b, cpu.line);
			fprintf(stderr, "\top: %s, reg: %c, val: %d\n",
				opcode_to_str(code[cpu.line].op),
				code[cpu.line].reg, code[cpu.line].val);
		}

		switch (code[cpu.line].reg) {
		case 'a':
			r = &cpu.a;
			break;
		case 'b':
			r = &cpu.b;
			break;
		default:
			r = NULL;
		}

		switch (code[cpu.line].op) {
		case HLF:
			*r *= 0.5;
			++cpu.line;
			break;
		case TPL:
			*r *= 3;
			++cpu.line;
			break;
		case INC:
			*r += 1;
			++cpu.line;
			break;
		case JMP:
			cpu.line += code[cpu.line].val;
			break;
		case JIE:
			if ((*r % 2) == 0) {
				cpu.line += code[cpu.line].val;
			} else {
				++cpu.line;
			}
			break;
		case JIO:
			if (*r == 1) {
				cpu.line += code[cpu.line].val;
			} else {
				++cpu.line;
			}
			break;
		default:
			++cpu.line;
		}
	}
	if (verbose) {
		fprintf(stderr, "cpu.a: %d, cpu.b: %d, cpu.line: %d\n", cpu.a,
			cpu.b, cpu.line);
	}

	printf("%d\n", cpu.b);

	free(code);

	return 0;
}
