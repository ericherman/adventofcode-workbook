/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

struct eembed_log *echeck_ensure_log(struct eembed_log *err)
{
	if (err) {
		return err;
	}
	return eembed_err_log;
}

static void echeck_append_fail(struct eembed_log *err, const char *msg)
{
	err->append_s(err, "FAIL:");
	if (msg) {
		err->append_s(err, " ");
		err->append_s(err, msg);
		err->append_s(err, ":");
	}
}

static void echeck_append_func_file_line(struct eembed_log *err,
					 const char *func, const char *file,
					 int line)
{
	err->append_s(err, "[");
	if (func) {
		err->append_s(err, func);
		err->append_s(err, " at ");
	}
	err->append_s(err, file);
	err->append_s(err, ":");
	err->append_l(err, line);
	err->append_s(err, "]");
}

unsigned char echeck_char_m(struct eembed_log *err, const char *func,
			    const char *file, int line, char actual,
			    char expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected '");
	if (expected) {
		err->append_c(err, expected);
	} else {
		err->append_s(err, "\\0");
	}
	err->append_s(err, "' but was '");
	if (actual) {
		err->append_c(err, actual);
	} else {
		err->append_s(err, "\\0");
	}
	err->append_s(err, "' ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

unsigned char echeck_unsigned_long_m(struct eembed_log *err, const char *func,
				     const char *file, int line,
				     unsigned long actual,
				     unsigned long expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	err->append_ul(err, expected);
	err->append_s(err, " but was ");
	err->append_ul(err, actual);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

unsigned char echeck_long_m(struct eembed_log *err, const char *func,
			    const char *file, int line, long actual,
			    long expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	err->append_l(err, expected);
	err->append_s(err, " but was ");
	err->append_l(err, actual);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

unsigned char echeck_size_t_m(struct eembed_log *err, const char *func,
			      const char *file, int line, size_t actual,
			      size_t expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	err->append_ul(err, expected);
	err->append_s(err, " but was ");
	err->append_ul(err, actual);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

unsigned char echeck_str_m(struct eembed_log *err, const char *func,
			   const char *file, int line, const char *actual,
			   const char *expected, const char *msg)
{
	if (actual == expected) {
		return 0;
	}
	if (actual != NULL && expected != NULL
	    && eembed_strcmp(expected, actual) == 0) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	if (expected) {
		err->append_s(err, "'");
		err->append_s(err, expected);
		err->append_s(err, "'");
	} else {
		err->append_s(err, "(null)");
	}
	err->append_s(err, " but was ");
	if (actual) {
		err->append_s(err, "'");
		err->append_s(err, actual);
		err->append_s(err, "'");
	} else {
		err->append_s(err, "(null)");
	}
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

unsigned char echeck_str_contains_m(struct eembed_log *err, const char *func,
				    const char *file, int line,
				    const char *haystack, const char *needle,
				    const char *msg)
{
	if (haystack == needle) {
		return 0;
	}

	if (haystack != NULL && needle != NULL
	    && eembed_strstr(haystack, needle) != NULL) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Did not locate ");
	if (needle) {
		err->append_s(err, "'");
		err->append_s(err, needle);
		err->append_s(err, "'");
	} else {
		err->append_s(err, "(null)");
	}
	err->append_s(err, " in ");
	if (haystack) {
		err->append_s(err, "'");
		err->append_s(err, haystack);
		err->append_s(err, "'");
	} else {
		err->append_s(err, "(null)");
	}
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

size_t echeck_str_contains_all_m(struct eembed_log *err, const char *func,
				 const char *file, int line,
				 const char *haystack, const char **needles,
				 size_t needles_len, const char *msg)
{
	size_t i = 0;
	size_t missing = 0;
	const char *needle = NULL;

	for (i = 0; i < needles_len; ++i) {
		needle = needles ? needles[i] : NULL;
		missing +=
		    echeck_str_contains_m(err, func, file, line, haystack,
					  needle, msg);
	}
	return missing;
}

unsigned char echeck_byte_array_m(struct eembed_log *err, const char *func,
				  const char *file, int line,
				  const unsigned char *actual,
				  size_t actual_len,
				  const unsigned char *expected,
				  size_t expected_len, const char *msg)
{

	size_t i = 0;
	size_t min_max = 0;
	int fail = 0;

	err = echeck_ensure_log(err);

	if (actual_len != expected_len) {
		err->append_s(err, "actual/expected length mis-match ");
		err->append_ul(err, (unsigned long)actual_len);
		err->append_s(err, " != ");
		err->append_ul(err, (unsigned long)expected_len);
		err->append_s(err, " ");
		echeck_append_func_file_line(err, func, file, line);
		err->append_eol(err);
		fail = 1;
	}
	min_max = actual_len < expected_len ? actual_len : expected_len;
	for (i = 0; i < min_max; i++) {
		if (actual[i] != expected[i]) {
			err->append_s(err, "buffers differ at ");
			err->append_ul(err, (unsigned long)i);
			err->append_eol(err);
			fail = 1;
		}
	}
	if (!fail) {
		return 0;
	}

	echeck_append_fail(err, msg);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	err->append_s(err, "index:\tactual\texpected");
	err->append_eol(err);
	for (i = 0; i < actual_len || i < expected_len; i++) {
		err->append_ul(err, i);
		err->append_s(err, ":\t");
		if (i < actual_len) {
			err->append_ul(err, actual[i]);
		}
		if (i < expected_len) {
			err->append_s(err, "\t");
			err->append_ul(err, expected[i]);
		}
		err->append_eol(err);
	}
	return 1;
}

unsigned char echeck_double_m(struct eembed_log *err, const char *func,
			      const char *file, int line, double actual,
			      double expected, double epsilon, const char *msg)
{
	double from, to, swap;

	if (actual == expected) {
		return 0;
	}

	epsilon = (epsilon < 0.0) ? -epsilon : epsilon;
	from = expected + epsilon;
	to = expected - epsilon;
	if (from > to) {
		swap = from;
		from = to;
		to = swap;
	}

	if (actual >= from && actual <= to) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	err->append_f(err, expected);
	err->append_s(err, "(+/- ");
	err->append_f(err, epsilon);
	err->append_s(err, ")");
	err->append_s(err, " but was ");
	err->append_f(err, actual);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

unsigned char echeck_ptr_m(struct eembed_log *err, const char *func,
			   const char *file, int line, const void *actual,
			   const void *expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	err->append_vp(err, expected);
	err->append_s(err, " but was ");
	err->append_vp(err, actual);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

unsigned char echeck_ptr_not_null_m(struct eembed_log *err, const char *func,
				    const char *file, int line,
				    const void *actual, const char *msg)
{
	if (actual) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected non-NULL pointer ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

char echeck_status_m(struct eembed_log *err, const char *func, const char *file,
		     int line, int val, const char *msg)
{
	char c;

	if ((-128 <= val) && (val <= 127)) {
		c = (char)val;
		return val;
	}
	if (val < -128) {
		c = -128;
	} else {
		c = 127;
	}
	err = echeck_ensure_log(err);
	if (msg) {
		err->append_s(err, msg);
		err->append_s(err, " ");
	}
	err->append_l(err, val);
	err->append_s(err, " capped at ");
	err->append_l(err, c);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return c;
}

void whine_if_context_data_corruption(struct echeck_err_injecting_context *ctx)
{
	struct eembed_log *log = NULL;
	if (ctx->free_bytes > ctx->alloc_bytes) {
		log = ctx->log;
		log->append_s(log, __FILE__);
		log->append_s(log, ":");
		log->append_ul(log, __LINE__);
		log->append_s(log, " BAD MOJO: ");
		log->append_s(log, " free_bytes > alloc_bytes?! (");
		log->append_ul(log, ctx->free_bytes);
		log->append_s(log, " > ");
		log->append_ul(log, ctx->alloc_bytes);
		log->append_s(log, ")");
		log->append_s(log, " CONTEXT DATA CORRUPTION!");
		log->append_eol(log);
	}
}

void *echeck_err_injecting_malloc(struct eembed_allocator *ea, size_t size)
{
	struct eembed_allocator *real;
	struct echeck_err_injecting_context *ctx = NULL;
	unsigned char *tracking_buffer = NULL;
	void *ptr = NULL;
	size_t wide = 0;
	size_t used = 0;

	ctx = (struct echeck_err_injecting_context *)ea->context;
	if (0x01 & (ctx->attempts_to_fail_bitmask >> ctx->attempts++)) {
		return NULL;
	}
	real = ctx->real;
	wide = sizeof(size_t) + size;
	tracking_buffer = (unsigned char *)real->malloc(real, wide);
	if (!tracking_buffer) {
		++ctx->fails;
		return NULL;
	}

	eembed_memcpy(tracking_buffer, &size, sizeof(size_t));
	++ctx->allocs;
	ctx->alloc_bytes += size;

	whine_if_context_data_corruption(ctx);

	used = ctx->alloc_bytes - ctx->free_bytes;
	if (used > ctx->max_used) {
		ctx->max_used = used;
	}
	ptr = (void *)(tracking_buffer + sizeof(size_t));
	return ptr;
}

void *echeck_err_injecting_calloc(struct eembed_allocator *ea, size_t nmemb,
				  size_t size)
{
	void *ptr = ea->malloc(ea, nmemb * size);
	if (ptr) {
		eembed_memset(ptr, 0x00, nmemb * size);
	}
	return ptr;
}

void *echeck_err_injecting_realloc(struct eembed_allocator *ea, void *ptr,
				   size_t newsize)
{
	unsigned char *tracking_buffer = NULL;
	size_t size = 0;
	void *ptr2 = NULL;

	if (ptr == NULL) {
		return ea->malloc(ea, newsize);
	}

	tracking_buffer = ((unsigned char *)ptr) - sizeof(size_t);
	eembed_memcpy(&size, tracking_buffer, sizeof(size_t));

	if (newsize == size) {
		return ptr;
	}

	ptr2 = ea->calloc(ea, 1, newsize);
	if (!ptr2) {
		return ptr2;
	}

	eembed_memcpy(ptr2, ptr, newsize < size ? newsize : size);

	ea->free(ea, ptr);

	return ptr2;
}

void *echeck_err_injecting_reallocarray(struct eembed_allocator *ea, void *ptr,
					size_t nmemb, size_t size)
{
	return ea->realloc(ea, ptr, (nmemb * size));
}

void echeck_err_injecting_free(struct eembed_allocator *ea, void *ptr)
{
	struct eembed_allocator *real;
	struct echeck_err_injecting_context *ctx = NULL;
	unsigned char *tracking_buffer = NULL;
	size_t size = 0;

	ctx = (struct echeck_err_injecting_context *)ea->context;

	if (ptr == NULL) {
		++ctx->fails;
		return;
	}

	tracking_buffer = ((unsigned char *)ptr) - sizeof(size_t);
	eembed_memcpy(&size, tracking_buffer, sizeof(size_t));

	real = ctx->real;
	real->free(real, tracking_buffer);

	ctx->free_bytes += size;
	++ctx->frees;

	whine_if_context_data_corruption(ctx);
}

int echeck_err_injecting_allocator_init(struct eembed_allocator *with_errs,
					struct eembed_allocator *real,
					struct echeck_err_injecting_context *c,
					struct eembed_log *log)
{
	eembed_assert(with_errs);
	eembed_assert(real);
	eembed_assert(c);
	eembed_assert(log);

	if (!with_errs || !real || !c || !log) {
		return 1;
	}

	eembed_memset(c, 0x00, sizeof(struct echeck_err_injecting_context));
	c->real = real;
	c->log = log;

	with_errs->context = c;

	with_errs->malloc = echeck_err_injecting_malloc;
	with_errs->calloc = echeck_err_injecting_calloc;
	with_errs->realloc = echeck_err_injecting_realloc;
	with_errs->reallocarray = echeck_err_injecting_reallocarray;
	with_errs->free = echeck_err_injecting_free;

	return 0;
}
