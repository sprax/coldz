/*
 * $Id: dmalloc.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
 *
 *. This file is part of BIT package.
 *
 *
 *  Copyright(c) 1993 by T.C. Zhao.  All rights reserved.
 *
 *  Permission to do anything with this software non-commercial and
 *  without fee is hereby granted, provided that the copyright notice
 *  and disclaimer appear in all copies and their documentation.
 *
 *  This software is provided "as is" without expressed or implied
 *  warranty of any kind.
 *.
 *
 * Very simple debug version of malloc family. It works by replacing the
 * standard malloc and its cousins with  my own routines that track, among
 * other things, where mallocs are called and how many bytes they get. All
 * these relies on the the work of true malloc. A true debug version of
 * malloc should replace malloc with calls to sbrk. Anyway, All I want out of
 * this hack is to make sure there are no memory leaks.
 */

#ifndef BIT_DMALLOC_H
#define BIT_DMALLOC_H

/* malloc from malloc.h is supposedly faster */
#if defined(M_DBG) || !defined(USE_STDLIB_H)

#ifdef lint
#include "unistd.h"
#include "ulocks.h"		/* shut up bitching about uptr_s */
#endif
/*
 * include stdlib.h so that all stuff from stdlib will NEVER overwrite things
 * from malloc.h
 */
#include <stdlib.h>
#include <malloc.h>
#else
#include <stdlib.h>
#endif

extern void set_mem_warn(int);
extern int get_mem_warn(void);
extern void mem_stat(void);

#ifdef  M_DBG
/* prototypes */
extern void *dbg_malloc(size_t, const char *, int);
extern void *dbg_calloc(size_t, size_t, const char *, int);
extern void *dbg_realloc(void *, size_t, const char *, int);
extern void *dbg_getmat(int, int, size_t, const char *, int);
extern char *dbg_strdup(const char *, const char *, int);
extern void dbg_free(void *, const char *, int);
extern void Tfree(void *);

#ifndef M_DBG_OWNER
/* actual replacememnt */
#define malloc(a)        dbg_malloc(a,__FILE__,__LINE__)
#define calloc(a,b)      dbg_calloc(a,b,__FILE__,__LINE__)
#define realloc(a,b)     dbg_realloc(a,b,__FILE__,__LINE__)
#define free(a)          dbg_free(a,__FILE__,__LINE__)
#define strdup(a)        dbg_strdup(a,__FILE__,__LINE__)

#ifndef get_mat
#define get_mat(a,b,c)   dbg_getmat(a,b,c,__FILE__,__LINE__)
#endif

#endif /* ! M_DMG_OWNER  */

#else /* if not debug, tfree becomes free */

#define Tfree(p)               free(p)
extern void *get_mat(int, int, size_t);
#endif /* M_DBG */

#endif /* _MY_MALLOC_H */

