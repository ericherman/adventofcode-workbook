#include <stdio.h>
#include <string.h>		/* strncpy */

/* may be a better include */
#include <openssl/md5.h>

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

int main(int argc, char **argv)
{
	const char *prefix;
	char input[80];
	char output[33];
	unsigned i, j, found;

	if (argc > 1) {
		prefix = argv[1];
	} else {
		prefix = "iwrupvqb";
	}
	found = 0;
	for (i = 0; !found; ++i) {
		snprintf(input, 80, "%s%u", prefix, i);
		_md5_string(input, strnlen(input, 80), output);
		found = 1;
		for (j = 0; j < 5; j++) {
			found = found && (output[j] == '0');
		}
	}
	printf("digest [%s]: %s\n", input, output);
	return 0;
}
