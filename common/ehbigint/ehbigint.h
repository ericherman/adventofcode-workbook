/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* ehbigint.h: slow Big Int library hopefully somewhat suitable for 8bit CPUs */
/* Copyright (C) 2016, 2018, 2019 Eric Herman <eric@freesa.org> */

#ifndef EHBIGINT_H
#define EHBIGINT_H

#ifdef __cplusplus
#define Ehbigint_begin_C_functions extern "C" {
#define Ehbigint_end_C_functions }
#else
#define Ehbigint_begin_C_functions
#define Ehbigint_end_C_functions
#endif

Ehbigint_begin_C_functions
#undef Ehbigint_begin_C_functions
#include <stddef.h>		/* size_t */
    struct ehbigint;

struct ehbigint {
	unsigned char *bytes;
	size_t bytes_len;
	size_t bytes_used;
	unsigned char sign;
};

/*
   assigns the byte[] to the struct, sets to zero
   parameters must not be NULL
*/
struct ehbigint *ehbi_init(struct ehbigint *bi, unsigned char *bytes,
			   size_t len);

/*
   assignes the byte[] to the struct, sets to val
   parameters must not be NULL
*/
struct ehbigint *ehbi_init_l(struct ehbigint *bi, unsigned char *bytes,
			     size_t len, long val, int *err);

/*
   populates an ehbigint with a value of zero
   parameter must not be NULL
*/
struct ehbigint *ehbi_zero(struct ehbigint *bi);

/*
   populates an ehbigint with a binary string value e.g. "0b0101010111110000"
   returns NULL on error, and populates err with error_code
*/

struct ehbigint *ehbi_set_binary_string(struct ehbigint *bi, const char *str,
					size_t len, int *err);

/*
   populates an ehbigint with a hex string value e.g. "0x4badc0de"
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_set_hex_string(struct ehbigint *bi, const char *str,
				     size_t str_len, int *err);

/*
   populates an ehbigint with a decimal string value e.g. "1234567890123456789"
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_set_decimal_string(struct ehbigint *bi, const char *dec,
					 size_t len, int *err);

/*
   populates an ehbigint with the value
*/
struct ehbigint *ehbi_set(struct ehbigint *bi, const struct ehbigint *val,
			  int *err);

/*
   populates an ehbigint with the value
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_set_l(struct ehbigint *bi, long val, int *err);

/*
   populates the first ehbigint with the sum of the second and third
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_add(struct ehbigint *res, const struct ehbigint *bi1,
			  const struct ehbigint *bi2, int *err);

/*
   populates the first ehbigint with the sum of the second and third
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_add_l(struct ehbigint *res, const struct ehbigint *bi1,
			    long v2, int *err);

/*
   increments the first ehbigint by the value in the second parameter
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_inc(struct ehbigint *bi, const struct ehbigint *val,
			  int *err);

/*
   increments the ehbigint by the value in the second parameter
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_inc_l(struct ehbigint *bi, long val, int *err);

/*
   decrements the first ehbigint by the value in the second parameter
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_dec(struct ehbigint *bi, const struct ehbigint *val,
			  int *err);

/*
   decrements the first ehbigint by the value in the second parameter
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_dec_l(struct ehbigint *bi, long val, int *err);

/*
   populates the first ehbigint with the value of the second perameter minus
   the third
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_subtract(struct ehbigint *res, const struct ehbigint *bi1,
			       const struct ehbigint *bi2, int *err);

/*
   populates the first ehbigint with the value of the second perameter minus
   the third
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_subtract_l(struct ehbigint *res,
				 const struct ehbigint *bi1, long v2, int *err);

/*
   populates the first ehbigint with the sum of the second and third
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_mul(struct ehbigint *res, const struct ehbigint *bi1,
			  const struct ehbigint *bi2, int *err);

/*
   populates the first ehbigint with the sum of the second and third
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_mul_l(struct ehbigint *res, const struct ehbigint *bi1,
			    long v2, int *err);

/*
   shifts the value of the ehbigint up by num_bits number of bits
   if not enough space was available, overflow is populated with the number
   of bits lost, but *does* *not* return NULL
   returns the ehbigint pointer, even on overflow
*/
struct ehbigint *ehbi_shift_left(struct ehbigint *bi, unsigned long num_bits,
				 unsigned long *overflow);

/*
   shifts the value of the ehbigint down by num_bits number of bits
*/
struct ehbigint *ehbi_shift_right(struct ehbigint *bi, unsigned long num_bits);

