/* day14.c 2015 AdventOfCode solution
   Copyright (C) 2015-2016 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>		/* printf */
#include <stdlib.h>		/* atoi */

/*
Dancer can fly 27 km/s for 5 seconds, but then must rest for 132 seconds.
Cupid can fly 22 km/s for 2 seconds, but then must rest for 41 seconds.
Rudolph can fly 11 km/s for 5 seconds, but then must rest for 48 seconds.
Donner can fly 28 km/s for 5 seconds, but then must rest for 134 seconds.
Dasher can fly 4 km/s for 16 seconds, but then must rest for 55 seconds.
Blitzen can fly 14 km/s for 3 seconds, but then must rest for 38 seconds.
Prancer can fly 3 km/s for 21 seconds, but then must rest for 40 seconds.
Comet can fly 18 km/s for 6 seconds, but then must rest for 103 seconds.
Vixen can fly 18 km/s for 5 seconds, but then must rest for 84 seconds.
*/

struct deerspeed_s {
	const char *name;
	const unsigned kps;
	const unsigned endurance;
	const unsigned rest;

	unsigned char seconds_left;
	unsigned distance;
	unsigned leading;
	unsigned char resting;
};

int main(int argc, char **argv)
{
	size_t i, j, best_distance, best_leading, seconds;
	int best_points;
	struct deerspeed_s *d;
	struct deerspeed_s deer[] = {
		{"Dancer", 27, 5, 132, 0, 0, 0, 0},
		{"Cupid", 22, 2, 41, 0, 0, 0, 0},
		{"Rudolph", 11, 5, 48, 0, 0, 0, 0},
		{"Donner", 28, 5, 134, 0, 0, 0, 0},
		{"Dasher", 4, 16, 55, 0, 0, 0, 0},
		{"Blitzen", 14, 3, 38, 0, 0, 0, 0},
		{"Prancer", 3, 21, 40, 0, 0, 0, 0},
		{"Comet", 18, 6, 103, 0, 0, 0, 0},
		{"Vixen", 18, 5, 84, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0}
	};

	best_points = (argc > 1) ? atoi(argv[1]) : 0;

	seconds = (argc > 2) ? atoi(argv[2]) : 2503;

	for (i = 0; i < 9; ++i) {
		d = &deer[i];
		d->seconds_left = d->endurance;
	}

	for (i = 0; i < seconds; ++i) {
		best_distance = 0;
		for (j = 0; j < 9; ++j) {
			d = &deer[j];
			if (d->resting) {
				--d->seconds_left;
				if (d->seconds_left == 0) {
					d->resting = 0;
					d->seconds_left = d->endurance;
				}
			} else {
				--d->seconds_left;
				d->distance += d->kps;
				if (d->seconds_left == 0) {
					d->resting = 1;
					d->seconds_left = d->rest;
				}
			}
			if (d->distance > best_distance) {
				best_distance = d->distance;
			}
		}
		for (j = 0; j < 9; ++j) {
			d = &deer[j];
			if (d->distance == best_distance) {
				++d->leading;
			}
		}
	}

	best_distance = 0;
	best_leading = 0;
	for (i = 0; i < 9; ++i) {
		d = &deer[i];
		printf("%s: distance: %u leading: %u\n", d->name, d->distance,
		       d->leading);
		if (d->distance > best_distance) {
			best_distance = d->distance;
		}
		if (d->leading > best_leading) {
			best_leading = d->leading;
		}
	}
	for (i = 0; i < 9; ++i) {
		d = &deer[i];
		if (d->distance == best_distance) {
			printf("Distance winner %s: %u\n", d->name,
			       d->distance);
		}
		if (d->leading == best_leading) {
			printf("Leading winner %s: %u\n", d->name, d->leading);
		}
	}

	printf("%lu\n",
	       (unsigned long)(best_points ? best_leading : best_distance));

	return 0;
}
