/*****************************************************************************
unArgs.h -- explained below.   S.R.Lines (spraxlo@ai)    93.6.20
*****************************************************************************/
#ifndef unArgs_h__
#define unArgs_h__

        /* This doesn't handle promotion of float to double! @@ */
#define stack_sizeof(a)  ( (sizeof(a) > 4) ? sizeof(a) : 4 )


/*________ VARARGS WITH TABLED SIZES: ______________________________________*/

        /* Get narg & make varg point to stack loc. of 1st arg. after varg: */
#define vat_start(v,n)	(n = (int)v, v = (void*)(&v + 1))

        /* Get last remaining arg using offset in tbl & decrement argcount: */
#define vat_dec(v,n,tbl,type)    *((type*)((size_t)v + tbl[--n]))


/*________ SA LAST & DECREMENT MACROS: _____________________________________*/

        /* Set n=argcount & make varg point to last ptr-sized (32-bit) arg: */
#define sa_last(v,n)	(n=(int)v, v = ((void**)&v + n))

        /* return last 32-bit value/ptr in arglist & postdecrement list:*/
#define sa_dec(v)	*(* (void ***) &v)--          /* (multicast) is bad */

/*________ VA LAST & DECREMENT MACROS: _____________________________________*/

        /* In case all vars of same size != sizeof(void *): set narg and    */
        /* make v point to last arg of size sizeof(type)    --Not tested.   */
#define va_last(v,n,type)	(n = (int)v, v = ((type*)(&v+1)) + (n-1))

        /* return last 32-bit value/ptr in arglist & postdecrement list:    */
#define va_dec(p,type)	*(* (type **) &p)--

/*****************************************************************************
EXPLANATIONS & EXAMPLES:

Because most (but not all) compilers push the last arg in a function call
first on the stack, functions calls within a vararg function like
foo(va_list v) { void *a; va_start(a,v); bar(va_arg(a,int),va_arg(a,float));
will usually get the float and int args to bar reversed, i.e. the int will
be total garbage, and the float will be truncated.  Switch statements on the
number of args are then also inefficient, because for each case one must
start near the stackframe center and pick off the variables in forward 
order by climbing the stack.

If, instead, one starts at the end of the arglist (or top of the stack frame),
then one can pull off the arguments in reverse order, i.e. the
last first.  The only catch is that you have to know the size and type of each
argument in order to decrement your stack pointer by the right amount and
correctly pull a value from it to use as a formal parameter or put into a
local variable.

The standard solutions are either to make the first argument a pointer to a
format string (or struct) containing all the sizes (as in the printf family),
or just to assume that all (exept maybe the first) are of one fixed size,
and put the number of them in the first arg, so that one knows how far to
skip to get to the last one.  A format string can, of course, be processed
"backwards," but the main problem with formats is that you need a different
symbol for every type, including user defined structs (which must be typedef'd).

An alternative solution for a function taking a varying number of arguments
of different sizes but in fixed order, is to use a table of offsets indexed
by arg number (hence stack_sizeof and the vat_ macros).

See MgVarg.c for an example using a very efficient switch statement as well
as an inefficient one using stdarg.h macros (basically similar to varargs.h).
To avoid casting twice, use va_dec instead of sa_dec, even with sa_last.

NOTES
stdarg.h macros va_start and va_arg provide a portable method of accessing
the stack, but are not necessary for prototypes or function args
(assume typedef void *va_list).

So far, unArgs.h is not referenced by anything in unLib, but is so named
because it is in process of being ported to different architectures.

A TEST PROGRAM
    Suppose froob(void *varg, ...) can handle 0,1,2,3, or 5 (but not 4) args
(not counting the initial va_list void* count arg),
and that they are bar (a typedef'd struct), float, int, char and short, resp.
Noting first that chars (and shorts) may be promoted to ints, and floats to
doubles, froob could then be written as:
****************************************************************************/
#if 0000000000  /* test unArgs.h vat_ macros for tabled size varargs */

