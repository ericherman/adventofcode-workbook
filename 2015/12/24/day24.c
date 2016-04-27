#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ehbigint.h>

#define BUF_LEN 255
#define BIG_INT_LEN 16
#define MAXBIHEX "0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"

#define DIE(format, arg)\
  fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
  fprintf(stderr, format, arg);\
  exit(EXIT_FAILURE)

#define DIE3(format, arg1, arg2, arg3)\
  fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
  fprintf(stderr, format, arg1, arg2, arg3);\
  exit(EXIT_FAILURE)

int cmp_uint_desc(const void *a, const void *b)
{
	unsigned l, r;

	l = *((unsigned *)a);
	r = *((unsigned *)b);
	return (int)(r - l);
}

int choose_k_from_n(size_t *idxs, size_t k, size_t n)
{
	size_t i;

	i = k - 1;
	++idxs[i];
	while (idxs[i] >= (n - k + 1 + i)) {
		if (i == 0) {
			break;
		}
		--i;
		++idxs[i];
	}

	if (idxs[0] > (n - k)) {
		return 0;
	}

	for (i = (i + 1); i < k; ++i) {
		idxs[i] = idxs[i - 1] + 1;
	}

	return 1;
}

char *uints_idxs_to_string(char *buf, unsigned *ints, size_t *idxs, size_t len)
{
	size_t i;
	int x;

	x = sprintf(buf, "[");
	for (i = 0; i < len; ++i) {
		x += sprintf(buf + x, "%u, ", ints[idxs[i]]);
	}
	/* trim trailing comma */
	if (x > 2) {
		x -= 2;
	}
	sprintf(buf + x, "]");
	return buf;
}

static unsigned long do_addul(unsigned long a, unsigned long b)
{
	unsigned long c;

	c = a + b;
	if (c < a || c < b) {
		DIE3("%lu + %lu == %lu? Overflow!\n", a, b, c);
	}
	return c;
}

int adds_up(unsigned *weights, size_t *idxs, size_t set_size,
	    unsigned long target, unsigned group_no, int verbose)
{
	unsigned long subtotal;
	size_t i;
	char buf[250];

	subtotal = 0;
	for (i = 0; i < set_size; ++i) {
		subtotal = do_addul(subtotal, weights[idxs[i]]);
	}
	if (subtotal == target) {
		if (verbose) {
			printf("\tgroup %u:%s\n", group_no,
			       uints_idxs_to_string(buf, weights, idxs, i));
		}
		return 1;
	} else {
		if (verbose > 3) {
			printf("\tgroup %d: %s\n"
			       "\t\tsubtotal != target, %lu != %lu\n",
			       group_no,
			       uints_idxs_to_string(buf, weights, idxs, i),
			       subtotal, target);
		}
	}
	return 0;
}

