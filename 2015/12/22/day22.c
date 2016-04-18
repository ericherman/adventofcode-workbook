#include <stdio.h>		/* printf */
#include <stdlib.h>		/* atoi */
#include <limits.h>		/* INT_MIN INT_MAX */
#include <string.h>		/* strcmp */

#define BUF_LEN 255

#define MAX_ACTIVE_SPELLS 10
struct spell_s {
	const int id;
	const char *name;
	const int mana_cost;
	const int damage;
	const int armor;
	const int heal;
	const int charge_mana;
	const int turns;
};

#define MISSLE_ID 0
#define DRAIN_ID 1
#define SHIELD_ID 2
#define POISON_ID 3
#define CHARGE_ID 4
const struct spell_s missle = { MISSLE_ID, "Missle", 53, 4, 0, 0, 0, 0 };
const struct spell_s drain = { DRAIN_ID, "Drain", 73, 2, 0, 2, 0, 0 };
const struct spell_s shield = { SHIELD_ID, "Shield", 113, 0, 7, 0, 0, 6 };
const struct spell_s poison = { POISON_ID, "Poison", 173, 3, 0, 0, 0, 6 };
const struct spell_s charge = { CHARGE_ID, "Charge", 229, 0, 0, 0, 101, 5 };

const struct spell_s *spellbook[] = {
	&missle,
	&drain,
	&shield,
	&poison,
	&charge,
	NULL
};

struct monster_s {
	int hp;
	int damage;
};

struct player_s {
	int hp;
	int mana;
	int mana_spent;
	int shield_turns;
	int poison_turns;
	int charge_turns;
};

int apply_effects(char *log, struct player_s *u, struct monster_s *m,
		  int verbose)
{
	int shielded;
	if (u->poison_turns) {
		m->hp -= poison.damage;
		--(u->poison_turns);
		if (verbose > 1) {
			sprintf(log + strlen(log),
				"\tpoison deals %d damage (mon hp: %d)"
				", timer: %d\n",
				poison.damage, m->hp, u->poison_turns);
		}
	}

	if (u->charge_turns) {
		u->mana += charge.charge_mana;
		--(u->charge_turns);
		if (verbose > 1) {
			sprintf(log + strlen(log),
				"\tmana charges %d, timer: %d\n",
				charge.charge_mana, u->charge_turns);
		}
	}

	if (u->shield_turns) {
		shielded = 1;
		--(u->shield_turns);
		if (verbose > 1) {
			sprintf(log + strlen(log),
				"\tshield active %d, timer: %d\n", shield.armor,
				u->shield_turns);
		}
	} else {
		shielded = 0;
	}

	return shielded;
}