/***************************************************************************
 testVat.c -- test unArgs.h vat_ macros for tabled size varargs.  S.R.Lines
 This file is an excerpt from the notes in unArgs.h.
 ***************************************************************************/

#include <stdio.h>
#include "unArgs.h"
#define SZ(a)	stack_sizeof(a)

typedef struct bar_s { int rot[4][4]; unsigned stat; } bar;
static bar DefBar;	/* will be initialized to all 0's in BSS */

static int print_parms(bar p1,float p2,int p3,char p4,short p5)
{
  return fprintf(stderr,
  "addr(p1)=%x, p1.stat=%d, p2=%.4f, p3=%d, p4=(%c), p5=%d\n"
  ,&p1,p1.stat,p2,p3,p4,p5);
}

static void froob(void *varg, ...)
{   static int tbl[] = { 0, SZ(bar),
                            SZ(bar)+SZ(double),
                            SZ(bar)+SZ(double)+SZ(int),
                            SZ(bar)+SZ(double)+SZ(int)*2 };
  bar   parm1 = DefBar;
  float parm2 = -9876.5432;
  int   parm3 = 12345;
  char  parm4 = 'c';
  short parm5 = -1;         /* Parms all initialized to default values.      */
#if 1	/* The following alternatives are equivalent: */
  int narg;
  vat_start(varg,narg);
#else
  int narg = (int)varg;     /* (un)cast first arg to get number of args.     */
  varg = (void *)(&varg+1); /* Now varg points to stack location of 1st arg. */
#endif
  {static tc = 0; fprintf(stderr,"\nfroob call %d, narg: %d\n",tc++,narg);}
  switch(narg) {
    case  5: parm5 = (short)vat_dec(varg,narg,tbl,int);
             parm4 = (char) vat_dec(varg,narg,tbl,int);     /* No breaks! */
    case  3: parm3 =        vat_dec(varg,narg,tbl,int);
    case  2: parm2 = (float)vat_dec(varg,narg,tbl,double);  /* not float! */
    case  1: parm1 =        vat_dec(varg,narg,tbl,bar);
    case  0: break;
    default: fprintf(stderr,"Warning: froob can't handle %d args\n",narg);
  }
  print_parms(parm1,parm2,parm3,parm4,parm5);
}


void main()
{
  froob(0);
  DefBar.stat = 5555;
  froob((void*)1,DefBar);
  DefBar.stat = 7777;
  froob((void*)2,DefBar,3.1459);
  froob((void*)3,DefBar,2.7123,8888,'s');
  froob((void*)4,DefBar,1.2345,6543,'t',444);
  froob((void*)5,DefBar,1.2345,6543,'e',-44);
  froob((void*)5,DefBar,3.1459,7890,'v',952);
  froob((void*)6,0);
}  

#endif /* 000000000 */


/****************************************************************************/
/****************************************************************************/
/**************** FOLLOWING IS A TEST OF va_last AND va_dec *****************/
/****************************************************************************/
/****************************************************************************/

#if 000000

/****************************************************************************
 testVad.c  --  test va_last & va_dec macros from unArgs.h.    S.R.Lines
 This entire file is an excerpt from the notes in unArgs.h.
 ****************************************************************************/
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
  int j;
  /*for (j = 0; j < NARG; j++)  fprintf(stderr,"dd[%d] is %g\n",j,dd[j]);*/
  vadbl(0);
  vadbl((void*) 1, 3.1459);
  vadbl((void*) 2, 3.1459, 2.71);
  vadbl((void*) 3, 3.1459, 2.71, 3.45);
  vadbl((void*) 4, 3.1459, 2.71, 3.45, 4.56);
  vadbl((void*) 5, 3.1459, 2.71, 3.45, 4.56, 5.67);
}


#endif /* 000000 */

/****************************************************************************/
#endif /* unArgs_h__ */



























