#include <stdio.h>

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	int val, total, i, c, lc;

	input_file_name = (argc > 1) ? argv[1] : "input";
	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	val = 0;
	total = 0;
	while ((c = fgetc(input)) != EOF) {
		if (c >= '0' && c <= '9') {
			i = (c - '0');;
			if (val) {
				val *= 10;
				if (val < 0) {
					val -= i;
				} else {
					val += i;
				}
			} else {
				val = i;
				if (lc == '-') {
					val *= -1;
				}
			}
		} else {
			if (val) {
				total += val;
				val = 0;
			}
		}
		lc = c;
	}
	if (val) {
		total += val;
	}

	printf("%d\n", total);

	return 0;
}
