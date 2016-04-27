#include <stdio.h>
#include <stdlib.h>

static unsigned long long mulull(unsigned long long x, unsigned long long y)
{
	unsigned long long result;
	result = x * y;
	if (x != 0 && result / x != y) {
		fprintf(stderr, "%llu * %llu overflows\n", x, y);
		exit(EXIT_FAILURE);
	}
	return result;
}

static size_t find_pos(size_t col, size_t row)
{
	size_t x, y, maxy, i;
	for (maxy = 1, x = 1, y = 1, i = 1; x < col || y < row; ++i) {
		if (y == 1) {
			x = 1;
			maxy++;
			y = maxy;
		} else {
			++x;
			--y;
		}
	}
	return i;
}

int main(int argc, char **argv)
{
	int verbose;
	size_t row, col, x, y, i, pos;
	unsigned long long first, mul, mod, next;

	verbose = (argc > 1) ? atoi(argv[1]) : 0;
	row = (argc > 2) ? atoi(argv[2]) : 0;
	col = (argc > 3) ? atoi(argv[3]) : 0;

	if (row == 0) {
		row = 3010;
	}
	if (col == 0) {
		col = 3019;
	}

	pos = find_pos(col, row);

	first = 20151125;
	mul = 252533;
	mod = 33554393;

	next = first;

	if (verbose) {
		printf("row: %lu, col: %lu\n", (unsigned long)row,
		       (unsigned long)col);
		printf("pos:%lu\n", (unsigned long)pos);

		printf("   |");
		for (x = 1; x <= 6; ++x) {
			printf("    %u     ", x);
		}
		printf("\n");
		printf("---+");
		for (x = 1; x <= 6; ++x) {
			printf("---------+");
		}
		printf("\n");
		for (y = 1; y <= 6; ++y) {
			printf(" %u |", y);
			for (x = 1; x <= 6; ++x) {
				pos = find_pos(x, y);
				next = first;
				for (i = 1; i < pos; ++i) {
					if (verbose > 1) {
						next = mulull(next, mul) % mod;
					} else {
						next = (next * mul) % mod;
					}
				}
				printf("%9llu ", next);
			}
			printf("\n");
		}
	}
	pos = find_pos(col, row);
	next = first;
	for (i = 1; i < pos; ++i) {
		if (verbose > 1) {
			next = mulull(next, mul) % mod;
		} else {
			next = (next * mul) % mod;
		}
	}

	printf("%llu\n", next);

	return 0;
}
