/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 06.c 2021 AdventOfCode solution */
/* Copyright (C) 2021 Eric Herman <eric@freesa.org> */

#include <libc-headers.h>

#include <eherr.h>		/* Fopen_or_die */

void fish_clocks_init(uint64_t *fish_clocks, size_t fish_clocks_len,
		      const char *path)
{
	(void)fish_clocks_len;

	FILE *input = Fopen_or_die(path, "r");
	int c = EOF;
	while ((c = fgetc(input)) != EOF) {
		if (c >= '0' && c <= '9') {
			uint8_t n = (c - '0');
			assert(n < fish_clocks_len);
			++fish_clocks[n];
		}
	}
	fclose(input);
}

static void fish_clocks_print(size_t day, uint64_t *fish_clocks, size_t len)
{
#ifndef NDEBUG
	printf("%2zu:", day);
	for (size_t i = 0; i < len; ++i) {
		printf("\t%" PRIu64, fish_clocks[i]);
	}
	printf("\n");
#else
	(void)day;
	(void)fish_clocks;
	(void)len;
#endif
}

void fish_clocks_print_sum(uint64_t *fish_clocks, size_t len)
{
	uint64_t sum = 0;
	for (size_t i = 0; i < len; ++i) {
		sum += fish_clocks[i];
	}
	printf("%" PRIu64 "\n", sum);
}

int main(int argc, char **argv)
{
	const char *path = (argc > 1) ? argv[1] : "input";
	size_t days = (argc > 2) ? (size_t)atoi(argv[2]) : 80;

	const size_t fish_clocks_len = 10;
	const size_t fish_clocks_size = fish_clocks_len * sizeof(uint64_t);
	uint64_t fish_clocks[fish_clocks_len];
	memset(fish_clocks, 0x00, fish_clocks_size);
	uint64_t fish_clocks_next[fish_clocks_len];
	memset(fish_clocks_next, 0x00, fish_clocks_size);

	fish_clocks_init(fish_clocks, fish_clocks_len, path);

	fish_clocks_print(0, fish_clocks, fish_clocks_len);

	for (size_t day = 1; day <= days; ++day) {
		memset(fish_clocks_next, 0x00, fish_clocks_size);
		uint64_t spawn = fish_clocks[0];

		fish_clocks_next[6] = spawn;
		fish_clocks_next[8] = spawn;
		for (size_t i = 1; i < fish_clocks_len; ++i) {
			fish_clocks_next[i - 1] += fish_clocks[i];
		}
		for (size_t i = 0; i < fish_clocks_len; ++i) {
			fish_clocks[i] = fish_clocks_next[i];
		}
		fish_clocks_print(day, fish_clocks, fish_clocks_len);
	}

	fish_clocks_print_sum(fish_clocks, fish_clocks_len);

	return 0;
}
