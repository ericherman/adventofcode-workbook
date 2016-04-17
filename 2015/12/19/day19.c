#include <stdio.h>
#include <stdlib.h>		/* malloc exit qsort */
#include <string.h>		/* memcpy */
#include <ehht.h>		/* github.com/ericherman/libehht */
#include <ehstr.h>		/* github.com/ericherman/libehstr */

#define BIG_BUF_LEN 4095
#define BUF_LEN 255
#define MAKE_VALGRIND_HAPPY 1

struct buf_pos_len_s {
	char *buf;
	size_t pos;
	size_t len;
};

int free_tables(struct ehht_key_s key, void *val, void *context)
{
	struct ehht_s *table, *subs;

	subs = val;
	ehht_free(subs);

	table = context;
	table->put(table, key.str, key.len, NULL);

	return 0;
}

int fill_buf(struct ehht_key_s key, void *val, void *context)
{
	struct ehht_s *subs;
	struct buf_pos_len_s *bpl;
	int chars;

	subs = val;
	bpl = context;

	chars =
	    snprintf(bpl->buf + bpl->pos, bpl->len - bpl->pos, "%s => ",
		     key.len > 0 ? key.str : "NULL");
	if (chars < 0) {
		fprintf(stderr, "oops\n");
		return chars;
	}
	bpl->pos += chars;

	chars = subs->to_string(subs, bpl->buf + bpl->pos, bpl->len - bpl->pos);
	if (chars < 0) {
		fprintf(stderr, "oops\n");
		return chars;
	}
	bpl->pos += chars;

	chars = snprintf(bpl->buf + bpl->pos, bpl->len - bpl->pos, "\n");
	if (chars < 0) {
		fprintf(stderr, "oops\n");
		return chars;
	}
	bpl->pos += chars;

	return 0;
}

struct prefix_remainder_subs_s {
	const char *molecule;
	size_t prefix_to;
	size_t postfix_from;
	struct ehht_s *table;
	struct ehht_s *perms;
	int verbose;
};

/* malloc or die */
void *mallocd(size_t size)
{
	void *ptr;

	ptr = malloc(size);
	if (!ptr) {
		fprintf(stderr, "Could not allocate %lu bytes.\n",
			(unsigned long)size);
		exit(EXIT_FAILURE);
	}
	return ptr;
}

char *substitute(const char *orig, const char *subs, size_t prefix_to,
		 size_t postfix_from, int verbose)
{
	char *perm, *prefix, *postfix;
	size_t perm_len, postfix_len;

	prefix = mallocd(sizeof(char) * (prefix_to + 1));
	strncpy(prefix, orig, prefix_to);
	prefix[prefix_to] = '\0';

	postfix_len = strlen(orig);
	postfix = mallocd(sizeof(char) * (postfix_len + 1));
	strncpy(postfix, orig + postfix_from, postfix_len);
	postfix[postfix_len] = '\0';

	if (verbose > 2) {
		printf("\tprefix:'%s'\n", prefix);
		printf("\tto:'%s'\n", subs);
		printf("\tpostfix:'%s'\n", postfix);
	}

	perm_len = 1 + strlen(prefix) + strlen(subs) + strlen(postfix);
	perm = mallocd(sizeof(char) * (perm_len + 1));
	snprintf(perm, perm_len, "%s%s%s", prefix, subs, postfix);

	if (verbose > 1) {
		printf("'%s'\n", perm);
	}

	free(prefix);
	free(postfix);

	return perm;
}

int permute_subs(struct ehht_key_s key, void *val, void *context)
{
	struct prefix_remainder_subs_s *ctx;
	char *perm;

	ctx = context;

	if (val != NULL) {
		fprintf(stderr, "Non-NULL val: %p\n", val);
	}
	if (key.len == 0) {
		fprintf(stderr, "zero key_len\n");
	}

	perm =
	    substitute(ctx->molecule, key.str, ctx->prefix_to,
		       ctx->postfix_from, ctx->verbose);

	ctx->perms->put(ctx->perms, perm, strlen(perm), NULL);

	free(perm);

	return 0;
}

