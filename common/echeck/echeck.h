/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* echeck.h */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Eric Herman <eric@freesa.org> */

#ifndef ECHECK_H
#define ECHECK_H

#ifdef __cplusplus
#define Echeck_begin_C_functions extern "C" {
#define Echeck_end_C_functions }
#else
#define Echeck_begin_C_functions
#define Echeck_end_C_functions
#endif

#ifndef ECHECK_FUNC
#if __STDC_VERSION__ >= 199901L
#define ECHECK_FUNC __func__
#else
#define ECHECK_FUNC ((char *)NULL)
#endif
#endif

Echeck_begin_C_functions
#undef Echeck_begin_C_functions
#include <stddef.h>		/* for size_t */
#include <float.h>		/* for DBL_EPSILON */
#include "eembed.h"		/* struct eembed_log, struct eembed_allocator */
/*check char*/
unsigned char echeck_char_m(struct eembed_log *err, const char *func,
			    const char *file, int line, char actual,
			    char expected, const char *msg);

#define lcheck_char_m(log, actual, expected, msg)\
	echeck_char_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_char_m(actual, expected, msg)\
	echeck_char_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_char(log, actual, expected)\
	echeck_char_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_char(actual, expected)\
	echeck_char_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check long */
unsigned char echeck_long_m(struct eembed_log *err, const char *func,
			    const char *file, int line, long actual,
			    long expected, const char *msg);

#define lcheck_long_m(log, actual, expected, msg)\
	echeck_long_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_long_m(actual, expected, msg)\
	echeck_long_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_long(log, actual, expected)\
	echeck_long_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_long(actual, expected)\
	echeck_long_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check int */
#define echeck_int_m(err, func, file, line, actual, expected, msg)\
	echeck_long_m(err, func, file, line, actual, expected, msg)

#define lcheck_int_m(log, actual, expected, msg)\
	echeck_int_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_int_m(actual, expected, msg)\
	echeck_int_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_int(log, actual, expected)\
	echeck_int_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_int(actual, expected)\
	echeck_int_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check str */
unsigned char echeck_str_m(struct eembed_log *err, const char *func,
			   const char *file, int line, const char *actual,
			   const char *expected, const char *msg);

#define lcheck_str_m(log, actual, expected, msg)\
	echeck_str_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_str_m(actual, expected, msg)\
	echeck_str_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_str(log, actual, expected)\
	echeck_str_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_str(actual, expected)\
	echeck_str_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check str contains*/
unsigned char echeck_str_contains_m(struct eembed_log *err, const char *func,
				    const char *file, int line,
				    const char *haystack, const char *needle,
				    const char *msg);

#define lcheck_str_contains_m(log, haystack, needle, msg)\
	echeck_str_contains_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 haystack, needle, msg)

#define check_str_contains_m(haystack, needle, msg)\
	echeck_str_contains_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 haystack, needle, msg)

#define lcheck_str_contains(log, haystack, needle)\
	echeck_str_contains_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 haystack, needle, #haystack)

#define check_str_contains(haystack, needle)\
	echeck_str_contains_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 haystack, needle, #haystack)

/* check str contains all*/
size_t echeck_str_contains_all_m(struct eembed_log *err, const char *func,
				 const char *file, int line,
				 const char *haystack, const char **needles,
				 size_t needles_len, const char *msg);

#define lcheck_str_contains_all_m(log, haystack, needles, needles_len, msg)\
	echeck_str_contains_all_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 haystack, needles, needles_len, msg)

#define check_str_contains_all_m(haystack, needles, needles_len, msg)\
	echeck_str_contains_all_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 haystack, needles, needles_len, msg)

#define lcheck_str_contains_all(log, haystack, needles, needles_len)\
	echeck_str_contains_all_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 haystack, needles, needles_len, #haystack)

#define check_str_contains_all(haystack, needles, needles_len)\
	echeck_str_contains_all_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 haystack, needles, needles_len, #haystack)

/* check ptr */
unsigned char echeck_ptr_m(struct eembed_log *err, const char *func,
			   const char *file, int line, const void *actual,
			   const void *expected, const char *msg);

#define lcheck_ptr_m(log, actual, expected, msg)\
	echeck_ptr_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_ptr_m(actual, expected, msg)\
	echeck_ptr_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_ptr(log, actual, expected)\
	echeck_ptr_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_ptr(actual, expected)\
	echeck_ptr_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check ptr not null*/
