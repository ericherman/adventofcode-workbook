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

/* may be a better include */
#include <openssl/md5.h>

/* output len is always 33 */
static void _md5_string(const char *input, size_t input_len, char *output)
{
	int i;
	MD5_CTX c;
	unsigned char digest[16];
	size_t offset, bytes, remaining;

	MD5_Init(&c);

	offset = 0;
	while (offset < input_len) {
		remaining = (input_len - offset);
		bytes = remaining > 512 ? 512 : remaining;
		MD5_Update(&c, input + offset, bytes);
		offset += bytes;
	}

	MD5_Final(digest, &c);

	for (i = 0; i < 16; ++i) {
		snprintf(output + (i * 2), 3, "%02x", digest[i]);
	}
}

int main(int argc, char *argv[])
{
	const char *prefix;
	char input[80];
	char output[33];
	unsigned i, j, leading_zeros, found;

	leading_zeros = (argc > 1) ? (unsigned)atoi(argv[1]) : 5;
	prefix = (argc > 2) ? argv[2] : "iwrupvqb";

	found = 0;
	for (i = 0; !found; ++i) {
		snprintf(input, 80, "%s%u", prefix, i);
		_md5_string(input, strnlen(input, 80), output);
		found = 1;
		for (j = 0; j < leading_zeros; j++) {
			found = found && (output[j] == '0');
		}
	}
	printf("digest [%s]: %s\n", input, output);

	return (found == 1) ? 0 : 1;
}
