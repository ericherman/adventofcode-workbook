/* day4.c 2015 AdventOfCode solution
   Copyright (C) 2015-2016 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <string.h>		/* strncpy */
#include <stdlib.h>		/* atoi */

#include "global.h"
#include "md5.h"

#include "ehstr.h"		/* byte_to_hex_chars */

#define is_odd(x) ((x) & 0x01)

/* output len is always 33 */
static void _md5_string_prefix(const char *input, size_t input_len,
			       char *output, size_t output_len)
{
	int i;
	MD5_CTX c;
	unsigned char digest[16];
	size_t offset, bytes, remaining, pos;

	MD5Init(&c);

	offset = 0;
	while (offset < input_len) {
		remaining = (input_len - offset);
		bytes = remaining > 512 ? 512 : remaining;
		MD5Update(&c, input + offset, bytes);
		offset += bytes;
	}

	MD5Final(digest, &c);

	pos = 0;
	for (i = 0; i < 16 && pos < (output_len - 2); ++i, pos += 2) {
		byte_to_hex_chars(digest[i], output + pos, output + pos + 1);
	}
	output[pos] = '\0';
}

int main(int argc, char *argv[])
{
	const char *prefix;
	char input[80];
	char output[32 + 1];
	unsigned i, j, leading_zeros, found, val;
	size_t out_len;

	leading_zeros = (argc > 1) ? (unsigned)atoi(argv[1]) : 5;
	prefix = (argc > 2) ? argv[2] : "iwrupvqb";

	if (leading_zeros < 1) {
		fprintf(stderr, "%u < 1\n", leading_zeros);
		return 1;
	}

	if (leading_zeros > 32) {
		fprintf(stderr, "%u > 32\n", leading_zeros);
		return 1;
	}

	out_len = leading_zeros;
	if (is_odd(out_len)) {
		out_len += 2;	/* max of 33, (31 + 2) */
	} else {
		out_len += 1;	/* max of 33, (32 + 1) */
	}

	found = 0;
	for (i = 0; !found; ++i) {
		snprintf(input, 80, "%s%u", prefix, i);
		_md5_string_prefix(input, strnlen(input, 80), output, out_len);
		found = 1;
		for (j = 0; found && j < leading_zeros; j++) {
			found = found && (output[j] == '0');
		}
		if (found) {
			val = i;
		}
	}
	printf("digest [%s]: %s\n", input, output);
	printf("%u\n", val);

	return (found == 1) ? 0 : 1;
}