unsigned char echeck_ptr_not_null_m(struct eembed_log *err, const char *func,
				    const char *file, int line,
				    const void *actual, const char *msg);

#define lcheck_ptr_not_null_m(log, actual, msg)\
	echeck_ptr_not_null_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, msg)

#define check_ptr_not_null_m(actual, msg)\
	echeck_ptr_not_null_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, msg)

#define lcheck_ptr_not_null(log, actual)\
	echeck_ptr_not_null_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, #actual)

#define check_ptr_not_null(actual)\
	echeck_ptr_not_null_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, #actual)

/* check unsigned long */
unsigned char echeck_unsigned_long_m(struct eembed_log *err, const char *func,
				     const char *file, int line,
				     unsigned long actual,
				     unsigned long expected, const char *msg);

#define lcheck_unsigned_long_m(log, actual, expected, msg)\
	echeck_unsigned_long_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_unsigned_long_m(actual, expected, msg)\
	echeck_unsigned_long_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_unsigned_long(log, actual, expected)\
	echeck_unsigned_long_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_unsigned_long(actual, expected)\
	echeck_unsigned_long_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check unsigned int */
#define echeck_unsigned_int_m(err, func, file, line, actual, expected, msg)\
	echeck_unsigned_long_m(err, func, file, line, actual, expected, msg)

#define lcheck_unsigned_int_m(log, actual, expected, msg)\
	echeck_unsigned_int_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_unsigned_int_m(actual, expected, msg)\
	echeck_unsigned_int_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_unsigned_int(log, actual, expected)\
	echeck_unsigned_int_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_unsigned_int(actual, expected)\
	echeck_unsigned_int_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check size_t */
unsigned char echeck_size_t_m(struct eembed_log *err, const char *func,
			      const char *file, int line, size_t actual,
			      size_t expected, const char *msg);

#define lcheck_size_t_m(log, actual, expected, msg)\
	echeck_size_t_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_size_t_m(actual, expected, msg)\
	echeck_size_t_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_size_t(log, actual, expected)\
	echeck_size_t_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_size_t(actual, expected)\
	echeck_size_t_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check byte_array */
unsigned char echeck_byte_array_m(struct eembed_log *err, const char *func,
				  const char *file, int line,
				  const unsigned char *actual,
				  size_t actual_len,
				  const unsigned char *expected,
				  size_t expected_len, const char *msg);

#define lcheck_byte_array_m(log, actual, actual_len, expected, expected_len,\
	msg)\
	echeck_byte_array_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, actual_len, expected, expected_len, msg)

#define check_byte_array_m(actual, actual_len, expected, expected_len, msg)\
	echeck_byte_array_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, actual_len, expected, expected_len, msg)

#define lcheck_byte_array(log, actual, actual_len, expected, expected_len)\
	echeck_byte_array_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, actual_len, expected, expected_len, #actual)

#define check_byte_array(actual, actual_len, expected, expected_len)\
	echeck_byte_array_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, actual_len, expected, expected_len, #actual)

/* check double */
unsigned char echeck_double_m(struct eembed_log *err, const char *func,
			      const char *file, int line, double actual,
			      double expected, double epsilon, const char *msg);

#define lcheck_double_m(log, actual, expected, epsilon, msg)\
	echeck_double_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
			actual, expected, epsilon, msg)

#define check_double_m(actual, expected, epsilon, msg)\
	echeck_double_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
			actual, expected, epsilon, msg)

#define lcheck_double(log, actual, expected, epsilon)\
	echeck_double_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
			actual, expected, epsilon, #actual)

#define check_double(actual, expected, epsilon)\
	echeck_double_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
			actual, expected, epsilon, #actual)

/* check double scaled epsilon */
/*
 * WARNING: The "check_double_scaled_epsilon" family of functions
 * uses the "expected" value to scale DBL_EPSILON. This should not
 * be used if "expected" is an expression with side-effects.
 */
#define echeck_double_scaled_epsilon_m(err, func, file, line, actual,\
				       expected, msg)\
	echeck_double_m(err, func, file, line, actual, expected,\
			((expected) * DBL_EPSILON), msg)

#define lcheck_double_scaled_epsilon_m(log, actual, expected, msg)\
	lcheck_double_m(log, actual, expected,\
			((expected) * DBL_EPSILON), msg)

