#include <stdio.h>
#include <stdlib.h>		/* malloc exit */
#include <string.h>		/* strndup */
#include <assert.h>
#include <ehht.h>		/* github.com/ericherman/libehht */
#include <limits.h>

struct combo_s {
	char *from;
	char *to;
	int happiness;
};

#define NBUF_LEN 40
#define BUF_LEN 255

static struct combo_s *new_combo(char *from, char *to, int happiness)
{
	struct combo_s *p;

	p = malloc(sizeof(struct combo_s));
	if (!p) {
		fprintf(stderr, "failed to alloc combo_s (%s -> %s)", from, to);
		exit(EXIT_FAILURE);
	}
	p->from = strndup(from, NBUF_LEN + 1);
	p->to = strndup(to, NBUF_LEN + 1);
	p->happiness = happiness;

	return p;
}

static size_t from_to_key(const char *from, const char *to, char *buf,
			  size_t buf_len)
{
	int s;
	s = snprintf(buf, buf_len, "[%s,%s]", from, to);
	if (s < 0) {
		fprintf(stderr, "could not write key to buf[%lu]\n",
			(unsigned long)buf_len);
		exit(EXIT_FAILURE);
	}
	return (size_t) s;
}

static size_t to_key(struct combo_s *p, char *buf, size_t buf_len)
{
	return from_to_key(p->from, p->to, buf, buf_len);
}

struct name_list_s {
	char **names;
	size_t size;
};

int calc_happiness(struct name_list_s *seating, struct ehht_s *combos,
		   int verbose)
{
	size_t i, j, c[2], len;
	int happiness;
	char buf[BUF_LEN], *from, *to;
	struct combo_s *combo;

	for (happiness = 0, i = 0; i < seating->size; ++i) {
		c[0] = (i == 0) ? (seating->size - 1) : (i - 1);
		c[1] = (i == (seating->size - 1)) ? 0 : (i + 1);

		from = seating->names[i];
		for (j = 0; j < 2; ++j) {
			to = seating->names[c[j]];
			len = from_to_key(from, to, buf, BUF_LEN);
			combo = ehht_get(combos, buf, len);
			if (!combo) {
				fprintf(stderr, "could not find %s\n", buf);
				exit(EXIT_FAILURE);
			}
			happiness += combo->happiness;
		}
	}

	if (verbose > 1) {
		printf("%5d", happiness);
		for (i = 0; i < seating->size; ++i) {
			printf(" %s", seating->names[i]);
		}
		printf("\n");
	}
	return happiness;
}

static int add_name_copy(const char *each_key, size_t each_key_len,
			 void *each_val, void *context)
{
	struct name_list_s *name_list;
	if (0) {
		fprintf(stderr, "%s %p\n", each_key, each_val);
	}
	name_list = (struct name_list_s *)context;
	name_list->names[name_list->size++] =
	    strndup(each_key, each_key_len + 1);
	return 0;
}

static struct name_list_s *to_name_list(struct ehht_s *names)
{
	struct name_list_s *name_list;

	name_list = malloc(sizeof(struct name_list_s));
	name_list->names = malloc(sizeof(char *) * ehht_size(names));
	name_list->size = 0;
	ehht_foreach_element(names, add_name_copy, name_list);
	return name_list;
}

void swap(void *a, void *b, void *buf, size_t element_size)
{
	memcpy(buf, b, element_size);	/* save b */
	memcpy(b, a, element_size);	/* a -> b */
	memcpy(a, buf, element_size);	/* saved b -> a */
}

size_t factorial(size_t n)
{
	size_t result = n;
	if (n < 2) {
		return 1;
	}
	do {
		result *= --n;
	} while (n > 1);
	return result;
}

/* allows caller to fetch a specifically indexed permutation */
void permute(size_t permutation, const void *src, void *dest, size_t len,
	     void *buf, size_t elem_size)
{
	size_t i, sub_perm, n_sub_perms, next_swap;
	unsigned char *d;
	void *a, *b;

	memcpy(dest, src, len * elem_size);
	sub_perm = permutation;
	d = (unsigned char *)dest;	/* allow ptr math */

	for (i = 0; i < len - 1; ++i) {
		n_sub_perms = factorial(len - 1 - i);
		next_swap = sub_perm / n_sub_perms;
		sub_perm = sub_perm % n_sub_perms;
		if (next_swap != 0) {
			a = d + i * elem_size;
			b = d + (i + next_swap) * elem_size;
			swap(a, b, buf, elem_size);
		}
	}
}

