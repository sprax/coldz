/******************************************************************************
 UnTest.c -- test for common unix routines    S.R.Lines (spraxlo@ai)    93.5.13
 Usage: First make libun.a (e.g.: cc -c -D${MFGR} un*.c; ar cruv libun.a un*.o;
 ranlib libun.a), then try making this test: cc -D$(MFGR) UnTest.c libun.a,
 where $MFGR==DEC,HP,SUN, etc.
 All symbols should be resolvable just by editing unLibPrivate.h.
 (If this library gets too big, each module will get its own header.)
 If this test links but running it gives funny results, chances are that some
 function prototypes are missing or wrong.  ANSI C recommended.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include <unLib.h>
#include <unBug.h>
#include <unTypes.h>
#define   UNTIME  1     /* use static variables defined in unTime.h */
#include "unTime.h"

#define SLEN 128
 
#ifndef THINK_C
int main(int argc, char **argv, char **envp)
#else
#include <MacUnix.h>
char *ProgName = "UnTest";
int Main(int argc, char **argv, char **envp)
#endif
{
  char *dups=NULL;
  double vp;
  int	j,k;
  dbl	d;
  char	*ss=NULL;
  float	*fp, ff;
  double dd;
  
  

  fp = (float *)calloc((size_t)1723, sizeof(float));
  for (dd = j = 0; j < 1723; j++)
    dd += fp[j]*fp[j];
  printf("Sqr sum from calloc(count,size): %g\n", dd);
  
  fp = (float *)calloc(sizeof(float), (size_t)1723);
  for (dd = j = 0; j < 1723; j++)
    dd += fp[j]*fp[j];
  printf("Sqr sum from calloc(size,count): %g\n", dd);
  
  memset(fp,0,sizeof(float)*1723);
  for (dd = j = 0; j < 1723; j++)
    dd += fp[j]*fp[j];
  printf("Sqr sum after memset(fp,0,size): %g\n", dd);
  
  /*
  rowZeroFlt(&fp,1723,1);
  for (dd = j = 0; j < 1723; j++)
    dd += fp[j]*fp[j];
  printf("Sqr sum after rowZeroFlt: %g\n", dd);
  */
   
  return 0;
  
  
  BEGCLOCK("UnTest");
  for (j = 1; j < argc; j++) {		/* testing argType from unParse.c */
    k = argType(&vp, argv[j], j);    
    if (k == INT_)
      printf("Arg %d (%s) is an INT_, value %d\n", j, argv[j], k=(int)vp);
    else if (k == DOUBLE_)
      printf("Arg %d (%s) is a DOUBLE_, value %g\n", j, argv[j], d=vp);
    else if (k == STRING_)
      printf("Arg %d (%s) is a STRING_\n",j,argv[j]);
    else if (k == -1)
      printf("Arg %d (%s) is UNKNOWN: name beginning w/ a number?\n",j,argv[j]);
  }

printf("\n\
________________________________unString__________________________________\n");
ss = strdup("Was this string strdup'd?");
printf( "strdup:  %s\n", ss);
printf("\n\
________________________________unMath____________________________________\n");
srandom(unTold);
srand  (unTold);
srand48(unTold);
printf("random:	%d  %d  %d  %d  %d  %d  %d\n"
,random(), random(), random(), random(), random(), random(), random());

printf("drand48: %g  %g  %g  %g  %g  %g  %g\n"
,drand48(), drand48(), drand48(), drand48(), drand48(), drand48(), drand48());

printf("\nanint:\t");
dd= -2.7;	printf("  (% .1f: % .1f)",	dd, anint(dd));
dd= -2.3;	printf("  (% .1f: % .1f)",	dd, anint(dd));
dd= -5.5;	printf("  (% .1f: % .1f)",	dd, anint(dd));
dd= +5.5;	printf("  (% .1f: % .1f)",	dd, anint(dd));
dd= -5.499999;	printf("  (% .7f: % .6f)",	dd, anint(dd));
dd= +5.499999;	printf("  (% .7f: % .6f)",	dd, anint(dd));

printf("\nirint:\t");
dd= -2.7;	printf("  (% .1f: % d)",	dd, irint(dd));
dd= -2.3;	printf("  (% .1f: % d)",	dd, irint(dd));
dd= -5.5;	printf("  (% .1f: % d)",	dd, irint(dd));
dd= +5.5;	printf("  (% .1f: % d)",	dd, irint(dd));
dd= -5.499999;	printf("  (% .7f: % d)",	dd, irint(dd));
dd= +5.499999;	printf("  (% .7f: % d)",	dd, irint(dd));

printf("\nirintf:\t");
ff= -2.7F;	printf("  (% .1f: % d)",	ff, irintf(ff));
ff= -2.3F;	printf("  (% .1f: % d)",	ff, irintf(ff));
ff= -5.5F;	printf("  (% .1f: % d)",	ff, irintf(ff));
ff= +5.5F;	printf("  (% .1f: % d)",	ff, irintf(ff));
ff= -5.499999F;	printf("  (% .7f: % d)",	ff, irintf(ff));
ff= +5.499999F;	printf("  (% .7f: % d)",	ff, irintf(ff));

printf("\n");
ENDCLOCK("UnTest");
die ("That's all for UnTest, baby!");
}



