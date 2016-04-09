#include <stdio.h>
#include <deque.h>
#include <stdlib.h>
#include <string.h>

struct depth_counter_s {
	int val;
	int total;
	int has_red;
	int is_array;
	unsigned depth;
};

struct depth_counter_s *new_depth_counter(unsigned depth)
{
	struct depth_counter_s *n;

	n = malloc(sizeof(struct depth_counter_s));
	/* just crash if n == NULL */
	n->val = 0;
	n->total = 0;
	n->has_red = 0;
	n->is_array = 0;
	n->depth = depth;

	return n;
}

#define STR_MAX 255

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	struct deque_s *stack;
	struct depth_counter_s *cnt, *free_me;
	char buf[STR_MAX + 1];
	size_t str_idx;
	char c, lc;
	int i;

	input_file_name = (argc > 1) ? argv[1] : "input";
	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	stack = deque_new(NULL, NULL, NULL);
	cnt = new_depth_counter(0);
	stack->push(stack, cnt);

	buf[0] = '\0';
	str_idx = 0;
	lc = '\0';
	while ((c = fgetc(input)) != EOF) {
		if (str_idx != 0) {
			/* TODO deal with escape sequences */
			if (c == '"') {
				buf[str_idx++] = c;
				buf[str_idx] = '\0';
				if (cnt->is_array == 0
				    && strcmp("\"red\"", buf) == 0) {
					cnt->has_red = 1;
				}
				buf[0] = '\0';
				str_idx = 0;
			} else {
				if (str_idx < STR_MAX) {
					buf[str_idx++] = c;
				} else {
					fprintf(stderr, "str too long\n");
				}
			}
		} else if (c == '"') {
			buf[str_idx++] = c;
		} else if ((c == '{' || c == '[') && str_idx == 0) {
			/* TODO deal with improper nesting, e.g. { [ } ] */
			if (cnt->val) {
				cnt->total += cnt->val;
				cnt->val = 0;
			}
			cnt = new_depth_counter(cnt->depth + 1);
			if (c == '[') {
				cnt->is_array = 1;
			}
			stack->push(stack, cnt);
		} else if ((c == '}' || c == ']') && str_idx == 0) {
			free_me = cnt;
			stack->pop(stack);
			cnt = stack->top(stack);
			if (free_me->has_red == 0) {
				cnt->total += (free_me->val + free_me->total);
			}
			free(free_me);
		} else if (c >= '0' && c <= '9') {
			i = (c - '0');
			if (cnt->val) {
				cnt->val *= 10;
				if (cnt->val < 0) {
					cnt->val -= i;
				} else {
					cnt->val += i;
				}
			} else {
				cnt->val = i;
				if (lc == '-') {
					cnt->val *= -1;
				}
			}
		} else {
			if (cnt->val) {
				cnt->total += cnt->val;
				cnt->val = 0;
			}
		}
		lc = c;
	}

	while (stack->size(stack) > 1) {
		free_me = stack->pop(stack);
		cnt = stack->top(stack);
		if (free_me->has_red == 0) {
			cnt->total += (free_me->val + free_me->total);
		}
		free(free_me);
	}

	cnt = stack->pop(stack);

	if (cnt->val) {
		cnt->total += cnt->val;
		cnt->val = 0;
	}

	printf("%d\n", cnt->total);

	free(cnt);
	deque_free(stack);

	return 0;
}
