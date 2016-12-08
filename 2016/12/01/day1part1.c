/* day1.c 2016 AdventOfCode solution
   Copyright (C) 2016 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	char c;
	int x, y, distance, facing;

	if (argc > 1) {
		path = argv[1];
	}

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	x = 0;
	y = 0;
	facing = 0;		/* north */
	distance = 0;

	while ((c = fgetc(input)) != EOF) {
		if ((c >= '0') && (c <= '9')) {
			distance = (distance * 10) + (c - '0');
		} else {
			if (distance) {
				switch (facing) {
				case 0:	/* north */
					y += distance;
					break;
				case 1:	/* east */
					x += distance;
					break;
				case 2:	/* south */
					y -= distance;
					break;
				case 3:	/* west */
					x -= distance;
					break;
				default:
					fprintf(stderr, "facing == %d?\n",
						facing);
					return 1;
				}
				distance = 0;
			}
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

	if (distance) {
		switch (facing) {
		case 0:	/* north */
			y += distance;
			break;
		case 1:	/* east */
			x += distance;
			break;
		case 2:	/* south */
			y -= distance;
			break;
		case 3:	/* west */
			x -= distance;
			break;
		default:
			fprintf(stderr, "facing == %d?\n", facing);
			return 1;
		}
		distance = 0;
	}

	distance = ((x < 0) ? -x : x) + ((y < 0) ? -y : y);

	printf("distance=%d\n", distance);

	return 0;
}
