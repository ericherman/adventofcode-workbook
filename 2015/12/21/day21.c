#include <stdio.h>		/* printf */
#include <stdlib.h>		/* atoi */
#include <limits.h>		/* INT_MIN INT_MAX */
#include <string.h>		/* strcmp */

#define BUF_LEN 255

struct item_s {
	const char *name;
	const unsigned cost;
	const unsigned damage;
	const unsigned armor;
};

/* Weapons:    Cost  Damage  Armor */
struct item_s weapons[] = {
	{"Dagger", 8, 4, 0},
	{"Shortsword", 10, 5, 0},
	{"Warhammer", 25, 6, 0},
	{"Longsword", 40, 7, 0},
	{"Greataxe", 74, 8, 0},
	{0, 0, 0, 0}
};

/* Armor:      Cost  Damage  Armor */
struct item_s armors[] = {
	{"None", 0, 0, 0},
	{"Leather", 13, 0, 1},
	{"Chainmail", 31, 0, 2},
	{"Splintmail", 53, 0, 3},
	{"Bandedmail", 75, 0, 4},
	{"Platemail", 102, 0, 5},
	{0, 0, 0, 0}
};

/* Rings:      Cost  Damage  Armor */
struct item_s rings[] = {
	{"None1", 0, 0, 0},
	{"None2", 0, 0, 0},
	{"Damage +1", 25, 1, 0},
	{"Damage +2", 50, 2, 0},
	{"Damage +3", 100, 3, 0},
	{"Defense +1", 20, 0, 1},
	{"Defense +2", 40, 0, 2},
	{"Defense +3", 80, 0, 3},
	{0, 0, 0, 0}
};

char *print_item_s(char *buf, size_t len, struct item_s item)
{
	snprintf(buf, len, "%s: cost: %d damage: %d armor: %d", item.name,
		 item.cost, item.damage, item.armor);
	buf[len - 1] = '\0';
	return buf;
}

struct fighter_s {
	int hp;
	int damage;
	int armor;
};

int fight(struct fighter_s u, struct fighter_s m, int verbose)
{
	int damage;

	if (verbose > 2) {
		printf("U HP: %4d, Damage: %2d, Armor: %2d\n", u.hp, u.damage,
		       u.armor);
		printf("M HP: %4d, Damage: %2d, Armor: %2d\n", m.hp, m.damage,
		       m.armor);
	}

	while (u.hp > 0) {
		damage = u.damage - m.armor;
		damage = (damage > 0) ? damage : 1;
		m.hp -= damage;
		if (verbose > 3) {
			printf("\tM HP: %4d\n", m.hp);
		}
		if (m.hp <= 0) {
			if (verbose > 1) {
				printf("Victor: U\n");
			}
			return 1;
		}
		damage = m.damage - u.armor;
		damage = (damage > 0) ? damage : 1;
		u.hp -= damage;
		if (verbose > 3) {
			printf("\tU HP: %4d\n", u.hp);
		}
	}
	if (verbose > 1) {
		printf("Victor: M\n");
	}
	return 0;
}

void equip_and_fight(struct fighter_s *m, struct fighter_s *u, size_t w,
		     size_t a, size_t r, size_t l, int *u_gold, int verbose)
{
	int cost;
	char buf[BUF_LEN];

	cost = weapons[w].cost;
	cost += armors[a].cost;
	cost += rings[r].cost;
	cost += rings[l].cost;

	u->damage = weapons[w].damage;
	u->damage += armors[a].damage;
	u->damage += rings[r].damage;
	u->damage += rings[l].damage;

	u->armor = weapons[w].armor;
	u->armor += armors[a].armor;
	u->armor += rings[r].armor;
	u->armor += rings[l].armor;

	if (verbose > 2) {
		printf("cost: %d, u_gold: %d\n", cost, *u_gold);
		printf("\t%s\n", print_item_s(buf, BUF_LEN, weapons[w]));
		printf("\t%s\n", print_item_s(buf, BUF_LEN, armors[a]));
		printf("\t%s\n", print_item_s(buf, BUF_LEN, rings[l]));
		printf("\t%s\n", print_item_s(buf, BUF_LEN, rings[r]));
	}

	if (cost < *u_gold) {
		if (verbose > 1) {
			printf("cost: %d\n", cost);
		}
		if (fight(*u, *m, verbose)) {
			*u_gold = cost;
			if (verbose) {
				printf("u_gold: %d\n", *u_gold);
			}
		}
	}
}

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	char buf[BUF_LEN];
	int matched, verbose;
	char stat[BUF_LEN], statb[BUF_LEN];
	int val, u_gold;
	size_t w, a, l, r;
	struct fighter_s u, m;

	verbose = (argc > 1) ? atoi(argv[1]) : 0;
	input_file_name = (argc > 2) ? argv[2] : "input";
	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	m.hp = m.damage = m.armor = INT_MIN;
	while (fgets(buf, BUF_LEN, input)) {
		val = INT_MIN;
		matched = sscanf(buf, "%s %s %d", stat, statb, &val);
		if (matched == 3 && val != INT_MIN) {
			if (verbose > 1) {
				printf("%s %s %d\n", stat, statb, val);
			}
			if (strcmp("Hit", stat) == 0) {
				m.hp = val;
			}
		} else {
			val = INT_MIN;
			matched = sscanf(buf, "%s %d", stat, &val);
			if (matched == 2 && val != INT_MIN) {
				if (verbose > 1) {
					printf("%s %d\n", stat, val);
				}
				if (strcmp("Damage:", stat) == 0) {
					m.damage = val;
				} else if (strcmp("Armor:", stat) == 0) {
					m.armor = val;
				}
			}
		}
	}
	fclose(input);
	if (verbose) {
		printf("m.hp: %d, m.damage: %d, m.armor: %d\n",
		       m.hp, m.damage, m.armor);
	}
	if (m.hp == INT_MIN || m.damage == INT_MIN || m.armor == INT_MIN) {
		fprintf(stderr, "bad input\n");
		return 1;
	}

	u.hp = 100;
	u_gold = INT_MAX;
	for (w = 0; weapons[w].name != NULL; ++w) {
		for (a = 0; armors[a].name != NULL; ++a) {
			for (l = 0; rings[l].name != NULL; ++l) {
				for (r = 0; rings[r].name != NULL; ++r) {
					if (l == r) {
						continue;
					}
					equip_and_fight(&m, &u, w, a, r, l,
							&u_gold, verbose);
				}
			}
		}
	}

	printf("%d\n", u_gold);

	return 0;
}
