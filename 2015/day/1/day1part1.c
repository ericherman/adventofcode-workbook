#include <stdio.h>

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	int floor, c;

	if (argc > 1) {
		path = argv[1];
	}

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	floor = 0;
	while ((c = fgetc(input)) != EOF) {
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
	}

	fclose(input);

	printf("floor=%d\n", floor);

	return 0;
}
