/*****************************************************************************
 unLib.h -- common unix library routines    S.R.Lines (spraxlo@ai)    93.5.18
 Usage: #include <unLib.h> in sources linked with libun.a
   Eventually, this file may split into unMath.h, unString.h, etc., but I'm
 only adding functions as I need them.
   Ideally, all declarations herein would depend on which libraries are
 actually available in the environment in which this module is included.
 In practice, it depends entirely on macro definitions likely to coincide with
 which libraries are used -- e.g. strdup is not commonly available on any DEC
 machines (VAX or MIPS), though some sites may have it installed in GNU glibc.
 In cases I know about, compiler predefines are used instead of user-defined
 macros, e.g. __STDC__,__GNUC__,SGI_CC, etc.
   ANSI C is recommended; this code supports cc's which don't implement const,
 but I draw the line at no prototypes.  (If you really must use an old ucb cc,
 add in the #ifdef _NO_PROTO convention.)
 *****************************************************************************/
#ifndef unLib_h__
#define unLib_h__

#define LOG2  log(2.0)

#ifdef WIN32
#include <stdio.h>
#endif

#ifdef DEC		/* Usually an old MIPS compiler */
#ifndef	__GNUC__
#define const		/* nothing: prevents "const not yet implemented" msg.*/
#endif
#define UN_STRDUP	1
#define UN_FOOLM	1
#else
#define NaN		(0.0)    /* Not a Number: what's the point? */
#endif

#ifdef HP
#ifndef	__GNUC__	/* The worst!  No other ANSI C compiler besides gcc? */
#define const		/* nothing: prevents "const not yet implemented" msg.*/
#endif
#define	UN_MAXLDBL	1	/* Attempt to fix losing math.h */
#define UN_DRAND48	1
#define UN_RANDOM	1
#define UN_IRINT	1
#define UN_RINT		1
#define UN_STRINGS_H	1
#define UN_FOOLS	1
#endif

#ifdef	THINK_C		/* Macintosh THINK C defines this automatically */
#include <unix.h>
#define UN_BZERO	1
#define UN_RANDOM	1
#define UN_RINT 	1
#define UN_DRAND48	1
#define UN_STRDUP	1
#endif

#ifdef	MAC_MPW		/* What does MPW auto-define?  I edit the makefile. */
#define UN_RANDOM	1
#define UN_DRAND48	1
#define UN_STRDUP	1
#endif

#ifdef NeXT		/* Skimpy version of GCC 2.3(?) + a few ObjC extns. */
#include <libc.h>	/* extern long random(void); */
#define	UN_UNISTD_H
#define UN_DRAND48	1
#define UN_IRINT	1
#define UN_STRDUP	1
#endif

/* #ifdef SGI_CC */	/* Internally defined by /usr/bin/cc -> /lib/driver */
#ifdef	SGI		/* Treat sgi cc & gcc the same. */
#define	UN_IRINT	1
#define UN_FOOLS	1
#endif

#ifdef	SUN		/* Use acc & avoid /bin/cc==/usr/ucb/cc if you can! */
#define	UN_FOOLM	1
#define	UN_FOOLS	1
#endif


#ifdef	UN_RANDOM
#define SRANDOM(seed)	srand((int)seed)
#define RANDOM()	rand()	/* RANDOM() --> rand() */ 
int	srandom(unsigned);	/* doesn't return a value -- should be void */
long	random(void);
char	*initstate(unsigned int seed, char *state, int n);
char	*setstate(char *state);
#endif

#ifdef	UN_DRAND48
#ifdef	UN_RANDOM
#define SRAND48(seed)	srand((int)seed)
#define DRAND48		((double)rand() / RAND_MAX)
#else
#define SRAND48(seed)	srandom((int)seed)
#define DRAND48		((double)random() / INT_MAX)
#endif
#endif
#ifndef	_INVAL_REDECL_DRAND48	/* cc complains about duplicate proto?	*/
void	srand48 (long seedval);	/* On some BSD-flavor Unixes, drand48	*/
double	drand48 (void);		/* is defined (-lm), but not declared;	*/
long	lrand48 (void);		/* this includes Sun and DEC.		*/
long	mrand48 (void);
double	erand48 (unsigned short xsubi[3]);
long	nrand48 (unsigned short xsubi[3]);
long	jrand48 (unsigned short xsubi[3]);		/* xsubi */
void	lcong48 (const unsigned short [7]);		/* param */
unsigned short *seed48 (const unsigned short [3]);	/* seed16v */
#endif

#ifdef	UN_RINT
#define	RINT(D)		((double)floor((D)+0.5))
#if 0
#define	RINT(D)		((double)((D)<0.0 ? ceil((D)-0.5) : floor((D)+0.5)) 
#endif
extern	double	rint(double);
#endif

#ifdef	UN_IRINT
#define	IRINT(A)	((int)(((A)<0.0)?ceil((double)(A)):floor((double)(A))))
#if 0	/* perhaps faster, but less portable: */
#define	IRINT(A)	((int)((A >= 0) ? (A) : (A-1)))
#endif
int	irint(double d);
#endif

#ifdef  UN_BZERO
#define BZERO(ptr,sz)	memset(ptr,0,sz)
void	bzero(char *ptr, size_t size);
#endif

#ifdef	UN_STRDUP
#if	defined(HP) || defined(SUN)
extern char	*strdup(char *src);
#else
extern char	*strdup(const char *src);
#endif
#endif

 

#ifndef MIN
#define MAX(a,b)        (((a)>(b)) ? (a) : (b))
#define MIN(a,b)        (((a)<(b)) ? (a) : (b))
#endif

		/* from unMem.c */
extern	void	*recalloc (void* oldptr, size_t count, size_t eltsize);


/* from what used to be aaString.c */
#ifndef _NO_PROTO

extern  int   strncpylower(char *dst, const char *src, int maxlen);
extern  int   strcpylower(char *dst, const char *src);   /* non-ASCII OK? */
extern  char *string_into_int (register char *str, int *np);
extern  char *string_into_signed_int (register char *str, int *np);
extern  char *string_to_lower ( char *str );
extern  char *string_to_upper ( char *str );

#else   /* ifndef _NO_PROTO */

extern  int   strncpylower();
extern  int   strcpylower();
extern  char *string_into_int ();
extern  char *string_into_signed_int ();
extern  char *string_to_lower ();
extern  char *string_to_upper ();

#endif  /* _NO_PROTO */

#endif  /* unLib_h__ */