/*
   populates the first ehbigint quotient and remainder with the results
   of the numerator divided by the denominator
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_div(struct ehbigint *quotient,
			  struct ehbigint *remainder,
			  const struct ehbigint *numerator,
			  const struct ehbigint *denominator, int *err);

/*
   populates the first ehbigint quotient and remainder with the results
   of the numerator divided by the denominator
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_div_l(struct ehbigint *quotient,
			    struct ehbigint *remainder,
			    const struct ehbigint *numerator,
			    long denominator, int *err);

/*
   populates the first ehbigint with the largest integer not greater
   than the square root of the thrid ehbigint; the second ehbigint
   is populated with the remainder
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_sqrt(struct ehbigint *intsqrt, struct ehbigint *remainder,
			   const struct ehbigint *val, int *err);

/*
   populates the first ehbigint result with the value of the base
   raised to the power of the exponent
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_exp(struct ehbigint *result, const struct ehbigint *base,
			  const struct ehbigint *exponent, int *err);

/*
   populates the first ehbigint result with the value of the base
   raised to the power of the exponent
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_exp_l(struct ehbigint *result,
			    const struct ehbigint *base, long exponent,
			    int *err);

/*
   populates the first ehbigint result with the value of the base
   raised to the power of the exponent mod the modulus
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_exp_mod(struct ehbigint *result,
			      const struct ehbigint *base,
			      const struct ehbigint *exponent,
			      const struct ehbigint *modulus, int *err);

/*
   populates the first ehbigint result with the value of the base
   raised to the power of the exponent mod the modulus
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_exp_mod_l(struct ehbigint *result,
				const struct ehbigint *base,
				const struct ehbigint *exponent, long modulus,
				int *err);

/*
   populates the first ehbigint result with the value of the base
   raised to the power of the exponent mod the modulus
   returns NULL on error, and populates err with error_code
*/
struct ehbigint *ehbi_exp_mod_ll(struct ehbigint *result,
				 const struct ehbigint *base, long exponent,
				 long modulus, int *err);

/*
   populates the first ehbigint result with the number of combinations
   of n objects taken k at a time, disregarding order.
   returns NULL on error, and populates err with error_code

   Knuth TAoCP vol 1
   1.2.6 Binomial Coefficients
     / n \    n(n-1)...(n-k+1)
    (     ) = ----------------
     \ k /       k(k-1)...1
*/
struct ehbigint *ehbi_n_choose_k(struct ehbigint *result,
				 const struct ehbigint *n,
				 const struct ehbigint *k, int *err);

/*
   populates the first ehbigint result with the number of combinations
   of n objects taken k at a time, disregarding order.
   returns NULL on error, and populates err with error_code

   Knuth TAoCP vol 1
   1.2.6 Binomial Coefficients
     / n \    n(n-1)...(n-k+1)
    (     ) = ----------------
     \ k /       k(k-1)...1
*/
struct ehbigint *ehbi_n_choose_k_l(struct ehbigint *result,
				   const struct ehbigint *n, long k, int *err);

/*
   populates the first ehbigint result with the number of combinations
   of n objects taken k at a time, disregarding order.
   returns NULL on error, and populates err with error_code

   Knuth TAoCP vol 1
   1.2.6 Binomial Coefficients
     / n \    n(n-1)...(n-k+1)
    (     ) = ----------------
     \ k /       k(k-1)...1
*/
struct ehbigint *ehbi_n_choose_k_ll(struct ehbigint *result, long n, long k,
				    int *err);

#ifndef EHBI_SKIP_IS_PROBABLY_PRIME

/* chance of incorrectly naming a non-prime as prime is 4^(-accuracy) */
#ifndef EHBI_DEFAULT_TRIALS_FOR_IS_PROBABLY_PRIME
/* 4^(-25) == 8.8817842e-16 */
#define EHBI_DEFAULT_TRIALS_FOR_IS_PROBABLY_PRIME 25U
#endif

#ifndef EHBI_MIN_TRIALS_FOR_IS_PROBABLY_PRIME
/* 4^(-10) == 9.53674316e-7 */
#define EHBI_MIN_TRIALS_FOR_IS_PROBABLY_PRIME 10U
#endif

#ifndef EHBI_NUM_SMALL_PRIME_WITNESSES
#define EHBI_NUM_SMALL_PRIME_WITNESSES 13U
#endif

/* used in ehbi_is_probably_prime */
#ifndef EHBI_MAX_TRIES_TO_GRAB_RANDOM_BYTES
#define EHBI_MAX_TRIES_TO_GRAB_RANDOM_BYTES 30U
#endif

/*
  returns 1 if the values is prime or probably prime
  returns 0 otherwise
  uses the second parameter (accuracy) to determine strength of the test
  populates the contents of err with 0 on success or error_code on error
*/
int ehbi_is_probably_prime(const struct ehbigint *bi,
			   unsigned int accuracy, int *err);

#endif /* EHBI_SKIP_IS_PROBABLY_PRIME */

/* sign inversion
   returns a pointer int the incoming parameter
   parameter must not be NULL
*/
struct ehbigint *ehbi_negate(struct ehbigint *bi);

/*
   returns 1 if the values represented by the ehbigint arguments are equal
   returns 0 otherwise
*/
int ehbi_equals(const struct ehbigint *bi1, const struct ehbigint *bi2);

