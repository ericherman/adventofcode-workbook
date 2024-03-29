/* SPDX-License-Identifier: GPL-3.0-or-later */
/* eherr.h easy hosted error logging */
/* Copyright (C) 2021 Eric Herman <eric@freesa.org> */

#ifndef EHERR_H
#define EHERR_H

#include <stdarg.h>
#include <stdio.h>

void vlogerror(FILE *log, const char *file, int line, const char *func,
	       const char *format, va_list ap);

void logerror(FILE *log, const char *file, int line, const char *func,
	      const char *format, ...);
#define Logerror0(msg) \
	logerror(stderr, __FILE__, __LINE__, __func__, msg)
#define Logerror(format, ...) \
	logerror(stderr, __FILE__, __LINE__, __func__, format, __VA_ARGS__)

void die(FILE *log, const char *file, int line, const char *func,
	 const char *format, ...);
#define Die0(msg) \
	die(stderr, __FILE__, __LINE__, __func__, msg)
#define Die(format, ...) \
	die(stderr, __FILE__, __LINE__, __func__, format, __VA_ARGS__)

void *calloc_or_die(FILE *log, const char *file, int line, const char *func,
		    size_t nmemb, size_t size);
#define Calloc_or_die(nmemb, size) \
	calloc_or_die(stderr, __FILE__, __LINE__, __func__, nmemb, size)

void *recalloc_or_die(FILE *log, const char *file, int line, const char *func,
		void *orig, size_t orig_nmemb, size_t next_nmemb, size_t size);
#define Recalloc_or_die(p, orig_nmemb, next_nmemb, element_size) \
	recalloc_or_die(stderr, __FILE__, __LINE__, __func__, \
			p, orig_nmemb, next_nmemb, element_size)

FILE *fopen_or_die(FILE *log, const char *file, int line, const char *func,
		const char *pathname, const char *mode);
#define Fopen_or_die(pathname, mode) \
	fopen_or_die(stderr, __FILE__, __LINE__, __func__, pathname, mode)

#endif /* EHERR_H */
