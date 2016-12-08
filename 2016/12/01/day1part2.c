/* day1part2.c 2016 AdventOfCode solution
   Copyright (C) 2016 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <string.h>
#include <ehht.h>

static int _walk(struct ehht_s *table, int *x, int *y, unsigned distance,
		 int facing)
{
	size_t i;
	char buf[80];
	int found;

	found = 0;
	for (i = 0; i < distance; ++i) {
		switch (facing) {
		case 0:	/* north */
			++(*y);
			break;
		case 1:	/* east */
			++(*x);
			break;
		case 2:	/* south */
			--(*y);
			break;
		case 3:	/* west */
			--(*x);
			break;
		default:
			fprintf(stderr, "facing == %d?\n", facing);
			found = -1;	/* error */
			return found;
		}
		sprintf(buf, "[%d,%d]", *x, *y);
		if (table->has_key(table, buf, strlen(buf))) {
			distance = (((*x) < 0) ? -(*x) : (*x))
			    + (((*y) < 0) ? -(*y) : (*y));
			printf("back at %s, distance: %d\n", buf, distance);
			found = 1;
		}
		table->put(table, buf, strlen(buf), NULL);
	}
	return found;
}

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	char c;
	int found, x, y, distance, facing;
	struct ehht_s *table;

	if (argc > 1) {
		path = argv[1];
	}

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	table = ehht_new(0, NULL, NULL, NULL, NULL);
	if (!table) {
		fprintf(stderr, "could not allocate hashtable\n");
		return 1;
	}

	found = 0;
	x = 0;
	y = 0;
	facing = 0;		/* north */
	distance = 0;

	table->put(table, "[0,0]", 5, NULL);

	while ((!found) && ((c = fgetc(input)) != EOF)) {
		if ((c >= '0') && (c <= '9')) {
			distance = (distance * 10) + (c - '0');
		} else {
			found = _walk(table, &x, &y, distance, facing);
			if (found < 0) {
				goto end_main;
			}
			distance = 0;

			if (c == 'L') {
				--facing;
				if (facing < 0) {
					facing += 4;
				};
			}
			if (c == 'R') {
				++facing;
				if (facing >= 4) {
					facing -= 4;
				};
			}
		}
	}
	fclose(input);

	_walk(table, &x, &y, distance, facing);

end_main:
	if (table) {
		ehht_free(table);
	}

	return found > 0 ? 0 : 1;
}
