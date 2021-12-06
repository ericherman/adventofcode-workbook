/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 03-2.c 2021 AdventOfCode solution */
/* Copyright (C) 2021 Eric Herman <eric@freesa.org> */

#include <libc-headers.h>

#include <deque.h>
#include <eherr.h>

static unsigned bstr_to_unsigned(const char *str)
{
	size_t i, len;
	unsigned u;
	len = strlen(str);
	u = 0;
	for (i = 0; i < len; ++i) {
		u = u << 1;
		if (str[i] == '1') {
			++u;
		}
	}
	printf("%s == %u\n", str, u);
	return u;
}

static unsigned reduce(deque_s *all, int keep_larger)
{
	deque_s *keep = deque_new();
	deque_s *next = deque_new();
	if (!keep || !next) {
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < all->size(all); ++i) {
		char *s = all->peek_bottom(all, i);
		if (s) {
			keep->push(keep, s);
		} else {
			exit(EXIT_FAILURE);
		}
	}
	for (size_t pos = 0; keep->size(keep) > 1; ++pos) {
		int ones = 0;
		int zeros = 0;
		for (size_t i = 0; i < keep->size(keep); ++i) {
			char *s = keep->peek_bottom(keep, i);
			if (!s) {
				s = keep->peek_bottom(keep, i);
			}
			assert(s);
			assert(pos < strlen(s));
			if (s[pos] == '1') {
				++ones;
			} else if (s[pos] == '0') {
				++zeros;
			} else {
				exit(EXIT_FAILURE);
			}
		}
		while (keep->size(keep)) {
			char *s = keep->shift(keep);
			if (keep_larger) {
				if (zeros > ones) {
					if (s[pos] == '0') {
						next->push(next, s);
					}
				} else {
					if (s[pos] == '1') {
						next->push(next, s);
					}
				}
			} else {
				if (zeros <= ones) {
					if (s[pos] == '0') {
						next->push(next, s);
					}
				} else {
					if (s[pos] == '1') {
						next->push(next, s);
					}
				}

			}
		}
		while (next->size(next)) {
			keep->push(keep, next->shift(next));
		}
	}
	assert(keep->size(keep) == 1);
	unsigned u = bstr_to_unsigned(keep->shift(keep));
	deque_free(keep);
	deque_free(next);
	return u;
}

int main(int argc, char **argv)
{

	char str[80];
	memset(str, 0x00, 80);

	deque_s *all = deque_new();
	if (!all) {
		exit(EXIT_FAILURE);
	}

	int matched = EOF;
	const char *path = (argc > 1) ? argv[1] : "input";
	FILE *input = Fopen_or_die(path, "r");
	while ((matched = fscanf(input, "%s", str)) != EOF) {
		if (matched) {
			char *s = malloc(strlen(str) + 1);
			strcpy(s, str);
			if (!s) {
				exit(EXIT_FAILURE);
			}
			all->push(all, s);
		}
	}
	fclose(input);

	unsigned o2 = reduce(all, 1);
	unsigned co2 = reduce(all, 0);

	printf("%d\n", o2 * co2);

	while (all->size(all)) {
		free(all->pop(all));
	}
	deque_free(all);

	return EXIT_SUCCESS;
}
