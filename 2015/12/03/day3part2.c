/* SPDX-License-Identifier: GPL-3.0-or-later */
/* day3part2.c 2015 AdventOfCode solution
   Copyright (C) 2015, 2016, 2019 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include "ehht.h"		/* github.com/ericherman/libehht */

struct house_s {
	int x;
	int y;
	unsigned presents;
};

static struct house_s *new_house(int x, int y)
{
	struct house_s *house = malloc(sizeof(struct house_s));
	if (house == NULL) {
		exit(EXIT_FAILURE);
	}
	house->presents = 0;
	house->x = x;
	house->y = y;
	return house;
}

#define EKH_HOUSE_KEY_BUF_LEN 80

static unsigned int make_key(char *key, int x, int y)
{
	int rv;

	rv = snprintf(key, EKH_HOUSE_KEY_BUF_LEN - 1, "[%d,%d]", x, y);
	if (rv < 5 || key[rv - 1] != ']') {
		fprintf(stderr, "Error: characters printed: %d (key:'%s')\n",
			rv, key);
		return 0;
	}

	return (unsigned int)rv;
}

static int null_and_free(struct ehht_key key, void *each_val, void *context)
{
	struct ehht *houses_1 = (struct ehht *)context;
	int err = 0;
	houses_1->put(houses_1, key.str, key.len, NULL, &err);
	if (err) {
		fprintf(stderr, "put(%s) failed.\n", key.str);
	}
	free(each_val);
	return err;
}

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	int c, x, y, ax, ay, bx, by, ignore;
	struct house_s *house;
	struct ehht *houses;
	char key[EKH_HOUSE_KEY_BUF_LEN];
	unsigned a_or_b, total_presents, max_presents;
	size_t key_len;
	int err;

	if (argc > 1) {
		path = argv[1];
	}

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	houses = ehht_new();
	house = new_house(0, 0);
	ax = 0;
	ay = 0;
	bx = 0;
	by = 0;
	house->presents = 2;
	total_presents = 2;
	max_presents = 2;
	a_or_b = 0;

	key_len = make_key(key, house->x, house->y);
	err = 0;
	houses->put(houses, key, key_len, house, &err);
	if (err) {
		fprintf(stderr, "put(%s) failed.\n", key);
		return 1;
	}

	while ((c = fgetc(input)) != EOF) {
		if ((a_or_b % 2) == 0) {
			x = ax;
			y = ay;
		} else {
			x = bx;
			y = by;
		}
		ignore = 0;
		switch (c) {
		case '^':
			x += 1;
			break;
		case 'v':
			x -= 1;
			break;
		case '<':
			y -= 1;
			break;
		case '>':
			y += 1;
			break;
		default:
			ignore = 1;
			break;
		}
		if (!ignore) {
			total_presents++;
			key_len = make_key(key, x, y);
			house =
			    (struct house_s *)houses->get(houses, key, key_len);
			if (house == NULL) {
				house = new_house(x, y);
				err = 0;
				houses->put(houses, key, key_len, house, &err);
				if (err) {
					fprintf(stderr, "put(%s) failed.\n",
						key);
					return 1;
				}
			}
			house->presents += 1;
			if (house->presents > max_presents) {
				max_presents = house->presents;
			}
			if ((a_or_b % 2) == 0) {
				ax = x;
				ay = y;
			} else {
				bx = x;
				by = y;
			}
			++a_or_b;
		}
	}

	fclose(input);

	printf("houses=%lu (of %u, max presents: %u)\n",
	       (unsigned long)houses->size(houses), total_presents,
	       max_presents);
	printf("%lu\n", (unsigned long)houses->size(houses));

	houses->for_each(houses, null_and_free, houses);
	houses->clear(houses);
	ehht_free(houses);

	return 0;
}
