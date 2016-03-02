#include <stdio.h>
#include <string.h>		/* strlen */

int main(int argc, char **argv)
{
	const char *path = "input";
	FILE *input;
	char buf[41];
	int matched;
	size_t i, len;
	unsigned vowels, doubles, naughty, nice;

	path = (argc > 1) ? argv[1] : "input";
	input = fopen(path, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", path);
		return 1;
	}

	nice = 0;
	while ((matched = fscanf(input, "%40s", buf)) != EOF) {
		if (matched) {
			vowels = 0;
			doubles = 0;
			naughty = 0;

			len = strlen(buf);
			for (i = 0; i < len; ++i) {
				switch (buf[i]) {
				case 'e':
				case 'i':
				case 'o':
				case 'u':
				case 'a':
					vowels++;
					break;
				default:
					break;
				}

				if (buf[i] == buf[i + 1]) {
					doubles++;
				}

				switch (buf[i]) {
				case 'a':
				case 'c':
				case 'p':
				case 'x':
					if (buf[i + 1] == buf[i] + 1) {
						naughty = 1;
					}
					break;
				default:
					break;
				}
			}
			if (vowels >= 3 && doubles && !naughty) {
				nice++;
			}
		}
	}

	fclose(input);

	printf("total=%u\n", nice);

	return 0;
}
