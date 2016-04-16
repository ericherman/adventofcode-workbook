#include <stdio.h>
#include <stdlib.h>		/* malloc exit */
#include <string.h>		/* strndup */
#include <assert.h>
#include <ehht.h>		/* github.com/ericherman/libehht */

struct leg_s {
	char *from;
	char *to;
	unsigned dist;
};

#define NAME_MAX 40
#define BUF_LEN 255

static struct leg_s *new_leg(char *from, char *to, unsigned dist)
{
	struct leg_s *p;

	p = malloc(sizeof(struct leg_s));
	if (!p) {
		fprintf(stderr, "failed to alloc leg_s (%s -> %s)", from, to);
		exit(EXIT_FAILURE);
	}
	p->from = strndup(from, NAME_MAX + 1);
	p->to = strndup(to, NAME_MAX + 1);
	p->dist = dist;

	return p;
}

static unsigned from_to_key(const char *from, const char *to, char *buf,
			    size_t buf_len)
{
	int s;
	s = snprintf(buf, buf_len, "[%s,%s]", from, to);
	if (s < 0) {
		fprintf(stderr, "could not write key to buf[%lu]\n",
			(unsigned long)buf_len);
		exit(EXIT_FAILURE);
	}
	return (unsigned)s;
}

static unsigned to_key(struct leg_s *p, char *buf, size_t buf_len)
{
	return from_to_key(p->from, p->to, buf, buf_len);
}

struct name_list_s {
	char **names;
	size_t size;
};

unsigned distance(struct name_list_s *route, struct ehht_s *legs)
{
	size_t i, len;
	unsigned distance;
	char buf[BUF_LEN];
	struct leg_s *leg;

	for (distance = 0, i = 0; i < (route->size - 1); ++i) {
		len =
		    from_to_key(route->names[i], route->names[i + 1], buf,
				BUF_LEN);
		leg = legs->get(legs, buf, len);
		if (!leg) {
			fprintf(stderr, "could find %s\n", buf);
			exit(EXIT_FAILURE);
		}
		distance += leg->dist;
	}

	return distance;
}

static int add_name_copy(struct ehht_key_s key, void *each_val, void *context)
{
	struct name_list_s *name_list;
	if (0) {
		fprintf(stderr, "%s %p\n", key.str, each_val);
	}
	name_list = (struct name_list_s *)context;
	name_list->names[name_list->size++] =
	    strndup(key.str, key.len + 1);
	return 0;
}

static struct name_list_s *to_name_list(struct ehht_s *names)
{
	struct name_list_s *name_list;

	name_list = malloc(sizeof(struct name_list_s));
	name_list->names = malloc(sizeof(char *) * names->size(names));
	name_list->size = 0;
	names->for_each(names, add_name_copy, name_list);
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

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	char buf[BUF_LEN], from[NAME_MAX], to[NAME_MAX];
	unsigned dist, best_dist, worst_dist;
	int matched;
	struct ehht_s *names;
	struct ehht_s *legs;
	struct leg_s *leg;
	struct name_list_s *tmp_list, *master_name_list;
	size_t i, j, len, combos;

	input_file_name = (argc > 1) ? argv[1] : "input";
	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	legs = ehht_new(0, NULL, NULL, NULL, NULL);
	names = ehht_new(0, NULL, NULL, NULL, NULL);

	leg = NULL;
	while (fgets(buf, 255, input)) {
		matched = sscanf(buf, "%40s to %40s = %u", from, to, &dist);
		if (matched != 3) {
			fprintf(stderr, "failed to match '%s'\n", buf);
		} else {
			names->put(names, from, strnlen(from, NAME_MAX), NULL);
			names->put(names, to, strnlen(to, NAME_MAX), NULL);

			leg = new_leg(from, to, dist);
			len = to_key(leg, buf, BUF_LEN);
			legs->put(legs, buf, len, leg);

			leg = new_leg(to, from, dist);
			len = to_key(leg, buf, BUF_LEN);
			legs->put(legs, buf, len, leg);
		}
	}
	fclose(input);

	master_name_list = to_name_list(names);
	ehht_free(names);

	combos = factorial(master_name_list->size);
	combos = combos - (combos / 2);	/* the rest are mirror routes */
	printf("%lu unique names, %lu permutations\n",
	       (unsigned long)master_name_list->size, (unsigned long)combos);

	for (i = 0; i < master_name_list->size; ++i) {
		printf("%s\n", master_name_list->names[i]);
	}

	tmp_list = malloc(sizeof(struct name_list_s));
	tmp_list->names = malloc(sizeof(char *) * master_name_list->size);
	tmp_list->size = master_name_list->size;

	best_dist = (unsigned)-1;	/* max_distance */
	worst_dist = 0;
	for (i = 0; i < combos; ++i) {
		permute(i, master_name_list->names, tmp_list->names,
			master_name_list->size, buf, sizeof(char *));
		dist = distance(tmp_list, legs);
		if (dist > worst_dist || dist < best_dist) {
			printf("%u: ", dist);
			for (j = 0; j < tmp_list->size; ++j) {
				printf("%s ", tmp_list->names[j]);
			}
			printf("\n");
			if (dist < best_dist) {
				best_dist = dist;
			} else {
				worst_dist = dist;
			}
		}
	}

	printf("best: %u, worst: %u\n", best_dist, worst_dist);

	return 0;
}
