/*****************************************************************************
 unBug.h    S.R. Lines  (spraxlo@ai.mit.edu)   93.6.27-8.17
 No way to #define a function taking a varying number of args into ";", hence
 the need for some macro-function pairs with a fixed number of args: EQQ#,WQQ#.
 *****************************************************************************/
#ifndef unBug_h__
#define unBug_h__

#ifndef UNBUG
#define UNBUG 0
#endif

#if	defined(assert) && !defined(NDEBUG) && defined(SUN) 
#include	<stdio.h>       /* Sun's loser assert.h needs stdio.h */
#endif

void	die (const char *fmt, ...); 		/* fprintf(stderr,...) & exit(-1) */
void	warn(const char *fmt, ...); 		/* fprintf(stderr,...), no exit */
void	stay(const char *fmt, ...);
void	eqq (const char *fmt, ...); 		/* Print and Exit, #if UNBUG */
void	wqq (const char *fmt, ...); 		/* Print only, cc only #if UNBUG */ 
void	Usage (char *fmt, ...);
int 	unassume(char *, char *, int);		/* Still defined if !UNBUG */
void	tstNew (unsigned options, char **argStr, int *argInt, double *argDbl);

#if UNBUG /*________ All the rest is compiled only if UNBUG */

#define STAMP_FTD(ss)   fprintf(stderr,ss,__FILE__,__TIME__,__DATE__)
#define STAY(ss)        stay("%s:%d: %s",__FILE__,__LINE__,(ss[0] ? ss : ""));

/* #undef    NDEBUG	--was an experiment. */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#if 1
#define  COREDUMP       sprintf((char *)0,"Hello, World!\n");
#else
#define  COREDUMP       {char *_c_d_ = (char *) -1; *_c_d_ = 'c';}
#endif

#define  assume(x)	((void) ((x) || unassume(#x, __FILE__, __LINE__)))
#define  WQQ(A)			fprintf(stderr,"QQ: %s\n",A)
#define  WQQ1(A)		fprintf(stderr,A)    /* A is format string */
#define  WQQ2(A,B)		fprintf(stderr,A,B)
#define  WQQ3(A,B,C)		fprintf(stderr,A,B,C)
#define  WQQ4(A,B,C,D)		fprintf(stderr,A,B,C,D)
#define  WQQ5(A,B,C,D,E)	fprintf(stderr,A,B,C,D,E)
#define  WQQ6(A,B,C,D,E,F)	fprintf(stderr,A,B,C,D,E,F)
#define  WQQ7(A,B,C,D,E,F,G)	fprintf(stderr,A,B,C,D,E,F,G)
#define  EQQ(A)			fprintf(stderr,"QQ: %s\n",A), exit(0)
#define  EQQ1(A)		fprintf(stderr,A), exit(0)
#define  EQQ2(A,B)		fprintf(stderr,A,B), exit(0)
#define  EQQ3(A,B,C)		fprintf(stderr,A,B,C), exit(0)
#define  EQQ4(A,B,C,D)		fprintf(stderr,A,B,C,D), exit(0)
#define  EQQ5(A,B,C,D,E)	fprintf(stderr,A,B,C,D,E), exit(0)
#define  EQQ6(A,B,C,D,E,F)	fprintf(stderr,A,B,C,D,E,F), exit(0)
#define  EQQ7(A,B,C,D,E,F,G)	fprintf(stderr,A,B,C,D,E,F,G), exit(0)

#else	/* ! UNBUG */

#define  COREDUMP       ;

#define  assume(x)  ((void) 0)		/* Make assume(x) == FALSE */
#define  WQQ(A)
#define  WQQ1(A)
#define  WQQ2(A,B)		
#define  WQQ3(A,B,C)		
#define  WQQ4(A,B,C,D)		
#define  WQQ5(A,B,C,D,E)	
#define  WQQ6(A,B,C,D,E,F)
#define  WQQ7(A,B,C,D,E,F,G)	
#define  EQQ(A)		
#define  EQQ1(A)
#define  EQQ2(A,B)		
#define  EQQ3(A,B,C)	
#define  EQQ4(A,B,C,D)		
#define  EQQ5(A,B,C,D,E)
#define  EQQ6(A,B,C,D,E,F)	
#define  EQQ7(A,B,C,D,E,F,G)

#endif	/* UNBUG */


#endif  /* unBug_h__ */


