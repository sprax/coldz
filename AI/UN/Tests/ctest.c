/*****************************************************************************
 ctest.c    S.R.Lines    93.5.5-5.26
 Test a C compilier -- this code is meant to compile on anything, provided
 all the right macros are defined; it tests for "advanced" as well as
 "retrograde features", hopefully not relying on any.
 *****************************************************************************/
#include	"unLib.h"
#include	<math.h>       /* cosh, frexp */
#include	<stdio.h>      /* FILE, fopen, ferror, printf, perror */
#include	<stdlib.h>     /* exit, malloc */
#include	<stddef.h>     /* NULL, size_t */
#include	<string.h>     /* memcpy, memset; strcpy, strcoll, strerror */
#ifndef	UN_STRINGS_H
#   ifndef WIN32
#include	<strings.h>     
#   endif
#endif
#include	<time.h>       /* time_t, CLOCKS_PER_SEC */
#ifndef WIN32
#ifndef	UN_UNISTD_H
#include	<unistd.h>	/* getpid, etc. */
#endif
#endif

#ifndef	_NO_PROTO
void	testVoid(void);
#else
void	testVoid();
#endif

/*_____________________	MAIN(ARGC,ARGV,ENVP) ________________________________*/
#ifndef  UN_ENVP
int main (argc,argv,envp)  int argc;  char **argv; char **envp;
#else
#  ifndef NO_PROTO
int main (int argc, char **argv)
#  else
int main (argc, argv)  int argc;  char **argv;
#  endif
#endif
{   
#ifndef	UN_ENVP
int  j;
for (j=0; envp[j] != NULL; j++)
  printf("%2d: %s\n",j,envp[j]);
#else
  printf("UN_ENVP is defined as (%d)\n",UN_ENVP);
#endif

#ifdef	__STDC__
    printf("__STDC__ IS defined, as (int) %d\n",(int)__STDC__);
#else
    printf("__STDC__ is NOT defined\n");
#endif
    testVoid();



    return 0;
}

void testVoid() { ; }


#if 0
    /*____________________ Assignment ____________________*/

    cp = "String assignment";


    /*____________________ Return ____________________*/
/*_____________ Declaration & Initialization __________*/
    int  j;
    char *cp=NULL, cc, cx = cc;	/* Should generate Warning */
#ifndef UN_INIT_ARRAY
    int tni[3] = { 1,2,3 };
#else
    int tni[3];
#endif
#endif

