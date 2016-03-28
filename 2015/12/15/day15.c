#include <stdio.h>		/* printf */
#include <stdlib.h>		/* atoi */
#include <assert.h>

/*
Sprinkles: capacity 5, durability -1, flavor 0, texture 0, calories 5
PeanutButter: capacity -1, durability 3, flavor 0, texture 0, calories 1
Frosting: capacity 0, durability -1, flavor 4, texture 0, calories 6
Sugar: capacity -1, durability 0, flavor 0, texture 2, calories 8
*/

struct cookie_ingredient_s {
	const char *name;
	const int capacity;
	const int durability;
	const int flavor;
	const int texture;
	const int calories;
};

const struct cookie_ingredient_s sprinkles = { "Sprinkles", 5, -1, 0, 0, 5 };
const struct cookie_ingredient_s pbutter = { "PeanutButter", -1, 3, 0, 0, 1 };
const struct cookie_ingredient_s frosting = { "Frosting", 0, -1, 4, 0, 6 };
const struct cookie_ingredient_s sugar = { "Sugar", -1, 0, 0, 2, 8 };

long cscore(unsigned spkl, unsigned pb, unsigned frst, unsigned sug, int cal)
{
	long capacity, durability, flavor, texture, calories;
	long score;

	capacity =
	    sprinkles.capacity * spkl + pbutter.capacity * pb +
	    frosting.capacity * frst + sugar.capacity * sug;
	durability =
	    sprinkles.durability * spkl + pbutter.durability * pb +
	    frosting.durability * frst + sugar.durability * sug;
	flavor =
	    sprinkles.flavor * spkl + pbutter.flavor * pb +
	    frosting.flavor * frst + sugar.flavor * sug;
	texture =
	    sprinkles.texture * spkl + pbutter.texture * pb +
	    frosting.texture * frst + sugar.texture * sug;

	if (cal) {
		calories =
		    sprinkles.calories * spkl + pbutter.calories * pb +
		    frosting.calories * frst + sugar.calories * sug;

		if (calories != cal) {
			return 0;
		}
	}

	if (capacity < 0) {
		capacity = 0;
	}
	if (durability < 0) {
		durability = 0;
	}
	if (flavor < 0) {
		flavor = 0;
	}
	if (texture < 0) {
		texture = 0;
	}

	score = capacity * durability * flavor * texture;

	return score;
}

int main(int argc, char **argv)
{
	unsigned spoons, spkl, pb, frst, sug;
	size_t best_spkl, best_pb, best_frst, best_sug;
	long score, best_score;
	int cal;

	cal = (argc > 1) ? atoi(argv[1]) : 0;
	spoons = (argc > 2) ? atoi(argv[2]) : 100;

	best_score = 0;
	for (frst = 0; frst <= spoons; ++frst) {
		for (spkl = 0; spkl <= (spoons - frst); ++spkl) {
			for (sug = 0; sug <= (spoons - (frst + spkl)); ++sug) {
				for (pb = 0;
				     pb <= (spoons - (frst + spkl + sug));
				     ++pb) {
					score =
					    cscore(spkl, pb, frst, sug, cal);
					if (score > best_score) {
						best_score = score;
						best_spkl = spkl;
						best_pb = pb;
						best_frst = frst;
						best_sug = sug;
					}
				}
			}
		}
	}

	printf("%u %s\n", best_spkl, sprinkles.name);
	printf("%u %s\n", best_pb, pbutter.name);
	printf("%u %s\n", best_frst, frosting.name);
	printf("%u %s\n", best_sug, sugar.name);

	printf("%li %s\n", best_score, "best score");

	return 0;
}
