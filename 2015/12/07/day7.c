/* SPDX-License-Identifier: GPL-3.0-or-later */
/* day7.c 2015 AdventOfCode solution
   Copyright (C) 2015, 2016, 2019 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <string.h>		/* strrchar strndup */
#include <stdlib.h>		/* malloc */
#include <ctype.h>		/* isdigit */
#include <ehht.h>		/* github.com/ericherman/libehht */
#include <ehstr.h>		/* github.com/ericherman/libehstr */

#define TOKEN_BUF_LEN 20

enum op_code {
	OP_INVALID = -1,
	OP_NOP = 0,
	OP_LOAD,
	OP_MOVE,
	OP_AND,
	OP_OR,
	OP_NOT,
	OP_LSHIFT,
	OP_RSHIFT,
	OP_HALT
};

static enum op_code parse_op_code(char *buf, size_t len)
{
	if (strncmp("NOP", buf, len) == 0) {
		return OP_NOP;
	}
	if (strncmp("LOAD", buf, len) == 0) {
		return OP_LOAD;
	}
	if (strncmp("AND", buf, len) == 0) {
		return OP_AND;
	}
	if (strncmp("OR", buf, len) == 0) {
		return OP_OR;
	}
	if (strncmp("NOT", buf, len) == 0) {
		return OP_NOT;
	}
	if (strncmp("LSHIFT", buf, len) == 0) {
		return OP_LSHIFT;
	}
	if (strncmp("RSHIFT", buf, len) == 0) {
		return OP_RSHIFT;
	}
	if (strncmp("HALT", buf, len) == 0) {
		return OP_HALT;
	}
	fprintf(stderr, "'%s' not a valid OP\n", buf);
	return OP_INVALID;
}

struct op_s {
	char *out;
	int result;
	enum op_code op;
	char *in1;
	char *in2;
	unsigned satisfied:1;
};

static struct op_s *new_op_s()
{
	struct op_s *o;

	o = malloc(sizeof(struct op_s));
	if (!o) {
		return NULL;
	}

	o->out = NULL;
	o->result = -1;
	o->op = OP_INVALID;
	o->in1 = NULL;
	o->in2 = NULL;
	o->satisfied = 0;

	return o;
}

static void free_op_s(struct op_s *o)
{
	if (!o) {
		return;
	}
	if (o->out)
		free(o->out);
	if (o->in1)
		free(o->in1);
	if (o->in2)
		free(o->in2);
	free(o);
}

static int to_string_op_s(struct op_s *o, char *buf, size_t buf_len)
{
	char result[20];
	if (!o) {
		return snprintf(buf, buf_len, "NULL");
	}
	if (o->satisfied) {
		sprintf(result, "%d", o->result);
	} else {
		sprintf(result, "(unsatisfied)");
	}

	return snprintf(buf, buf_len, "%s = %s (%d %s %s)", o->out, result,
			o->op, o->in1 ? o->in1 : "NULL",
			o->in2 ? o->in2 : "NULL");
}

static int to_string_each(struct ehht_key_s key, void *each_val, void *context)
{
	char buf[100];
	struct op_s *o;

	o = (struct op_s *)each_val;
	to_string_op_s(o, buf, 100);
	fprintf(stderr, "Key: %s (%lu) Val: %s\n", key.str,
		(unsigned long)key.len, buf);
	if (context) {
		fprintf(stderr, "%p\n", context);
	}
	return 0;
}

static struct op_s *parse_token(char *buf, size_t buf_len)
{
	struct op_s *ops;
	char *out;
	char token[3][TOKEN_BUF_LEN];
	size_t i, j, k, tokens;
	unsigned len;

	out = strrchr(buf, '>');
	if (!out) {
		return NULL;
	}
	out += 2;

	len = strnlen(buf, buf_len);
	for (j = 0; j < 3; ++j) {
		token[j][0] = '\0';
	}
	for (i = 0, j = 0, k = 0, tokens = 0; i < len && j < 3; ++i) {
		if (isalnum(buf[i])) {
			token[j][k++] = buf[i];
		} else {
			token[j++][k] = '\0';
			if (k > 0) {
				++tokens;
			}
			k = 0;
		}
	}
	if (0) {
		fprintf(stderr, "out: %s := (%s %s %s)\n", out, token[0],
			token[1], token[2]);
	}
	ops = new_op_s();
	if (ops == NULL) {
		fprintf(stderr, "allocation failed (%s)\n", buf);
		return NULL;
	}
	ops->out = strdup(out);

	switch (tokens) {
	case 3:
		ops->op = parse_op_code(token[1], TOKEN_BUF_LEN);
		ops->in1 = strndup(token[0], TOKEN_BUF_LEN);
		ops->in2 = strndup(token[2], TOKEN_BUF_LEN);
		break;
	case 2:
		if (strcmp("NOT", token[0]) == 0) {
			ops->in1 = strndup(token[1], TOKEN_BUF_LEN);
			ops->op = OP_NOT;
		} else {
			goto token_parse_error;
		}
		break;
	case 1:
		ops->op = (isdigit(token[0][0])) ? OP_LOAD : OP_MOVE;
		ops->in1 = strndup(token[0], TOKEN_BUF_LEN);
		break;
	default:
token_parse_error:
		fprintf(stderr, "buf: '%s' mis-parsed", buf);
		fprintf(stderr, " as out: %s := (%s %s %s)\n",
			out, token[0], token[1], token[2]);
		free_op_s(ops);
		return NULL;
	}
	return ops;
}

