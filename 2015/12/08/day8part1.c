#include <stdio.h>

int main(int argc, char **argv)
{
	const char *path;
	FILE *input;
	char buf[255];
	size_t i;
	unsigned total, chars, escaped, instr;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	total = 0;
	chars = 0;
	escaped = 0;
	instr = 0;
	while (fgets(buf, 255, input)) {
		for (i = 0; buf[i] != '\0' && i < 255; ++i) {
			if (!instr) {
				if (buf[i] == '"') {
					instr = 1;
					++total;
				}
			} else {
				++total;
				if (escaped) {
					++chars;
					escaped = 0;
				} else if (buf[i] == '\\') {
					escaped = 1;
					if (buf[i + 1] == 'x') {
						i += 2;
						total += 2;
					}
				} else if (buf[i] == '"') {
					instr = 0;
				} else {
					++chars;
				}
			}
		}
	}

	fclose(input);

	printf("total: %u, mem chars:%u \n", total, chars);

	return 0;
}