/*
   returns 1 if the values represented by the first two arguments are equal
   returns 0 otherwise
*/
int ehbi_equals_l(const struct ehbigint *bi1, long i2);

/*
   returns 1 if the first parameter is less than the second
   returns 0 otherwise
*/
int ehbi_less_than(const struct ehbigint *bi1, const struct ehbigint *bi2);

/*
   returns 1 if the first parameter is less than the second
   returns 0 otherwise
*/
int ehbi_less_than_l(const struct ehbigint *bi1, long i2);

/*
   returns 1 if the first parameter is greater than the second
   returns 0 otherwise
*/
int ehbi_greater_than(const struct ehbigint *bi1, const struct ehbigint *bi2);

/*
   returns 1 if the first parameter is greater than the second
   returns 0 otherwise
*/
int ehbi_greater_than_l(const struct ehbigint *bi1, long i2);

/*
   returns 0 if the values represented by the ehbigint arguments are equal
   returns <0 if the first ehbigint is less than the second
   returns >0 if the first ehbigint is greater than the second
*/
int ehbi_compare(const struct ehbigint *bi1, const struct ehbigint *bi2);

/*
   returns 0 if the values represented by the arguments are equal
   returns <0 if the first ehbigint is less than the second
   returns >0 if the first ehbigint is greater than the second
*/
int ehbi_compare_l(const struct ehbigint *bi1, long i2);

/*
   returns 1 if negative
   returns 0 otherwise
*/
int ehbi_is_zero(const struct ehbigint *bi);

/*
   returns 1 if negative
   returns 0 otherwise
*/
int ehbi_is_negative(const struct ehbigint *bi);

/*
   returns 1 if odd
   returns 0 if even
*/
int ehbi_is_odd(const struct ehbigint *bi);

/*
   populates the passed in buffer with a binary string representation
   of the ehbigint
   returns pointer to buf success or NULL on error and sets the value of
   err with error_code.
*/
char *ehbi_to_binary_string(const struct ehbigint *bi, char *buf,
			    size_t buf_len, int *err);

/*
   populates the passed in buffer with a hex string representation
   of the ehbigint
   returns pointer to buf success or NULL on error and sets the value of
   err with error_code.
*/
char *ehbi_to_hex_string(const struct ehbigint *bi, char *buf, size_t buf_len,
			 int *err);

/*
   populates the passed in buffer with a decimal string representation
   of the ehbigint
   returns pointer to buf success or NULL on error and sets the value of
   err with error_code.
*/
char *ehbi_to_decimal_string(const struct ehbigint *bi, char *buf,
			     size_t buf_len, int *err);

/****************************************************************************/
/* Constructors */
/****************************************************************************/
struct ehbigint *ehbi_alloc(size_t bytes, int *err);
struct ehbigint *ehbi_alloc_l(size_t bytes, long val, int *err);

void ehbi_free(struct ehbigint *bi);

/*****************************************************************************/
/* Log */
/*****************************************************************************/
/* Get the struct eembed_log pointer to where messages currently target.
   Defaults to eembed_err_log. */
struct eembed_log *ehbi_log_get(void);

/* Set the struct eembed_log pointer to where messages shall target. */
/* returns the previous value */
struct eembed_log *ehbi_log_set(struct eembed_log *log);

/* if _POSIX_C_SOURCE backtrace_symbols_fd is used */
void ehbi_log_backtrace(struct eembed_log *log);

/*****************************************************************************/
/* error codes */
/*****************************************************************************/
enum {
	EHBI_SUCCESS = 0,
	EHBI_NULL_CHAR_PTR,
	EHBI_BAD_INPUT,
	EHBI_NOT_HEX,
	EHBI_BAD_HIGH_NIBBLE,
	EHBI_BAD_LOW_NIBBLE,
	EHBI_NULL_STRUCT,
	EHBI_NULL_STRING,
	EHBI_NULL_STRING_BUF,
	EHBI_NULL_ARGS,
	EHBI_ZERO_LEN_STRING,
	EHBI_STRING_BUF_TOO_SMALL,
	EHBI_STRING_BUF_TOO_SMALL_PARTIAL,
	EHBI_STRING_BUF_TOO_SMALL_NO_NULL,
	EHBI_NULL_BYTES,
	EHBI_BYTES_TOO_SMALL,
	EHBI_BYTES_TOO_SMALL_FOR_CARRY,
	EHBI_BYTES_TOO_SMALL_FOR_BORROW,
	EHBI_BAD_DATA,
	EHBI_CORRUPT_DATA,
	EHBI_STACK_TOO_SMALL,
	EHBI_DIVIDE_BY_ZERO,
	EHBI_PRNG_ERROR,
	EHBI_SQRT_NEGATIVE,
	EHBI_LAST
};

/*****************************************************************************/
Ehbigint_end_C_functions
#undef Ehbigint_end_C_functions
#endif /* EHBIGINT_H */