void trim_at_period(char *buf, size_t len)
{
	size_t i;

	for (i = 0; buf[i] != '\0' && i < len; ++i) {
		if (buf[i] == '.') {
			buf[i] = '\0';
			return;
		}
	}
}

/*
	Carol would lose 46 happiness units by sitting next to Eric.
	Carol would gain 33 happiness units by sitting next to Frank.
*/
const char *in = "%40s would %4s %d happiness units by sitting next to %40s.";

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	char buf[BUF_LEN], from[NBUF_LEN], sort[5], to[NBUF_LEN];
	int happiness, best_happiness, worst_happiness;
	int matched, include_you, verbose;
	struct ehht_s *names;
	struct ehht_s *combos;
	struct combo_s *combo;
	struct name_list_s *tmp_list, *master_name_list;
	size_t i, j, len, perms;

	include_you = (argc > 1) ? atoi(argv[1]) : 0;
	verbose = (argc > 2) ? atoi(argv[2]) : 0;
	input_file_name = (argc > 3) ? argv[3] : "input";
	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	combos = ehht_new(0, NULL);
	names = ehht_new(0, NULL);
	if (include_you) {
		ehht_put(names, "you", strlen("you"), NULL);
	}

	combo = NULL;
	while (fgets(buf, 255, input)) {
		matched = sscanf(buf, in, from, sort, &happiness, to);
		if (matched != 4) {
			fprintf(stderr, "failed to match '%s'\n", buf);
		} else {
			trim_at_period(to, NBUF_LEN);
			ehht_put(names, from, strnlen(from, NBUF_LEN), NULL);
			ehht_put(names, to, strnlen(to, NBUF_LEN), NULL);

			if (strcmp("lose", sort) == 0) {
				happiness *= -1;
			}

			combo = new_combo(from, to, happiness);
			len = to_key(combo, buf, BUF_LEN);
			ehht_put(combos, buf, len, combo);
			if (verbose > 1) {
				printf("%s -> %s: %d (%p)\n", from, to,
				       happiness, (void *)combo);
			}

			if (include_you) {
				combo = new_combo(from, "you", 0);
				len = to_key(combo, buf, BUF_LEN);
				ehht_put(combos, buf, len, combo);
				if (verbose > 1) {
					printf("%s -> %s: %d (%p)\n", from, to,
					       happiness, (void *)combo);
				}
				combo = new_combo("you", from, 0);
				len = to_key(combo, buf, BUF_LEN);
				ehht_put(combos, buf, len, combo);
				if (verbose > 1) {
					printf("%s -> %s: %d (%p)\n", from, to,
					       happiness, (void *)combo);
				}
			}
		}
	}
	fclose(input);

	master_name_list = to_name_list(names);
	ehht_free(names);

	perms = factorial(master_name_list->size);
	perms = perms - (perms / 2);	/* the rest are mirror seatings */
	printf("%lu unique names, %lu permutations\n",
	       (unsigned long)master_name_list->size, (unsigned long)perms);

	for (i = 0; i < master_name_list->size; ++i) {
		printf("%s\n", master_name_list->names[i]);
	}

	tmp_list = malloc(sizeof(struct name_list_s));
	tmp_list->names = malloc(sizeof(char *) * master_name_list->size);
	tmp_list->size = master_name_list->size;

	best_happiness = INT_MIN;
	worst_happiness = INT_MAX;
	for (i = 0; i < perms; ++i) {
		permute(i, master_name_list->names, tmp_list->names,
			master_name_list->size, buf, sizeof(char *));
		happiness = calc_happiness(tmp_list, combos, verbose);
		if (happiness > best_happiness || happiness < worst_happiness) {
			if (verbose) {
				printf("%d: ", happiness);
				for (j = 0; j < tmp_list->size; ++j) {
					printf("%s ", tmp_list->names[j]);
				}
				printf("\n");
			}
			if (happiness > best_happiness) {
				best_happiness = happiness;
			} else {
				worst_happiness = happiness;
			}
		}
	}

	printf("best: %d, worst: %d\n", best_happiness, worst_happiness);

	return 0;
}
