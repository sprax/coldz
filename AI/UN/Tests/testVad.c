/*****************************************************************************
 testVad.c  --  test va_last & va_dec macros from unArgs.h.    S.R.Lines
 This entire file is an excerpt from the notes in unArgs.h.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "unArgs.h"

static void print_dbl(const char *fmt, ...)
{
  va_list  args;
  va_start(args,fmt);
  vfprintf(stderr,fmt,args);
}

#define NARG 6
static double dd[NARG];   /* All initialized to 0 in BSS. */

static void vadbl(void *varg, ...)
{
  char *pc, fmt[256] = "";
  int j, narg;
  va_last(varg,narg,double);
  /*{static tc = 0; fprintf(stderr,"vadbl call %d, narg: %d\n",tc++,narg);}*/
  
  /* Now create a format for narg doubles, setting dd & j as side effects: */
  if (narg > NARG)  narg = NARG;           /* cap narg */
  for (j = 0; narg; j++) {
    dd[--narg] = va_dec(varg,double);
    strcat(fmt,"%g ");
  }
  strcat(fmt,"\n");
  
  /* The following calls to vfprintf and print_dbl should all produce
     exactly the same results.  Stupid, but this is only a test. */
  vfprintf(stderr,fmt,(va_list)((double *)varg + 1));
  vfprintf(stderr,fmt,(va_list)((unsigned long)varg+sizeof(double)));
  pc = "dd: %g %g %g %g %g %g\n";
  fprintf(stderr,pc,dd[0],dd[1],dd[2],dd[3],dd[4],dd[5]);
  print_dbl(pc, dd[0], dd[1], dd[2], dd[3], dd[4], dd[5]);
  switch(j) {
    case  0: break;
    case  1: print_dbl(fmt,dd[0]); break;
    case  2: print_dbl(fmt,dd[0], dd[1]); break;
    case  3: print_dbl(fmt,dd[0], dd[1], dd[2]); break;
    case  4: print_dbl(fmt,dd[0], dd[1], dd[2], dd[3]); break;
    case  5: print_dbl(fmt,dd[0], dd[1], dd[2], dd[3], dd[4]); break;
    case  6: print_dbl(fmt,dd[0], dd[1], dd[2], dd[3], dd[4], dd[5]); break;
    default: fprintf(stderr,"vadbl can't handle %d args!\n",j);
  }
}


void main()
{
  //int j;
  /*for (j = 0; j < NARG; j++)  fprintf(stderr,"dd[%d] is %g\n",j,dd[j]);*/
  vadbl(0);
  vadbl((void*) 1, 3.1459);
  vadbl((void*) 2, 3.1459, 2.71);
  vadbl((void*) 3, 3.1459, 2.71, 3.45);
  vadbl((void*) 4, 3.1459, 2.71, 3.45, 4.56);
  vadbl((void*) 5, 3.1459, 2.71, 3.45, 4.56, 5.67);
}