#define check_double_scaled_epsilon_m(actual, expected, msg)\
	check_double_m(actual, expected, ((expected) * DBL_EPSILON), msg)

#define lcheck_double_scaled_epsilon(log, actual, expected)\
	lcheck_double(log, actual, expected, ((expected) * DBL_EPSILON))

#define check_double_scaled_epsilon(actual, expected)\
	check_double(actual, expected, ((expected) * DBL_EPSILON))

/*check status*/
/* safe casting of non-zero int to avoid EXIT_SUCCESS */
char echeck_status_m(struct eembed_log *err, const char *func, const char *file,
		     int line, int val, const char *msg);

#define lcheck_status_m(log, val, msg)\
	echeck_status_m(log, ECHECK_FUNC, __FILE__, __LINE__, val, msg)

#define check_status_m(val, msg)\
	echeck_status_m(NULL, ECHECK_FUNC, __FILE__, __LINE__, val, msg)

#define lcheck_status(log, val)\
	echeck_status_m(log, ECHECK_FUNC, __FILE__, __LINE__, val, NULL)

#define check_status(val)\
	echeck_status_m(NULL, ECHECK_FUNC, __FILE__, __LINE__, val, NULL)

struct echeck_err_injecting_context;

int echeck_err_injecting_allocator_init(struct eembed_allocator *with_errs,
					struct eembed_allocator *real,
					struct echeck_err_injecting_context *c,
					struct eembed_log *log);

struct echeck_err_injecting_context {
	unsigned long allocs;
	unsigned long alloc_bytes;
	unsigned long frees;
	unsigned long free_bytes;
	unsigned long fails;
	unsigned long max_used;
	unsigned long attempts;
	unsigned long attempts_to_fail_bitmask;

	struct eembed_allocator *real;
	struct eembed_log *log;
};

#define echeck_test_main_log_failures(failures, funcname, filename) \
	do { \
		if (failures) { \
			struct eembed_log *elog = eembed_err_log; \
			elog->append_ul(elog, failures); \
			elog->append_s(elog, " failures in function "); \
			elog->append_s(elog, funcname); \
			elog->append_s(elog, " of "); \
			elog->append_s(elog, filename); \
			elog->append_eol(elog); \
		} \
	} while (0)

#if FAUX_FREESTANDING
#define eembed_test_init() \
	eembed_system_print = eembed_faux_freestanding_system_print
#else
#define eembed_test_init() EEMBED_NOP()
#endif

#if (EEMBED_HOSTED || FAUX_FREESTANDING)

#ifndef ECHECK_TEST_MAIN
int printf(const char *format, ...);
#define ECHECK_TEST_MAIN(pfunc) \
void eembed_faux_freestanding_system_print(const char *str) \
{ \
	printf("%s", str); \
}\
int main(void) \
{ \
	unsigned failures = 0; \
	eembed_test_init(); \
	failures += pfunc(); \
	echeck_test_main_log_failures(failures, #pfunc, __FILE__); \
	return check_status(failures); \
}
#endif

#ifndef ECHECK_TEST_MAIN_V
int printf(const char *format, ...);
int atoi(const char *nptr);
#define ECHECK_TEST_MAIN_V(pfunc) \
void eembed_faux_freestanding_system_print(const char *str) \
{ \
	printf("%s", str); \
}\
int main(int argc, char **argv) \
{ \
	int verbose = 0; \
	unsigned failures = 0; \
	eembed_test_init(); \
	verbose = (argc > 1) ? atoi(argv[1]) : 0; \
	failures = pfunc(verbose); \
	echeck_test_main_log_failures(failures, #pfunc, __FILE__); \
	return check_status(failures); \
}
#endif /* ECHECK_TEST_MAIN_V */

#else /* (EEMBED_HOSTED || FAUX_FREESTANDING) */

#ifndef ECHECK_TEST_MAIN
#define ECHECK_TEST_MAIN(pfunc)	/* skip */
#endif

#ifndef ECHECK_TEST_MAIN_V
#define ECHECK_TEST_MAIN_V(pfunc)	/* skip */
#endif

#endif /* (EEMBED_HOSTED || FAUX_FREESTANDING) */

Echeck_end_C_functions
#undef Echeck_end_C_functions
#endif /* ECHECK_H */
