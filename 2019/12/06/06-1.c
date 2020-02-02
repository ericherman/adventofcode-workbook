/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 06-1.c 2019 AdventOfCode solution
   Copyright (C) 2019 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <stdlib.h>		/* malloc */
#include <string.h>		/* memcpy */

#include "deque.h"		/* github.com/ericherman/libdeque */
#include "ehht.h"		/* github.com/ericherman/libehht */

struct mass_pair_s {
	char *parent;
	char *child;
};

int descend(int depth, char *key, struct ehht_s *map)
{
	struct mass_pair_s *pair;
	int total = depth;
	struct deque_s *pair_list;
	int err;

	pair_list = map->get(map, key, strlen(key));
	if (pair_list) {
		while ((pair = pair_list->pop(pair_list)) != NULL) {
			total += descend(depth + 1, pair->child, map);
			free(pair->parent);
			free(pair->child);
			free(pair);
		}
		err = 0;
		map->put(map, key, strlen(key), NULL, &err);
		if (err) {
			fprintf(stderr, "put(%s) failed\n", key);
			exit(EXIT_FAILURE);
		}
		deque_free(pair_list);
	}
	return total;
}

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	char *line;
	char *rest;
	char *token;
	size_t len;
	ssize_t read;
	int matched;
	char l[4];
	char r[4];
	struct ehht_s *map;
	struct deque_s *pair_list;
	struct mass_pair_s *pair;
	size_t depth;
	int totals;
	int err;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	map = ehht_new();
	line = NULL;
	while ((read = getline(&line, &len, input)) != -1) {
		rest = line;
		while ((token = strtok_r(rest, ",", &rest))) {
			matched = sscanf(token, "%3s)%3s", l, r);
			if (matched) {
				pair = calloc(1, sizeof(struct mass_pair_s));
				if (!pair) {
					exit(EXIT_FAILURE);
				}
				pair->parent = strdup(l);
				if (!pair->parent) {
					exit(EXIT_FAILURE);
				}
				pair->child = strdup(r);
				if (!pair->child) {
					exit(EXIT_FAILURE);
				}
				pair_list = map->get(map, l, strlen(l));
				if (!pair_list) {
					pair_list = deque_new();
					if (!pair_list) {
						exit(EXIT_FAILURE);
					}
					err = 0;
					map->put(map, l, strlen(l), pair_list,
						 &err);
					if (err) {
						fprintf(stderr,
							"put(%s) failed\n", l);
						exit(EXIT_FAILURE);
					}
				}
				pair_list->push(pair_list, pair);
			}
		}
	}
	fclose(input);
	free(line);

	depth = 0;
	token = "COM";
	totals = descend(depth, token, map);

	printf("%d\n", totals);

	ehht_free(map);

	return 0;
}
