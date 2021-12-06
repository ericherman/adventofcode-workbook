#ifndef LIBC_HEADERS
#define LIBC_HEADERS 1

/* TODO: check __STDC_VERSION__ (e.g. 201710) and conditionally include */

/* Freestanding headers */
#include <float.h>		/* floating-point types and limits */
/* #include <iso646.h> */	/* (C95) Alternative operator spellings */
#include <stdalign.h>		/* (C11) alignas and alignof macros */
#include <stdarg.h>		/* varargs */
#include <stdbool.h>		/* (C99) boolean typedefs/macros */
#include <stddef.h>		/* NULL, size_t, ptrdiff_t, and friends */
#include <stdint.h>		/* (C99) uint32_t and friends */
/* #include <stdnoreturn.h> */	/* (C11) noreturn macro */

#if ( __STDC_HOSTED__ || __STDC_IEC_60559_BFP__ || __STDC_IEC_60559_BFP__ )
#include <fenv.h>		/* (C23 freestanding) floating-point */
#include <math.h>		/* (C23 freestanding) */
#endif

/* hosted headers */
#if ( __STDC_HOSTED__ )

#include <assert.h>
#include <complex.h>		/* (C99) Complex number arithmetic */
#include <ctype.h>		/* character types */
#include <errno.h>		/* error reporting macros */
#include <inttypes.h>		/* (C99) int formats, eg PRIu32 for uint32_t */
#include <limits.h>		/* e.g.: UINT_MAX */
#include <locale.h>		/* localization */
#include <setjmp.h>		/* long jump */
#include <signal.h>		/* signal handling */
#include <stdatomic.h>		/* (C11) atomic ops */
#include <stdio.h>		/* standard I/O */
#include <stdlib.h>		/* (partially freestanding) */
#include <string.h>		/* (partially freestanding) */
#include <tgmath.h>		/* (C99) type-generic math */
#include <threads.h>		/* (C11) */
#include <time.h>		/* clock_t, time_t and utils */
#include <uchar.h>		/* (C11) UTF-16, UTF-32 */
#include <wchar.h>		/* (C95) wide char util */
#include <wctype.h>		/* (C95) wide character types */

#endif /* __STDC_HOSTED__ */

#endif /* LIBC_HEADERS */
