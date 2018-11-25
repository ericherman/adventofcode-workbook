/* day17.c 2015 AdventOfCode solution
   Copyright (C) 2015-2016 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   See COPYING or <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>		/* malloc exit qsort */
#include <ehht.h>		/* github.com/ericherman/libehht */
#include <string.h>		/* memcpy */
#include <alloca.h>

#define BUF_LEN 255
#define MAKE_VALGRIND_HAPPY 1

struct container_s {
	size_t i;
	unsigned capacity;
};

static struct container_s *new_container(size_t i, unsigned capacity)
{
	struct container_s *p;

	p = malloc(sizeof(struct container_s));
	if (!p) {
		fprintf(stderr, "failed to alloc container_s (%lu -> %u)",
			(unsigned long)i, capacity);
		exit(EXIT_FAILURE);
	}
	p->i = i;
	p->capacity = capacity;

	return p;
}

static size_t to_key(size_t n, char *buf, size_t buf_len)
{
	int s;
	s = snprintf(buf, buf_len, "%lu", (unsigned long)n);
	if (s < 0) {
		fprintf(stderr, "could not write key to buf[%lu]\n",
			(unsigned long)buf_len);
		exit(EXIT_FAILURE);
	}
	return (size_t)s;
}

static unsigned capacity_of(struct container_s **containers, size_t len,
			    size_t bits)
{
	size_t i, total;

	total = 0;
	for (i = 0; i < len; ++i) {
		if (bits & (1 << i)) {
			total += containers[i]->capacity;
		}
	}
	return total;
}

static size_t print_containers_bits(char *buf, size_t buf_len,
				    struct container_s **containers, size_t len,
				    size_t bits)
{
	int s;
	size_t i, pos;
	struct container_s *c;
	unsigned total;

	total = 0;
	for (pos = 0, i = 0; i < len; ++i) {
		if (bits & (1 << i)) {
			c = containers[i];
			total += c->capacity;
			s = snprintf(&buf[pos], buf_len - pos, "%s(%lu:%u)",
				     pos ? "+" : "",
				     (unsigned long)c->i, c->capacity);
			if (s < 0) {
				fprintf(stderr, "could not write to buf[%lu]\n",
					(unsigned long)buf_len);
				exit(EXIT_FAILURE);
			}
			pos += (unsigned)s;
		}
	}
	s = snprintf(&buf[pos], buf_len - pos, "=%u", total);
	if (s < 0) {
		fprintf(stderr, "could not write to buf[%lu]\n",
			(unsigned long)buf_len);
		exit(EXIT_FAILURE);
	}
	pos += s;

	return pos;
}

static unsigned short int bits_on(size_t n)
{
	size_t i;
	unsigned short int bits_on;

	for (bits_on = 0, i = 0; i < __WORDSIZE; ++i) {
		if (n & (1 << i)) {
			++bits_on;
		}
	}

	return bits_on;
}

static size_t find_combos(struct container_s **containers,
			  size_t num_containers, unsigned target, int min_only,
			  struct ehht_s *table, int verbose)
{
	int len;
	size_t i, max;
	unsigned short int num_on, min_num_on;
	char buf1[BUF_LEN];
	char buf2[BUF_LEN];

	min_num_on = num_containers;
	max = 1 << (num_containers);

	for (i = 0; i < max; ++i) {
		num_on = min_only ? bits_on(i) : 0;
		if (num_on <= min_num_on
		    && capacity_of(containers, num_containers, i) == target) {
			if (num_on < min_num_on) {
				table->clear(table);
				min_num_on = num_on;
			}
			len = snprintf(buf1, BUF_LEN, "%lu", (unsigned long)i);
			if (len < 0) {
				fprintf(stderr, "could not write to buf[%lu]\n",
					(unsigned long)BUF_LEN);
				exit(EXIT_FAILURE);
			}
			table->put(table, buf1, len, NULL);
			if (verbose) {
				print_containers_bits(buf2, BUF_LEN, containers,
						      num_containers, i);
				fprintf(stderr, "combo %s: %s\n", buf1, buf2);
			}
		}
	}

	return table->size(table);
}

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	char buf[BUF_LEN];
	int matched, verbose, min_only;
	unsigned capacity, target;
	struct ehht_s *table;
	struct container_s *container, **containers;
	size_t i, len, num_containers;

	input_file_name = (argc > 1) ? argv[1] : "input";
	min_only = (argc > 2) ? atoi(argv[2]) : 0;

	target = (argc > 3) ? (unsigned)atoi(argv[3]) : 150;
	verbose = (argc > 4) ? atoi(argv[4]) : 0;

	input = fopen(input_file_name, "r");
	if (!input) {
		fprintf(stderr, "could not open %s\n", input_file_name);
		return 1;
	}

	table = ehht_new(0, NULL, NULL, NULL, NULL);

	container = NULL;
	num_containers = 0;
	while (fgets(buf, BUF_LEN, input)) {
		matched = sscanf(buf, "%u", &capacity);
		if (matched != 1) {
			fprintf(stderr, "failed to match '%s'\n", buf);
		} else {
			container = new_container(num_containers++, capacity);
			len = to_key(container->i, buf, BUF_LEN);
			table->put(table, buf, len, container);
		}
	}
	fclose(input);

	containers = malloc(sizeof(struct container_s *) * num_containers);
	for (i = 0; i < num_containers; ++i) {
		len = to_key(i, buf, BUF_LEN);
		container = table->remove(table, buf, len);
		containers[i] = container;
	}

	find_combos(containers, num_containers, target, min_only, table,
		    verbose);

	printf("%u\n", (unsigned)table->size(table));

	if (MAKE_VALGRIND_HAPPY) {
		for (i = 0; i < num_containers; ++i) {
			free(containers[i]);
		}
		free(containers);
		ehht_free(table);
	}
	return 0;
}
