/*****************************************************************************
 c.c    S.R.Lines    94.5.11
 *****************************************************************************/
#define		UNTIME  0
#define		UNBUG	0
#include	<assert.h>
#include	<stddef.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<errno.h>
#include	<math.h>
#include	<un/unBug.h>
#include	<un/unMacro.h>
#include	<un/unSys.h>
#include	<un/unMem.h>
#include	<un/unTypes.h>
#include	<un/unTime.h>
#include	<libim.h>
#include	<m/mj.h>
#include	"new.h"

void jane(ppus irgb[4], int dX, int dY, unt dW, unt dH)
{
int	j,k,kE = dW-2*dX;
ppus	ru,rr,rg,rb;
pus	pu,pr,pg,pb;

  ru = irgb[0], rr = irgb[1], rg = irgb[2], rb = irgb[3];
  for (ru += dY, rr += dY, rg += dY, rb += dY, j = dH-2*dY; j-- ; ) {
    pu = *ru++ + dX, pr = *rr++ + dX, pg = *rg++ + dX, pb = *rb++ + dX;
    for (k = kE; k-- ; ) {
        *pr++ += *pg  ;
        *pg++ += ((pg[j/2] + pb[j/3] + *pb));
        *pb++ += 2; 
    }
  }
}

void janeF(ppf irgb[4], int dX, int dY, unt dW, unt dH, unt dD)
{
rflt	a=0.0, b=0.5, c=1.0;
unt	k;
ppf	ri;
pfl	pi;

  switch (dD) {
    case  1:  {
      for (ri = irgb[0] + dY; dH-- ; ) {
        if (dH % 2) for (pi = *ri++ + dX, k = dW; k-- ; )  *pi++ = 0.0;
        else        for (pi = *ri++ + dX, k = dW; k-- ; )  {
          c = *pi/255.0F;
          *pi++ += 128.0F*(c*(1.0F - c) + a*b - b);
          a = b*(1.0F + b)*0.5F;
          b = c;
        }
      }
    } return;
    default:  die("NOIMP for depth:%u",dD);
  }
}



