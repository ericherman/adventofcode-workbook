/* SPDX-License-Identifier: GPL-3.0-or-later */
/* day13.c 2015 AdventOfCode solution
   Copyright (C) 2015-2021 Eric Herman <eric@freesa.org>

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
#include <limits.h>
#include <permute.h>

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
	return (size_t)s;
}

static size_t to_key(struct combo_s *p, char *buf, size_t buf_len)
{
	return from_to_key(p->from, p->to, buf, buf_len);
}

struct name_list_s {
	char **names;
	size_t size;
};

static int calc_happiness(struct name_list_s *seating, struct ehht *combos,
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
			combo = combos->get(combos, buf, len);
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

static int add_name_copy(struct ehht_key key, void *each_val, void *context)
{
	struct name_list_s *name_list;
	if (0) {
		fprintf(stderr, "%s %p\n", key.str, each_val);
	}
	name_list = (struct name_list_s *)context;
	name_list->names[name_list->size++] = strndup(key.str, key.len + 1);
	return 0;
}

static struct name_list_s *to_name_list(struct ehht *names)
{
	struct name_list_s *name_list;

	name_list = malloc(sizeof(struct name_list_s));
	name_list->names = malloc(sizeof(char *) * names->size(names));
	name_list->size = 0;
	names->for_each(names, add_name_copy, name_list);
	return name_list;
}

static void trim_at_period(char *buf, size_t len)
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
	struct ehht *names;
	struct ehht *combos;
	struct combo_s *combo;
	struct name_list_s *tmp_list, *master_name_list;
	size_t i, j, len, perms;
	int err;

	input_file_name = (argc > 1) ? argv[1] : "input";
	include_you = (argc > 2) ? atoi(argv[2]) : 0;
	verbose = (argc > 3) ? atoi(argv[3]) : 0;

	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	combos = ehht_new();
	names = ehht_new();
	err = 0;
	if (include_you) {
		names->put(names, "you", strlen("you"), NULL, &err);
		if (err) {
			fprintf(stderr, "put(you) failed\n");
			exit(EXIT_FAILURE);
		}
	}

	combo = NULL;
	while (fgets(buf, 255, input)) {
		matched = sscanf(buf, in, from, sort, &happiness, to);
		if (matched != 4) {
			fprintf(stderr, "failed to match '%s'\n", buf);
		} else {
			trim_at_period(to, NBUF_LEN);
			names->put(names, from, strnlen(from, NBUF_LEN), NULL,
				   &err);
			if (err) {
				fprintf(stderr, "put(%s) failed\n", from);
				exit(EXIT_FAILURE);
			}
			names->put(names, to, strnlen(to, NBUF_LEN), NULL,
				   &err);
			if (err) {
				fprintf(stderr, "put(%s) failed\n", to);
				exit(EXIT_FAILURE);
			}

			if (strcmp("lose", sort) == 0) {
				happiness *= -1;
			}

			combo = new_combo(from, to, happiness);
			len = to_key(combo, buf, BUF_LEN);
			combos->put(combos, buf, len, combo, &err);
			if (err) {
				fprintf(stderr, "put(%s) failed\n", buf);
				exit(EXIT_FAILURE);
			}
			if (verbose > 1) {
				printf("%s -> %s: %d (%p)\n", from, to,
				       happiness, (void *)combo);
			}

			if (include_you) {
				combo = new_combo(from, "you", 0);
				len = to_key(combo, buf, BUF_LEN);
				combos->put(combos, buf, len, combo, &err);
				if (err) {
					fprintf(stderr, "put(%s) failed\n",
						buf);
					exit(EXIT_FAILURE);
				}
				if (verbose > 1) {
					printf("%s -> %s: %d (%p)\n", from, to,
					       happiness, (void *)combo);
				}
				combo = new_combo("you", from, 0);
				len = to_key(combo, buf, BUF_LEN);
				combos->put(combos, buf, len, combo, &err);
				if (err) {
					fprintf(stderr, "put(%s) failed\n",
						buf);
					exit(EXIT_FAILURE);
				}
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

	perms = zfactorial(master_name_list->size);
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
		permute(i, tmp_list->names, master_name_list->names,
			master_name_list->size, sizeof(char *), buf);
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

	if (verbose) {
		printf("best: %d, worst: %d\n", best_happiness,
		       worst_happiness);
	}
	printf("%d\n", best_happiness);

	return 0;
}
