/****************************************************************************
 testVat.c -- test unArgs.h vat_ macros for tabled size varargs.  S.R.Lines
 This file is an excerpt from the notes in unArgs.h.
 ****************************************************************************/

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
  float parm2 = -9876.5432F;
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


