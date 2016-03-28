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
	unsigned char resting;
};

int main(int argc, char **argv)
{
	size_t i, j, best, seconds;
	struct deerspeed_s *d;
	struct deerspeed_s deer[] = {
		{"Dancer", 27, 5, 132, 0, 0, 0},
		{"Cupid", 22, 2, 41, 0, 0, 0},
		{"Rudolph", 11, 5, 48, 0, 0, 0},
		{"Donner", 28, 5, 134, 0, 0, 0},
		{"Dasher", 4, 16, 55, 0, 0, 0},
		{"Blitzen", 14, 3, 38, 0, 0, 0},
		{"Prancer", 3, 21, 40, 0, 0, 0},
		{"Comet", 18, 6, 103, 0, 0, 0},
		{"Vixen", 18, 5, 84, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0}
	};

	seconds = (argc > 1) ? atoi(argv[1]) : 2503;
	for (i = 0; i < 9; ++i) {
		d = &deer[i];
		d->seconds_left = d->endurance;
	}

	for (i = 0; i < seconds; ++i) {
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
		}
	}

	for (best = 0, j = 0, i = 0; i < 9; ++i) {
		d = &deer[i];
		printf("%s: %u\n", d->name, d->distance);
		if (d->distance > best) {
			best = d->distance;
			j = i;
		}
	}
	d = &deer[j];
	printf("Winner %s: %u\n", d->name, d->distance);
	return 0;
}
