/* SPDX-License-Identifier: GPL-3.0-or-later */
/* 04-2.c 2019 AdventOfCode solution
   Copyright (C) 2019 Eric Herman <eric@freesa.org>
*/
#include <stdio.h>

int contains_two_adjacent(int digits[6])
{
	int i;
	for (i = 1; i < 6; ++i) {
		if (digits[i - 1] == digits[i]) {
			if (i == 1) {
				if (digits[i] != digits[i + 1]) {
					return 1;
				}
			} else if (i == 5) {
				if (digits[i - 2] != digits[i]) {
					return 1;
				}
			} else {
				if ((digits[i] != digits[i + 1]) &&
				    (digits[i - 2] != digits[i])) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int digits_never_decrease(int digits[6])
{
	int i;
	for (i = 1; i < 6; ++i) {
		if (digits[i - 1] > digits[i]) {
			return 0;
		}
	}
	return 1;
}

int main(void)
{
	int from = 402328;
	int to = 864247;
	int i, j, tmp, found;
	int digits[6];

	found = 0;
	for (i = from; i <= to; ++i) {
		tmp = i;
		for (j = 0; j < 6; ++j) {
			digits[5 - j] = tmp % 10;
			tmp = tmp / 10;
		}
		if ((contains_two_adjacent(digits)) &&
		    (digits_never_decrease(digits))) {
			++found;
		}
	}
	printf("%d\n", found);

	return 0;
}
