/*******************************************************************************
 unLibPrivate.h --only for libun.a srcs.    S.R.Lines (spraxlo@ai)    93.6.27
 This is the "internal switchboard" telling what to define for each environment.
 ******************************************************************************/

/* may break on old compilers */
#if (defined(DEC) || defined(HP)) && !defined(__GNUC__) && !defined(const)
#define const		/* nothing: prevents "const not yet implemented" msg.*/
#endif

#ifdef DEC		/* Usually an old MIPS compiler */
#ifndef	__GNUC__
#define const		/* nothing: prevent "const not yet implemented" msg.*/
#endif
#define UN_STRDUP	1
#define UN_FOOLM	1
#else
#define NaN		(0.0)    /* Not a Number: what's the point? */
#endif

#ifdef HP
#define	UN_MAXLDBL	1	/* Attempt to fix losing math.h */
#ifndef	__GNUC__	/* The worst!  No other ANSI C compiler besides gcc?*/
#define const		/* nothing: prevent "const not yet implemented" msg.*/
#endif
#define UN_CBRT 	1
#define UN_DRAND48	1
#define UN_RANDOM	1
#define UN_IRINT	1
#define UN_RINT		1
#define UN_STRINGS_H	1
#define UN_FOOLS	1
#endif

#ifdef	THINK_C		/* Macintosh THINK C defines this automatically */
#include <unix.h>
#define UN_ANINT	1
#define UN_BCOPY	1
#define UN_BZERO	1
#define UN_BSET 	1
#define UN_CBRT 	1
#define UN_LOG2 	1
#define UN_RANDOM	1
#define UN_AINT 	1
#define UN_RINT 	1
#define UN_NINT 	1
#define UN_IRINT 	1
#define UN_DRAND48	1
#define UN_STRDUP	1
#define UN_MKDIR	1
#endif

#ifdef	MAC_MPW		/* What does MPW auto-define?  I edit the makefile. */
#define UN_CBRT 	1
#define UN_RANDOM	1
#define UN_DRAND48	1
#define UN_STRDUP	1
#endif

#ifdef NeXT		/* Skimpy version of GCC 2.3(?) + a few ObjC extns. */
#include <libc.h>	/* extern long random(void); */
#define	UN_UNISTD_H
#define UN_CBRT 	1
#define UN_LOG2 	1
#define UN_ANINT	1
#define UN_DRAND48	1
#define UN_IRINT	1
#define UN_STRDUP	1
#endif

	/* SGI_CC is defined internally by /usr/bin/cc -> /lib/driver */
#if	defined(__sgi) || defined(SGI)	/* Treat sgi cc & gcc the same. */
#define	UN_ANINT	1	/* Only in SGI's fortran */
#define	UN_NINT 	1	/* Only in SGI's fortran */
#define	UN_IRINT	1
#define	UN_IRINTF	1
#define UN_LOG2 	1
#endif

#ifdef	SUN		/* Use acc & avoid /bin/cc==/usr/ucb/cc if you can! */
#define UN_IRINTF	1
#define UN_CBRT 	1
#define	UN_FOOLS	1
#endif


