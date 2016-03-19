#include <ehht.h>		/* github.com/ericherman/libehht */

#include <stdio.h>
#include <stdlib.h>

struct house_s {
	int x;
	int y;
	unsigned presents;
};

struct house_s *new_house(int x, int y)
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

static int null_and_free(const char *each_key, size_t each_key_len,
			 void *each_val, void *context)
{
	struct ehht_s *houses = (struct ehht_s *)context;
	ehht_put(houses, each_key, each_key_len, NULL);
	free(each_val);
	return 0;
}

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	int c, x, y, ignore;
	struct house_s *house;
	struct ehht_s *houses;
	char key[EKH_HOUSE_KEY_BUF_LEN];
	unsigned total_presents, max_presents;
	size_t key_len;

	if (argc > 1) {
		path = argv[1];
	}

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	houses = ehht_new(0, NULL);
	house = new_house(0, 0);
	house->presents = 1;
	total_presents = 1;
	max_presents = 1;

	key_len = make_key(key, house->x, house->y);
	ehht_put(houses, key, key_len, house);

	while ((c = fgetc(input)) != EOF) {
		x = house->x;
		y = house->y;
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
			    (struct house_s *)ehht_get(houses, key, key_len);
			if (house == NULL) {
				house = new_house(x, y);
				ehht_put(houses, key, key_len, house);
			}
			house->presents += 1;
			if (house->presents > max_presents) {
				max_presents = house->presents;
			}
		}
	}

	fclose(input);

	printf("houses=%lu (of %u, max presents: %u)\n",
	       (unsigned long)ehht_size(houses), total_presents, max_presents);

	ehht_foreach_element(houses, null_and_free, houses);
	ehht_clear(houses);
	ehht_free(houses);

	return 0;
}