int can_split_remainder(unsigned *weights, size_t *idxs1, size_t wlen,
			size_t set_size, unsigned long target,
			unsigned groups, int verbose)
{
	size_t i, j, k, m, n, p;
	int used, addsup;
	char buf[250];
	size_t *idxs2, *idxs3, *idxs4;

	idxs2 = malloc(sizeof(size_t) * wlen);
	if (!idxs2) {
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < wlen; ++i) {
		idxs2[i] = (size_t)-1;
	}

	for (k = 0, i = 0; i < wlen; ++i) {
		used = 0;
		for (j = 0; j < set_size && !used; ++j) {
			if (idxs1[j] == i) {
				used = 1;
			}
		}
		if (!used) {
			idxs2[k++] = i;
		}
	}

	idxs3 = malloc(sizeof(size_t) * wlen);
	if (!idxs3) {
		exit(EXIT_FAILURE);
	}
	if (groups == 4) {
		idxs4 = malloc(sizeof(size_t) * wlen);
		if (!idxs4) {
			exit(EXIT_FAILURE);
		}
	} else {
		idxs4 = NULL;
	}

	if (verbose > 3) {
		printf("\tremaining to group:%s\n",
		       uints_idxs_to_string(buf, weights, idxs2, k));
	}

	addsup = 0;
	for (i = 1; i < k; ++i) {
		/* must reset the indexes before permuting */
		for (j = 0; j < k; ++j) {
			idxs3[j] = idxs2[j];
		}
		if (adds_up(weights, idxs3, i, target, 2, verbose)) {
			if (groups == 4) {
				p = 0;
				for (m = 0; m < wlen; ++m) {
					used = 0;
					for (n = 0; n < set_size && !used; ++n) {
						if (idxs1[n] == m) {
							used = 1;
						}
					}
					for (n = 0; n < j && !used; ++n) {
						if (idxs3[n] == m) {
							used = 1;
						}
					}
					if (!used) {
						idxs4[p++] = m;
					}
				}
				addsup =
				    adds_up(weights, idxs4, p, target, 3,
					    verbose);
			} else {
				addsup = 1;
			}
			if (addsup) {
				goto cleanup;
			}
		}
		while (choose_k_from_n(idxs3, i, k)) {
			if (adds_up(weights, idxs3, i, target, 2, verbose)) {
				if (groups == 4) {
					p = 0;
					for (m = 0; m < wlen; ++m) {
						used = 0;
						for (n = 0;
						     n < set_size && !used;
						     ++n) {
							if (idxs1[n] == m) {
								used = 1;
							}
						}
						for (n = 0; n < j && !used; ++n) {
							if (idxs3[n] == m) {
								used = 1;
							}
						}
						if (!used) {
							idxs4[p++] = m;
						}
					}
					addsup =
					    adds_up(weights, idxs4, p, target,
						    3, verbose);
				} else {
					addsup = 1;
				}
				if (addsup) {
					goto cleanup;
				}
			}
		}
	}

cleanup:
	if (verbose > 1 && addsup) {
		printf("\tgroup 2:%s\n",
		       uints_idxs_to_string(buf, weights, idxs3, k));
	}
	if (groups == 4) {
		free(idxs4);
	}
	free(idxs3);
	free(idxs2);
	return addsup;
}

void check_permutation(unsigned *weights, size_t *idxs, size_t set_size,
		       size_t wlen, unsigned long target, unsigned groups,
		       size_t *shortest, struct ehbigint *smallest_product,
		       int verbose)
{

	size_t j;
	int err;
	struct ehbigint product, result, weight;
	unsigned char product_bytes[BIG_INT_LEN];
	unsigned char result_bytes[BIG_INT_LEN];
	unsigned char weight_bytes[sizeof(unsigned long)];
	char hex[BUF_LEN], pbuf[BUF_LEN], spbuf[BUF_LEN];


	product.bytes = product_bytes;
	product.bytes_len = BIG_INT_LEN;
	err = ehbi_set_ul(&product, 1);
	if (err) {
		DIE("ehbi_set_ul: %d\n", err);
	}
	result.bytes = result_bytes;
	result.bytes_len = BIG_INT_LEN;

	weight.bytes = weight_bytes;
	weight.bytes_len = sizeof(unsigned long);

	if (!adds_up(weights, idxs, set_size, target, 1, verbose)) {
		return;
	}

	for (j = 0; j < set_size; ++j) {
		err = ehbi_set_ul(&weight, weights[idxs[j]]);
		if(err) {
			DIE("ehbi_set_ul: %d\n", err);
		}
		err = ehbi_mul(&result, &product, &weight);
		if (err) {
			DIE("ehbi_inc: %d\n", err);
		}
		err = ehbi_set(&product, &result);
		if (err) {
			DIE("ehbi_set: %d\n", err);
		}
	}
	if (ehbi_greater_than(&product, smallest_product, &err)) {
		if (err) {
			DIE("ehbi_greater_than error: %d\n", err);
		}
		if (verbose > 1) {
			printf("\tsubtotal: %lu\n", target);
			ehbi_to_hex_string(&product, hex, BUF_LEN);
			ehbi_hex_to_decimal(hex, BUF_LEN, pbuf, BUF_LEN);
			ehbi_to_hex_string(smallest_product, hex, BUF_LEN);
			ehbi_hex_to_decimal(hex, BUF_LEN, spbuf, BUF_LEN);
			printf("\tproduct > *smallest_product, %s > %s\n",
			       pbuf, spbuf);
		}
		return;
	}

	if (verbose) {
		printf("\tsubtotal: %lu\n", target);
		ehbi_to_hex_string(&product, hex, BUF_LEN);
		ehbi_hex_to_decimal(hex, BUF_LEN, pbuf, BUF_LEN);
		printf("\tproduct: %s\n", pbuf);
	}

	if (can_split_remainder
	    (weights, idxs, wlen, set_size, target, groups, verbose)) {
		if ((set_size <= *shortest)
		    && !ehbi_greater_than(&product, smallest_product, &err)) {
			*shortest = set_size;
			ehbi_set(smallest_product, &product);
			if (verbose) {
				printf("*shortest: %lu\n",
				       (unsigned long)*shortest);
				ehbi_to_hex_string(smallest_product, hex,
						   BUF_LEN);
				ehbi_hex_to_decimal(hex, BUF_LEN, spbuf,
						    BUF_LEN);
				printf("*smallest_product: %s\n", spbuf);
			}
		}
		if (err) {
			DIE("ehbi_greater_than error: %d\n", err);
		}
	}
}

