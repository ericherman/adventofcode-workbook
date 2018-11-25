/* day9.c 2015 AdventOfCode solution
   Copyright (C) 2015-2016 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>		/* malloc exit */
#include <string.h>		/* strndup */
#include <assert.h>
#include <ehht.h>		/* github.com/ericherman/libehht */
#include <misc.h>

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

static unsigned distance(struct name_list_s *route, struct ehht_s *legs)
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
	name_list->names[name_list->size++] = strndup(key.str, key.len + 1);
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

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	char buf[BUF_LEN], from[NAME_MAX], to[NAME_MAX];
	unsigned dist, best_dist, worst_dist;
	int matched, print_worst;
	struct ehht_s *names;
	struct ehht_s *legs;
	struct leg_s *leg;
	struct name_list_s *tmp_list, *master_name_list;
	size_t i, j, len, combos;

	input_file_name = (argc > 1) ? argv[1] : "input";
	print_worst = (argc > 2) ? atoi(argv[2]) : 0;

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
	if (print_worst) {
		printf("%u\n", worst_dist);
	} else {
		printf("%u\n", best_dist);
	}

	return 0;
}