size_t permute(struct ehht_s *table, const char *molecule, struct ehht_s *perms,
	       int verbose)
{
	char *buf;
	struct ehht_s *subs;
	struct ehht_keys_s *ks;
	const char *remainder;
	struct prefix_remainder_subs_s ctx;
	size_t i, pos, len;

	ks = table->keys(table, 0);

	ctx.molecule = molecule;
	ctx.table = table;
	ctx.perms = perms;
	ctx.verbose = verbose;
	len = strlen(molecule);
	for (pos = 0; pos < len; ++pos) {
		remainder = molecule + pos;
		for (i = 0; i < ks->len; ++i) {
			if (strstr(remainder, ks->keys[i].str) == remainder) {
				ctx.prefix_to = pos;
				ctx.postfix_from = pos + ks->keys[i].len;
				subs =
				    table->get(table, ks->keys[i].str,
					       ks->keys[i].len);
				subs->for_each(subs, permute_subs, &ctx);
			}
		}
	}
	table->free_keys(table, ks);

	/* remove no change */
	perms->remove(perms, molecule, strlen(molecule));

	if (verbose > 1) {
		buf = mallocd(sizeof(char) * BIG_BUF_LEN * 100);
		perms->to_string(perms, buf, BIG_BUF_LEN * 100);
		printf("%s\n", buf);
		free(buf);
	}

	return perms->size(perms);
}

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	char *buf, from[BUF_LEN], to[BUF_LEN];
	int matched, verbose;
	struct ehht_s *table, *subs, *perms;
	size_t result, len;
	char *molecule;
	struct buf_pos_len_s bpl;

	verbose = (argc > 1) ? atoi(argv[1]) : 0;
	input_file_name = (argc > 2) ? argv[2] : "input";
	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	table = ehht_new(0, NULL, NULL, NULL, NULL);

	buf = mallocd(sizeof(char) * BIG_BUF_LEN);
	molecule = NULL;
	while (fgets(buf, BIG_BUF_LEN, input)) {
		trimstr(buf, BIG_BUF_LEN);
		matched = sscanf(buf, "%40s => %40s", from, to);
		if (matched == 2) {
			len = strlen(from);
			subs = table->get(table, from, len);
			if (!subs) {
				subs = ehht_new(0, NULL, NULL, NULL, NULL);
				table->put(table, from, len, subs);
			}
			subs->put(subs, to, strlen(to), NULL);
		} else {
			if (strlen(buf) < 3 || molecule) {
				continue;
			}
			len = strlen(buf);
			molecule = mallocd(sizeof(char) * (len + 1));
			molecule = strcpy(molecule, buf);
			if (!molecule) {
				fprintf(stderr, "could not allocate string");
				return 1;
			}
		}
	}
	fclose(input);
	free(buf);
	if (!molecule) {
		fprintf(stderr, "molecule not found\n");
		return 1;
	}
	if (table->size(table) == 0) {
		fprintf(stderr, "substitutions not found\n");
		return 1;
	}

	if (verbose) {
		bpl.buf = mallocd(sizeof(char) * BIG_BUF_LEN);
		bpl.buf[0] = '\0';
		bpl.pos = 0;
		bpl.len = BIG_BUF_LEN;
		table->for_each(table, fill_buf, &bpl);
		printf("%s", bpl.buf);
		free(bpl.buf);
		printf("%s\n", molecule);
	}

	perms = ehht_new(0, NULL, NULL, NULL, NULL);
	result = permute(table, molecule, perms, verbose);
	printf("%lu\n", (unsigned long)result);

	if (MAKE_VALGRIND_HAPPY) {
		free(molecule);
		table->for_each(table, free_tables, table);
		ehht_free(perms);
		ehht_free(table);
	}
	return 0;
}
