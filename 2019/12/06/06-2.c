/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 06-1.c 2019 AdventOfCode solution
   Copyright (C) 2019 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ehht.h"		/* github.com/ericherman/libehht */

int direct_steps_apart(struct ehht_s *map, char *to, char *from)
{
	int direct_steps;

	direct_steps = 0;
	while ((from = map->get(map, from, strlen(from))) != NULL) {
		if (strcmp(to, from) == 0) {
			return direct_steps;
		}
		++direct_steps;
	}
	return -1;
}

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	char *line;
	char *rest;
	char *token;
	char *tmp;
	size_t len;
	ssize_t read;
	int matched;
	char l[4];
	char r[4];
	struct ehht_s *map;
	int found, deorbits, steps;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	map = ehht_new(0, NULL, NULL, NULL, NULL);
	line = NULL;
	while ((read = getline(&line, &len, input)) != -1) {
		rest = line;
		while ((token = strtok_r(rest, ",", &rest))) {
			matched = sscanf(token, "%3s)%3s", l, r);
			if (matched) {
				tmp = strdup(l);
				if (!tmp) {
					exit(EXIT_FAILURE);
				}
				printf("%s -> %s\n", r, tmp);
				map->put(map, r, strlen(r), tmp);
			}
		}
	}
	fclose(input);
	free(line);

	deorbits = 0;
	tmp = map->get(map, "YOU", strlen("YOU"));
	found = 0;
	while (!found) {
		steps = direct_steps_apart(map, tmp, "SAN");
		if (steps < 0) {
			tmp = map->get(map, tmp, strlen(tmp));
			deorbits++;
		} else {
			found = 1;
		}
	}
	printf("%d\n", steps + deorbits);

	return 0;
}