int main(int argc, char **argv)
{
	const char *input_file_name;
	FILE *input;
	char hex[BUF_LEN], buf[BUF_LEN];
	int matched, verbose;
	unsigned *weights, groups;
	unsigned long total, target;
	struct ehbigint smallest_product;
	unsigned char smallest_product_bytes[BIG_INT_LEN];
	size_t i, j, bytes, wsize, wlen, from, shortest;
	size_t *idxs;

	verbose = (argc > 1) ? atoi(argv[1]) : 0;
	groups = (argc > 2) ? ((atoi(argv[2]) == 4) ? 4 : 3) : 3;
	input_file_name = (argc > 3) ? argv[3] : "input";
	input = fopen(input_file_name, "r");
	if (!input) {
		DIE("could not open %s\n", input_file_name);
	}

	wsize = 0;
	wlen = 0;
	weights = NULL;
	while (fgets(buf, 255, input)) {
		if (wlen >= wsize) {
			bytes = sizeof(int) * (wsize + BUF_LEN);
			weights = realloc(weights, bytes);
			if (!weights) {
				DIE("could not allocate %d bytes\n", bytes);
			}
			wsize += BUF_LEN;
		}
		matched = sscanf(buf, "%u", &weights[wlen]);
		if (matched) {
			++wlen;
		}
	}
	fclose(input);

	qsort(weights, wlen, sizeof(unsigned), cmp_uint_desc);

	bytes = sizeof(size_t) * (wlen + 1);
	idxs = malloc(bytes);
	if (!idxs) {
		exit(EXIT_FAILURE);
	}

	total = 0;
	for (i = 0; i < wlen; ++i) {
		total += weights[i];
		if (verbose) {
			printf("%lu: %u\n", (unsigned long)i, weights[i]);
		}
	}
	if ((total % groups) != 0) {
		printf("%lu does not divide evenly by %u\n", total,
		       (unsigned)groups);
		exit(EXIT_FAILURE);
	}
	target = total / groups;
	if (verbose) {
		printf("target: %lu\n", target);
	}
	shortest = (wlen - 1);
	smallest_product.bytes = smallest_product_bytes;
	smallest_product.bytes_len = BIG_INT_LEN;
	ehbi_from_hex_string(&smallest_product, MAXBIHEX, strlen(MAXBIHEX));

	from = target / weights[0];
	for (i = from; i <= shortest; ++i) {
		for (j = 0; j < wlen; ++j) {
			idxs[j] = j;
		}
		check_permutation(weights, idxs, i, wlen, target, groups,
				  &shortest, &smallest_product, verbose);
		while (choose_k_from_n(idxs, i, wlen)) {
			check_permutation(weights, idxs, i, wlen, target,
					  groups, &shortest, &smallest_product,
					  verbose);
		}
	}


	ehbi_to_hex_string(&smallest_product, hex, BUF_LEN -1);
	ehbi_hex_to_decimal(hex, BUF_LEN-1, buf, BUF_LEN-1);
	printf("%s\n", buf);

	free(idxs);
	free(weights);

	return 0;
}