static unsigned token_to_result(struct ehht_s *wires, char *token, int *result)
{
	struct op_s *o;

	if (isdigit(token[0])) {
		*result = atoi(token);
		return 1;
	}

	o = wires->get(wires, token, strnlen(token, TOKEN_BUF_LEN));
	if (o == NULL) {
		fprintf(stderr, "NULL value for %s\n", token);
		return 0;
	}
	if (o->satisfied) {
		*result = o->result;
		return 1;
	}
	return 0;
}

static int satisfy_next(struct ehht_key_s key, void *each_val, void *context)
{
	struct ehht_s *wires;
	struct op_s *a, *b;
	int l, r;
	char err_buf[80];

	l = r = 0;
	wires = (struct ehht_s *)context;
	a = (struct op_s *)each_val;

	if (a == NULL) {
		fprintf(stderr, "NULL value for %s\n",
			key.len ? key.str : "context");
		return 1;
	}
	if (a->satisfied) {
		return 0;
	}

	if (0) {
		fprintf(stderr, "(%s)\n", key.str);
	}
	switch (a->op) {
		/* OP_NOP */
	case OP_LOAD:
		a->result = atoi(a->in1);
		a->satisfied = 1;
		if (0) {
			to_string_op_s(a, err_buf, 80);
			fprintf(stderr, "LOAD: %s\n", err_buf);
		}
		break;
	case OP_MOVE:
		b = wires->get(wires, a->in1, strnlen(a->in1, TOKEN_BUF_LEN));
		if (b == NULL) {
			to_string_op_s(a, err_buf, 80);
			fprintf(stderr, "NULL value for %s (%s)\n", a->in1,
				err_buf);
			return 1;
		}
		if (b->satisfied) {
			a->result = b->result;
			a->satisfied = 1;
			if (0) {
				to_string_op_s(a, err_buf, 80);
				fprintf(stderr, "MOVE: %s\n", err_buf);
			}
		}
		break;
	case OP_AND:
		if (token_to_result(wires, a->in1, &l)
		    && token_to_result(wires, a->in2, &r)) {
			a->result = (l & r);
			a->satisfied = 1;
			if (0) {
				to_string_op_s(a, err_buf, 80);
				fprintf(stderr, "AND: %s\n", err_buf);
			}
		}
		break;
	case OP_OR:
		if (token_to_result(wires, a->in1, &l)
		    && token_to_result(wires, a->in2, &r)) {
			a->result = (l | r);
			a->satisfied = 1;
			if (0) {
				to_string_op_s(a, err_buf, 80);
				fprintf(stderr, "OR: %s\n", err_buf);
			}
		}
		break;
	case OP_NOT:
		if (token_to_result(wires, a->in1, &l)) {
			a->result = ~l;
			a->satisfied = 1;
			if (0) {
				to_string_op_s(a, err_buf, 80);
				fprintf(stderr, "NOT: %s\n", err_buf);
			}
		}
		break;
	case OP_LSHIFT:
		if (token_to_result(wires, a->in1, &l)) {
			a->result = l << atoi(a->in2);
			a->satisfied = 1;
			if (0) {
				to_string_op_s(a, err_buf, 80);
				fprintf(stderr, "LSHIFT: %s\n", err_buf);
			}
		}
		break;
	case OP_RSHIFT:
		if (token_to_result(wires, a->in1, &l)) {
			a->result = l >> atoi(a->in2);
			a->satisfied = 1;
			if (0) {
				to_string_op_s(a, err_buf, 80);
				fprintf(stderr, "RSHIFT: %s\n", err_buf);
			}
		}
		break;
		/* OP_HALT, */
		/* OP_INVALID */
	default:
		fprintf(stderr, "op %i not implemented (%s)\n", a->op, a->out);
		return 1;
	}

	return 0;
}

static int set_null_and_free(struct ehht_key_s key, void *each_val,
			     void *context)
{
	struct ehht_s *wires;
	int err;

	wires = context;
	err = 0;
	wires->put(wires, key.str, key.len, NULL, &err);
	if (err) {
		fprintf(stderr, "put(%s) failed\n", key.str);
		exit(EXIT_FAILURE);
	}
	free_op_s((struct op_s *)each_val);
	return 0;
}

int main(int argc, char **argv)
{
	const char *path;
	FILE *input;
	struct ehht_s *wires;
	struct op_s *ops;
	char buf[80];
	struct op_s *a;
	size_t i;
	int end;
	int err;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	wires = ehht_new();
	while (fgets(buf, 80, input)) {
		trimstr(buf, 80);
		ops = parse_token(buf, 80);
		if (ops) {
			if (0) {
				fprintf(stderr, "(%s)\n", ops->out);
			}
			err = 0;
			wires->put(wires, ops->out,
				   strnlen(ops->out, TOKEN_BUF_LEN), ops, &err);
			if (err) {
				fprintf(stderr, "put(%s) failed\n", ops->out);
				exit(EXIT_FAILURE);
			}
		}
	}
	fclose(input);

	if (argc > 2) {
		a = wires->get(wires, "b", 1);
		free(a->in1);
		a->in1 = strdup(argv[2]);
	}

	a = wires->get(wires, "a", 1);
	if (a == NULL) {
		fprintf(stderr, "no element 'a'\n");
		wires->for_each(wires, to_string_each, NULL);
		return 1;
	}

	end = 0;
	for (i = 0; (!end) && (!a->satisfied) && (i < wires->size(wires)); ++i) {
		end = wires->for_each(wires, satisfy_next, wires);
	}

	if (a->satisfied) {
		printf("%d\n", a->result);
	} else {
		fprintf(stderr, "a: unsatisfied\n");
		wires->for_each(wires, to_string_each, NULL);
		printf("a: unsatisfied\n");
		return 1;
	}

	wires->for_each(wires, set_null_and_free, wires);
	ehht_free(wires);

	return 0;
}
