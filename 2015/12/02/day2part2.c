#include <stdio.h>

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	unsigned int l, w, h, area, smallest_a, smallest_b, total;
	int matched;

	if (argc > 1) {
		path = argv[1];
	}

	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	l = w = h = area = smallest_a = smallest_b = 0;
	while ((matched = fscanf(input, "%ux%ux%u", &l, &w, &h)) != EOF) {
		if (matched == 3) {
			smallest_a = l;
			smallest_b = w;
			area = (w * h);
			if (area < (smallest_a * smallest_b)) {
				smallest_a = w;
				smallest_b = h;
			}
			area = l * h;
			if (area < (smallest_a * smallest_b)) {
				smallest_a = l;
				smallest_b = h;
			}
			total += (2 * smallest_a);
			total += (2 * smallest_b);
			total += (l * h * w);
		}
		l = w = h = area = smallest_a = smallest_b = 0;
	}

	fclose(input);

	printf("total=%u\n", total);

	return 0;
}
