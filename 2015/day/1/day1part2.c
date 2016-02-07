#include <stdio.h>

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	int floor, c;
	unsigned int i, first_basement;

	if (argc > 1) {
		path = argv[1];
	}

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	i = 0;
	floor = 0;
	first_basement = 0;
	while ((c = fgetc(input)) != EOF) {
		++i;
		switch (c) {
		case '(':
			++floor;
			break;
		case ')':
			--floor;
			break;
		default:	/*ignore */
			break;
		}
		if (floor < 0 && first_basement == 0) {
			first_basement = i;
			printf("first basement index: %u\n", i);
		}
	}

	fclose(input);

	printf("floor=%d\n", floor);

	return 0;
}
