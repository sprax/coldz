/*****************************************************************************
 testTypes.c    S.R.Lines    93.5.27
 Test a C compiler's handling of basic types, storage classes, typedefs, etc.
 Intended mainly for (purportedly) ANSI-compliant cc's.
 NOTES:	The IC typedef works as synonym for CI only on file-gobal level,
 not for declarations within function scope.
 *****************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#undef	PI
#if	defined(SUN) && defined(__GNUC__)
extern char *printf();
#endif

int const foo = 8;

typedef const int           CI;		/* const int */
typedef       int   const   IC;		/* "integer const" (bad style) */
typedef       int          *PI;		/* ptr to int */

typedef       int  *const   CPI;	/* const ptr to int */
typedef const int          *PCI;	/* ptr to const int */
typedef       int   const  *PIC;	/* ptr to "integer const" */
typedef       int         **PPI;	/* ptr to ptr to int */

typedef       int   const **PPIC;	/* ptr to ptr to "integer const" */
typedef const int         **PPCI;	/* ptr to ptr to const int */
typedef       int  *const  *CPPI;	/* const ptr to ptr to int -OR- */
typedef       int **const   PCPI;	/* ptr to const ptr to int -WHICH? */
typedef const int  *const   CPCI;	/* const ptr to const int */

typedef const int  *const  *PCPCI;	/* ptr to const ptr to const int */

typedef    double           T;		/* Test type */
typedef const   T           CT;		/* const T */
typedef         T          *PT;		/* ptr to T */
typedef         T  *const   CPT;	/* const ptr to T */
typedef const   T          *PCT;	/* ptr to const T */
typedef const   T  *const   CPCT;	/* const ptr to const T */

#if 0
typedef        CT          *pCT;	/* Now for the real test types */
typedef         T          *PT;		/* ptr to T */
typedef         T  *const   CPT;	/* const ptr to T */
typedef const   T          *PCT;	/* ptr to const T */
typedef const   T  *const   CPCT;	/* const ptr to const T */
#endif



#ifdef _NO_PROTO
void tt()
#else
void tt()
#endif
{
  printf("Int works.\n");
}


IC bad;		/* These should get at least a warning, but don't */
CI good;

void main(int argc, char **argv)
{
  int j,k;
  printf("_______________________TEST_C_TYPES_______________________\n\n");


  j = bad;
  k = good;

/*
  bad = -1;
  good = 1;
*/

}



