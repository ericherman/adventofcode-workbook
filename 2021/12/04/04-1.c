/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 04-1.c 2021 AdventOfCode solution
   Copyright (C) 2021 Eric Herman <eric@freesa.org>
*/
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <eherr.h>
#include <ehstr.h>
#include <deque.h>

struct bingo_square {
	uint8_t num;
	uint8_t marked;
};

struct bingo_card {
	struct bingo_square square[5][5];
	int is_winner;
};

char *bingo_card_to_string(struct bingo_card *card, char *buf, size_t buf_size)
{
	buf[0] = '\0';
	for (size_t y = 0; y < 5; ++y) {
		for (size_t x = 0; x < 5; ++x) {
			if (x > 0) {
				strcat(buf, " ");
			}
			struct bingo_square s = card->square[y][x];
			char square_buf[5];

			if (s.marked) {
				strcat(buf, "_");
			} else {
				strcat(buf, " ");
			}

			sprintf(square_buf, "%02d", s.num);
			strcat(buf, square_buf);

			if (s.marked) {
				strcat(buf, "_");
			} else if (x < 4) {
				strcat(buf, " ");
			}
			if (x == 4) {
				strcat(buf, "\n");
			}
		}
	}
	if (card->is_winner) {
		strcat(buf, "Bingo!\n");
	}
	return buf;
}

static int _bingo_card_is_winner(struct bingo_card *card)
{
	uint8_t rows[5] = { 0, 0, 0, 0, 0 };
	uint8_t cols[5] = { 0, 0, 0, 0, 0 };
	for (size_t y = 0; y < 5; ++y) {
		for (size_t x = 0; x < 5; ++x) {
			if (card->square[y][x].marked) {
				++(rows[y]);
				++(cols[x]);
			}
		}
		if (rows[y] == 5) {
			return 1;
		}
	}
	for (size_t i = 0; i < 5; ++i) {
		if (cols[i] == 5) {
			return 1;
		}
	}

	int use_diagonals = 0;
	if (use_diagonals) {
		uint8_t diag1 = 0;
		uint8_t diag2 = 0;
		for (size_t i = 0; i < 5; ++i) {
			if (card->square[i][i].marked) {
				++diag1;
			}
			if (card->square[i][4 - i].marked) {
				++diag2;
			}
		}
		if (diag1 == 5 || diag2 == 5) {
			return 1;
		}
	}
	return 0;
}

int bingo_card_mark(struct bingo_card *card, uint8_t val)
{
	int found = 0;
	for (size_t y = 0; y < 5; ++y) {
		for (size_t x = 0; x < 5; ++x) {
			if (card->square[y][x].num == val) {
				++(card->square[y][x].marked);
				++found;
			}
		}
	}
	if (found) {
		card->is_winner = _bingo_card_is_winner(card);
	}
	return card->is_winner;
}

uint32_t bingo_card_sum_unmarked(struct bingo_card *card)
{
	uint32_t sum_unmarked = 0;
	for (size_t y = 0; y < 5; ++y) {
		for (size_t x = 0; x < 5; ++x) {
			if (card->square[y][x].marked == 0) {
				sum_unmarked += card->square[y][x].num;
			}
		}
	}
	return sum_unmarked;
}

struct card_iterator_context {
	uint8_t val;
	struct bingo_card *winner;
};

int card_iterator_func(deque_s *d, void *each, void *context)
{
	struct bingo_card *card = each;
	struct card_iterator_context *ctx = context;

	if (bingo_card_mark(card, ctx->val)) {
		ctx->winner = card;
	}

	char buf[200];
	printf("%s\n", bingo_card_to_string(card, buf, 200));

	return 0;
}

int main(int argc, char **argv)
{
	const char *path = (argc > 1) ? argv[1] : "input";
	FILE *input = fopen(path, "r");
	if (!input) {
		Die("could not open %s", path);
	}
	const size_t buf_size = 500;
	char *buf = Calloc_or_die(buf_size, 1);

	fgets(buf, buf_size, input);
	chomp_crlf(buf, buf_size);

	size_t numbers_size = 2 + strnlen(buf, buf_size) / 2;
	uint8_t *numbers = Calloc_or_die(numbers_size, 1);
	size_t numbers_len = 0;
	for (size_t i = 0; i < strnlen(buf, buf_size); ++i) {
		if (buf[i] >= '0' && buf[i] <= '9') {
			numbers[numbers_len] *= 10;
			numbers[numbers_len] += (buf[i] - '0');
		} else if (buf[i] == ',') {
			++numbers_len;
		} else {
			Die("unexpected buf[%zu] == '%c'\n", i, buf[i]);
		}
	}
	++numbers_len;

	deque_s *cards = deque_new();
	if (!cards) {
		Die("deque_new() returned NULL");
	}

	size_t row = 0;
	int matched = 0;
	struct bingo_card *card = NULL;
	unsigned a, b, c, d, e;
	while ((matched =
		fscanf(input, "%u %u %u %u %u", &a, &b, &c, &d, &e)) != EOF) {
		if (matched) {
			if (!card) {
				size_t card_size = sizeof(struct bingo_card);
				card = Calloc_or_die(1, card_size);
			}

			card->square[row][0].num = a;
			card->square[row][1].num = b;
			card->square[row][2].num = c;
			card->square[row][3].num = d;
			card->square[row][4].num = e;

			if (row == 4) {
				cards->push(cards, card);
				card = NULL;
				row = 0;
			} else {
				++row;
			}
		}
	}
	fclose(input);
	assert(b == NULL);

	struct card_iterator_context ctx = { 0, NULL };
	for (size_t i = 0; !ctx.winner && i < numbers_len; ++i) {
		ctx.val = numbers[i];
		printf("\nNumber: %u\n", (unsigned)ctx.val);
		cards->for_each(cards, card_iterator_func, &ctx);
	}
	unsigned long answer = 0;
	if (ctx.winner) {
		answer = ctx.val * bingo_card_sum_unmarked(ctx.winner);
	}

	while (cards->size(cards)) {
		card = cards->shift(cards);
		free(card);
	}
	deque_free(cards);

	printf("%lu\n", answer);

	return EXIT_SUCCESS;
}