void fight(const char *pastlog, int depth, struct player_s u,
	   const struct spell_s *spell, struct monster_s m, int *least_mana,
	   int verbose)
{
	size_t i;
	int damage;
	int shielded;
	char *log;

	log = malloc(strlen(pastlog) + 4096);
	if (!log) {
		fprintf(stderr, "could not allocate log space\n");
		exit(EXIT_FAILURE);
	}
	sprintf(log, "%s", pastlog);

	++depth;
	if (verbose > 1) {
		sprintf(log + strlen(log),
			"%d U HP: %4d, Mana: %d, Spent: %d, Spell: %s\n",
			depth, u.hp, u.mana, u.mana_spent, spell->name);
		sprintf(log + strlen(log), "%d M HP: %4d, Damage: %d\n", depth,
			m.hp, m.damage);
	}

	/* player turn */
	apply_effects(log, &u, &m, verbose);

	if (spell->id == SHIELD_ID && u.shield_turns > 1) {
		if (verbose > 2) {
			sprintf(log + strlen(log), "\t\talready shielded\n");
			sprintf(log + strlen(log), "\tBacktrack\n");
		}
		goto print_log_free_and_return;
	}
	if (spell->id == POISON_ID && u.poison_turns > 1) {
		if (verbose > 2) {
			sprintf(log + strlen(log), "\t\talready poisoning\n");
			sprintf(log + strlen(log), "\tBacktrack\n");
		}
		goto print_log_free_and_return;
	}
	if (spell->id == CHARGE_ID && u.charge_turns > 1) {
		if (verbose > 2) {
			sprintf(log + strlen(log), "\t\talready recharging\n");
			sprintf(log + strlen(log), "\tBacktrack\n");
		}
		goto print_log_free_and_return;
	}

	if (spell->mana_cost > u.mana) {
		if (verbose > 2) {
			sprintf(log + strlen(log),
				"\tspell->mana_cost (%d) > u.mana (%d)\n",
				spell->mana_cost, u.mana);
			sprintf(log + strlen(log), "\tBacktrack\n");
		}
		goto print_log_free_and_return;
	}

	u.mana -= spell->mana_cost;
	u.mana_spent += spell->mana_cost;
	if (verbose > 2) {
		sprintf(log + strlen(log),
			"\tu.mana_spent (%d), *least_mana (%d)\n", u.mana_spent,
			*least_mana);
	}
	if (u.mana_spent >= *least_mana) {
		if (verbose > 2) {
			sprintf(log + strlen(log),
				"\tu.mana_spent (%d) >= *least_mana (%d)\n",
				u.mana_spent, *least_mana);
			sprintf(log + strlen(log), "\tBacktrack\n");
		}
		goto print_log_free_and_return;
	}

	switch (spell->id) {
	case MISSLE_ID:
		m.hp -= missle.damage;
		if (verbose > 1) {
			sprintf(log + strlen(log),
				"\tmagic_missle.damage: %d\n", missle.damage);
		}
		break;
	case DRAIN_ID:
		m.hp -= drain.damage;
		u.hp += drain.heal;
		if (verbose > 1) {
			sprintf(log + strlen(log),
				"\tdrain.damage: %d, drain.heal %d\n",
				drain.damage, drain.heal);
		}
		break;
	case SHIELD_ID:
		u.shield_turns += shield.turns;
		if (verbose > 1) {
			sprintf(log + strlen(log), "\tcast shield (%d turns)\n",
				u.shield_turns);
		}
		break;
	case POISON_ID:
		u.poison_turns += poison.turns;
		if (verbose > 1) {
			sprintf(log + strlen(log), "\tcast poison (%d turns)\n",
				u.poison_turns);
		}
		break;
	case CHARGE_ID:
		u.charge_turns += charge.turns;
		if (verbose > 1) {
			sprintf(log + strlen(log), "\tcast charge (%d turns)\n",
				u.charge_turns);
		}
		break;
	default:
		fprintf(stderr, "bad spell id %lu\n", (unsigned long)spell->id);
		exit(EXIT_FAILURE);
	}

	/* moster turn */
	shielded = apply_effects(log, &u, &m, verbose);
	if (m.hp < 1) {
		if (u.mana_spent < *least_mana) {
			*least_mana = u.mana_spent;
		}
		if (verbose) {
			sprintf(log + strlen(log), "WIN: u.mana_spent: %d\n",
				u.mana_spent);
		}
		goto print_log_free_and_return;
	}
	damage = m.damage - ((shielded) ? shield.armor : 0);
	damage = (damage > 0) ? damage : 1;
	u.hp -= damage;
	if (verbose > 2) {
		sprintf(log + strlen(log), "\tBoss attack for %d, result: %d\n",
			damage, u.hp);
	}
	if (u.hp < 1) {
		if (verbose > 1) {
			sprintf(log + strlen(log), "LOSE\n");
		}
		goto print_log_free_and_return;
	}

	/* recurse */
	for (i = 0; spellbook[i]; ++i) {
		if (spellbook[i]->id == SHIELD_ID && u.shield_turns > 2) {
			if (verbose > 3) {
				sprintf(log + strlen(log),
					"%d skipping spell %s\n", depth,
					spellbook[i]->name);
			}
			continue;
		}
		if (spellbook[i]->id == POISON_ID && u.poison_turns > 2) {
			if (verbose > 3) {
				sprintf(log + strlen(log),
					"%d skipping spell %s\n", depth,
					spellbook[i]->name);
			}
			continue;
		}
		if (spellbook[i]->id == CHARGE_ID && u.charge_turns > 2) {
			if (verbose > 3) {
				sprintf(log + strlen(log),
					"%d skipping spell %s\n", depth,
					spellbook[i]->name);
			}
			continue;
		}

		if (verbose > 2) {
			sprintf(log + strlen(log), "%d trying spell %s\n",
				depth, spellbook[i]->name);
		}
		fight(log, depth, u, spellbook[i], m, least_mana, verbose);
	}
      print_log_free_and_return:
	printf("%s", log);
	free(log);
}

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	char buf[BUF_LEN];
	int matched, verbose;
	char stat[BUF_LEN], statb[BUF_LEN];
	int val, least_mana;
	struct monster_s m;
	struct player_s u;
	size_t i;

	verbose = (argc > 1) ? atoi(argv[1]) : 0;
	input_file_name = (argc > 2) ? argv[2] : "input";
	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	m.hp = INT_MIN;
	m.damage = INT_MIN;
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
				}
			}
		}
	}
	fclose(input);
	if (verbose) {
		printf("m.hp: %d, m.damage: %d\n", m.hp, m.damage);
	}
	if (m.hp == INT_MIN || m.damage == INT_MIN) {
		fprintf(stderr, "bad input\n");
		return 1;
	}

	u.hp = 50;
	u.mana = 500;
	u.mana_spent = 0;
	u.shield_turns = 0;
	u.poison_turns = 0;
	u.charge_turns = 0;

	least_mana = INT_MAX;

	for (i = 0; spellbook[i]; ++i) {
		fight("", 0, u, spellbook[i], m, &least_mana, verbose);
	}

	if (least_mana == INT_MAX) {
		printf("no victory\n");
		exit(EXIT_FAILURE);
	}
	printf("%d\n", least_mana);

	return 0;
}
