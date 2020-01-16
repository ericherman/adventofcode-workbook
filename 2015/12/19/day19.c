/* SPDX-License-Identifier: GPL-3.0-or-later */
/* day19.c 2015 AdventOfCode solution
   Copyright (C) 2015, 2016, 2019 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>		/* malloc exit qsort */
#include <string.h>		/* strstr */
#include <limits.h>		/* UINT_MAX */
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

static void construct_molecule(struct ehht_s *table, struct ehht_keys_s *tks,
			       const char *molecule, unsigned depth,
			       unsigned *best_known, int *max_tries,
			       int verbose);

static int free_tables(struct ehht_key_s key, void *val, void *context)
{
	struct ehht_s *table, *subs;

	subs = val;
	ehht_free(subs);

	table = context;
	table->put(table, key.str, key.len, NULL);

	return 0;
}

static int fill_buf(struct ehht_key_s key, void *val, void *context)
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
static void *mallocd(size_t size)
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

static char *substitute(const char *orig, const char *subs, size_t prefix_to,
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

static size_t permute(struct ehht_s *table, const char *molecule,
		      struct ehht_s *perms, int verbose)
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

static int cmp_key_len_asc(const void *a, const void *b)
{
	const struct ehht_key_s *l, *r;
	l = a;
	r = b;

	if (l->len == r->len) {
		return 0;
	}
	return l->len > r->len ? 1 : -1;
}

static int cmp_key_len_dsc(const void *a, const void *b)
{
	return cmp_key_len_asc(b, a);
}

static struct ehht_s *reverse_table(struct ehht_s *table, int verbose)
{
	struct ehht_s *rev, *subs;
	struct ehht_keys_s *ks, *subks;
	int copy_keys;
	size_t i, j;

	rev = ehht_new();

	copy_keys = 0;
	ks = table->keys(table, copy_keys);
	for (i = 0; i < ks->len; ++i) {
		subs = table->get(table, ks->keys[i].str, ks->keys[i].len);
		copy_keys = 0;
		subks = subs->keys(subs, copy_keys);
		for (j = 0; j < subks->len; ++j) {
			subs =
			    rev->get(rev, subks->keys[j].str,
				     subks->keys[j].len);
			if (!subs) {
				subs = ehht_new();
				rev->put(rev, subks->keys[j].str,
					 subks->keys[j].len, subs);
			}
			subs->put(subs, ks->keys[i].str, ks->keys[i].len, NULL);
		}
		subs->free_keys(subs, subks);
	}
	table->free_keys(table, ks);

	if (verbose) {
		copy_keys = 0;
		ks = rev->keys(rev, copy_keys);
		printf("rev size: %lu with: [\n",
		       (unsigned long)rev->size(rev));
		for (i = 0; i < ks->len; ++i) {
			subs = rev->get(rev, ks->keys[i].str, ks->keys[i].len);
			printf("\t'%s' => %p: [ \n", ks->keys[i].str,
			       (void *)subs);
			subs->size(subs);
			copy_keys = 0;
			subks = subs->keys(subs, copy_keys);
			for (j = 0; j < subks->len; ++j) {
				printf("\t\t'%s' => '%s',\n",
				       subks->keys[j].str,
				       (char *)subs->get(subs,
							 subks->keys[j].str,
							 subks->keys[j].len));
			}
			subs->free_keys(subs, subks);
			printf("\t],\n");
		}
		printf("]\n");
		rev->free_keys(rev, ks);
	}
	return rev;
}

static int find_in_substitute(struct ehht_key_s key, size_t prefix_to,
			      size_t postfix_from, struct ehht_s *table,
			      struct ehht_keys_s *tks, const char *molecule,
			      unsigned depth, unsigned *best_known,
			      int *max_tries, int verbose)
{
	char *perm;

	perm = substitute(molecule, key.str, prefix_to, postfix_from, verbose);
	if (strcmp("e", perm) == 0) {
		if (verbose > 1) {
			printf("found:%u %s\n", depth, perm);
		}
		if (depth < *best_known) {
			if (verbose) {
				printf("new best:" "%u %s\n", depth, perm);
			}
			*best_known = depth;
		}
		free(perm);
		return 1;
	} else {
		if (strstr(perm, "e") == NULL) {
			construct_molecule(table, tks, perm, depth, best_known,
					   max_tries, verbose);
		}
	}
	free(perm);
	return 0;
}

static void construct_molecule(struct ehht_s *table, struct ehht_keys_s *tks,
			       const char *molecule, unsigned depth,
			       unsigned *best_known, int *max_tries,
			       int verbose)
{
	struct ehht_s *subs;
	struct ehht_keys_s *sks;
	const char *submol;
	size_t i, j, k, prefix_to, postfix_from;
	struct ehht_key_s key, skey;

	--(*max_tries);
	++depth;
	if (verbose > 2) {
		printf("depth:%u (tries reamining: %d) %s\n", depth, *max_tries,
		       molecule);
	}
	if (((*max_tries < 1) && (*best_known != UINT_MAX))
	    || (*best_known < depth)) {
		return;
	}

	for (i = strlen(molecule); i; --i) {
		prefix_to = i - 1;
		submol = molecule + prefix_to;
		for (j = 0; j < tks->len; ++j) {
			if (strstr(submol, tks->keys[j].str)) {
				key = tks->keys[j];
				subs = table->get(table, key.str, key.len);
				sks = subs->keys(subs, 0);
				qsort(sks->keys, sks->len,
				      sizeof(struct ehht_key_s),
				      cmp_key_len_asc);
				postfix_from = prefix_to + strlen(key.str);
				for (k = 0; k < sks->len; ++k) {
					skey = sks->keys[k];
					if (find_in_substitute
					    (skey, prefix_to, postfix_from,
					     table, tks, molecule, depth,
					     best_known, max_tries, verbose)) {
						subs->free_keys(subs, sks);
						return;
					}
				}
				subs->free_keys(subs, sks);
			}
		}
	}
}

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	char *buf, from[BUF_LEN], to[BUF_LEN];
	int matched, verbose, construct;
	struct ehht_s *table, *subs, *perms;
	size_t result, len;
	char *molecule;
	struct buf_pos_len_s bpl;
	struct ehht_keys_s *tks;
	unsigned best;
	int max_tries;

	input_file_name = (argc > 1) ? argv[1] : "input";
	construct = (argc > 2) ? atoi(argv[2]) : 0;
	max_tries = (argc > 3) ? atoi(argv[3]) : 0;
	verbose = (argc > 4) ? atoi(argv[4]) : 0;
	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	table = ehht_new();

	buf = mallocd(sizeof(char) * BIG_BUF_LEN);
	molecule = NULL;
	while (fgets(buf, BIG_BUF_LEN, input)) {
		trimstr(buf, BIG_BUF_LEN);
		matched = sscanf(buf, "%40s => %40s", from, to);
		if (matched == 2) {
			len = strlen(from);
			subs = table->get(table, from, len);
			if (!subs) {
				subs = ehht_new();
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

	if (construct) {
		perms = NULL;
		subs = table;
		table = reverse_table(table, verbose);
		subs->for_each(subs, free_tables, subs);
		ehht_free(subs);
		tks = table->keys(table, 0);
		qsort(tks->keys, tks->len, sizeof(struct ehht_key_s),
		      cmp_key_len_dsc);
		best = UINT_MAX;
		construct_molecule(table, tks, molecule, 0, &best, &max_tries,
				   verbose);
		printf("%u\n", best);
		if (MAKE_VALGRIND_HAPPY) {
			table->free_keys(table, tks);
		}
	} else {
		perms = ehht_new();
		result = permute(table, molecule, perms, verbose);
		printf("%lu\n", (unsigned long)result);
		if (MAKE_VALGRIND_HAPPY) {
			ehht_free(perms);
		}
	}

	if (MAKE_VALGRIND_HAPPY) {
		free(molecule);
		table->for_each(table, free_tables, table);
		ehht_free(table);
	}
	return 0;
}
