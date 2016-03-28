#include <stdio.h>
#include <string.h>

struct triaits_s {
	const char *key;
	const int val;
};

int main(int argc, char **argv)
{
	const char *input_path, *in;
	FILE *input;
	char buf[255];
	char s[3][40];
	int i, j, m, n, v[3], wrong;

	struct triaits_s traits[] = {
		{"children:", 3},
		{"cats:", 7},
		{"samoyeds:", 2},
		{"pomeranians:", 3},
		{"akitas:", 0},
		{"vizslas:", 0},
		{"goldfish:", 5},
		{"trees:", 3},
		{"cars:", 2},
		{"perfumes:", 1},
	};

	input_path = (argc > 1) ? argv[1] : "input";
	input = fopen(input_path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_path);
		return 1;
	}

	in = "Sue %d: %40s %d, %40s %d, %40s %d";
	while (fgets(buf, 255, input)) {
		m = sscanf(buf, in, &n, s[0], &v[0], s[1], &v[1], s[2], &v[2]);
		if (m != 7) {
			fprintf(stderr, "failed to match '%s'\n", buf);
		} else {
			wrong = 0;
			for (i = 0; i < 3; ++i) {
				for (j = 0; j < 10; ++j) {
					if (strcmp(traits[j].key, s[i]) == 0) {
						if (traits[j].val != v[i]) {
							wrong++;
						}
					}
				}
			}
			if (wrong == 0) {
				printf("%s", buf);
			}
		}
	}

	fclose(input);

	return 0;
}
