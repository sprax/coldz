/*****************************************************************************
rowComb.c    S.R.Lines (spraxlo@ai.mit.edu)    93.6.12-8.30
Assumptions:
	[See rowClip.c]
    comparison ((unc)'c' < 'C') promotes both sides to int (not unc,short,etc)
    comparison ((flt)'c' < 'C') promotes both sides to float (not double)
    comparison ((dbl)'c' < 'C') promotes both sides to double.
*****************************************************************************/
#undef    NDEBUG	/* mallocAss in unMem.h needs active assert(). (?)@@ */
#include <stdio.h>
#include <assert.h>
#include <un/unTypes.h>
#include "rowComb.h"

#undef	IRINTF		/* Return int nearest to float argument: want fast! */
#define IRINTF(f)  	((int)((f) < 0.0 ? ((f)-0.5F) : ((f)+0.5F)))

void
rowCombFlt4 (ppf dst, ppf src[4], int dX, int dY, unt dW, unt dH, flt wts[4])
{
int     k;
pfl     dp;
pfl     p0, p1, p2, p3;
flt     wts0 = wts[0], wts1 = wts[1], wts2 = wts[2], wts3 = wts[3];
ppf     src0 = src[0]+dY, src1 = src[1]+dY, src2 = src[2]+dY, src3 = src[3]+dY;


  for (dst += dY ; dH-- ; ) {
    p0 = *src0++ + dX;
    p1 = *src1++ + dX;
    p2 = *src2++ + dX;
    p3 = *src3++ + dX;
    dp = *dst++  + dX;
    for (k = dW ; k-- ; ) {
      *dp++ = wts0**p0++ + wts1**p1++ + wts2**p2++ + wts3**p3++;
    }
  }
}



void
rowCombFlt8 (ppf dst, ppf src[8], int dX, int dY, unt dW, unt dH, flt wts[8])
{
int k;
pfl dp;
pfl p0, p1, p2, p3, p4, p5, p6, p7;
flt wts0 = wts[0], wts1 = wts[1], wts2 = wts[2], wts3 = wts[3];
flt wts4 = wts[4], wts5 = wts[5], wts6 = wts[6], wts7 = wts[7];
ppf src0 = src[0], src1 = src[1], src2 = src[2], src3 = src[3];
ppf src4 = src[4], src5 = src[5], src6 = src[6], src7 = src[7];

  for ( ; dH-- ; ) {
    p0 = *src0++;
    p1 = *src1++;
    p2 = *src2++;
    p3 = *src3++;
    p4 = *src4++;
    p5 = *src5++;
    p6 = *src6++;
    p7 = *src7++;
    dp = *dst++;
    for (k = dW ; k-- ; ) {
      *dp++ = wts0**p0++ + wts1**p1++ + wts2**p2++ + wts3**p3++
            + wts4**p4++ + wts5**p5++ + wts6**p6++ + wts7**p7++;
    }
  }
}



void
rowComb8Unc (ppu dst, ppu src[8], int wide, int tall, float wts[8])
{
  int k;
  puc dp; 
  puc p0, p1, p2, p3, p4, p5, p6, p7;
  rgst flt fv;
  flt wts0 = wts[0], wts1 = wts[1], wts2 = wts[2], wts3 = wts[3];
  flt wts4 = wts[4], wts5 = wts[5], wts6 = wts[6], wts7 = wts[7];

  for (; tall; tall--) {
    p0 = *src[0]++;
    p1 = *src[1]++;
    p2 = *src[2]++;
    p3 = *src[3]++;
    p4 = *src[4]++;
    p5 = *src[5]++;
    p6 = *src[6]++;
    p7 = *src[7]++;
    dp = *dst++;
    for (k = wide; k--; ) {
      fv = wts0**p0++ + wts1**p1++ + wts2**p2++ + wts3**p3++
         + wts4**p4++ + wts5**p5++ + wts6**p6++ + wts7**p7++;
      *dp++ = (unc) (fv + 0.5);
    }
  }
}


void rowComb8UncRGB (ppu dst[4], ppu src[8][4], int wide, int tall, rgst flt wts[8])
{
  int j, k;
  ppu reo[4][8];
  for (j = 1; j < 4; j++)
    for (k = 0; k < 8; k++)
      reo[j][k] = src[k][j];
  rowComb8Unc(dst[1],reo[1],wide,tall,wts);
  rowComb8Unc(dst[2],reo[2],wide,tall,wts);
  rowComb8Unc(dst[3],reo[3],wide,tall,wts);
}




#if 0000
void rowCombN ( ppf dsX[9], ppf dsY[9], unt dW, unt dH, flt wts[9], unt num )
{
  int   j, k;
  unt   n, nM = num - 1;
  pfl   dx, dy, sx1, sy1, sx[9], sy[9];
  ppf   dstX = dsX[0], dstY = dsY[0];
  ppf   srcX = dsX[1], srcY = dsY[1];           /* Treat 1st 2 special */
  flt   wts1 = wts[1];
  register flt xx, yy;

  if (num < 5) {
    switch (num) {
      case 3: fxyComb3(dsX,dsY,dW,dH,wts); return;
      case 4: fxyComb5(dsX,dsY,dW,dH,wts); return;
      case 5: fxyComb5(dsX,dsY,dW,dH,wts); return;
      default: die("fxyCombN: NOIMP for num %d\n",num); return;
    }
  }
  for (j = 0; j < dH; j++) {
    dx   = *dstX++;
    dy   = *dstY++;
    sx1  = *srcX++;
    sy1  = *srcY++;
    for (n = 2; n < num; n++) {
      sx[n] = dsX[n][j];
      sy[n] = dsY[n][j];
    }
    k = dW;
    while (k--) {
      xx = *sx1++ * wts1;
      yy = *sy1++ * wts1;
      n = nM;
      do {
        xx += *sx[n]++ * wts[n],
        yy += *sy[n]++ * wts[n];
      } while (--n > 1);
      *dx++ = xx;
      *dy++ = yy;
    }
  }
}
#endif


void rowComb2Unc (ppu dst, ppu srcA, ppu srcB, int wide,int tall, double ctrs)
{
  int k;
  puc dp;  
  register puc pA, pB;
  register flt fv, fB = (flt)(ctrs), fA = (flt)(1.0 - ctrs);

  for (; tall; tall--) {
    pA = *srcA++;
    pB = *srcB++;
    dp = *dst++;
    for (k = wide; k; k--) {
      fv = (fA * *pA++ + fB * *pB++);
      *dp++ = (unc) (fv + 0.4997);
    }
  }
}

void rowComb2UU ( ppu dstM, ppu srcA, ppu srcB, int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, double ctrs)
{
  int k;
  puc dp;  
  register puc pA, pB;
  register flt fv, fB = (flt)(ctrs), fA = (flt)(1.0 - ctrs);

  assert(dW <= sW && dH <= sH);
  
  for (dstM += dY, srcA += sY, srcB += sY; dH; dH--) {
    pA = *srcA++ + sX;
    pB = *srcB++ + sX;
    dp = *dstM++  + dX;
    for (k = dW; k; k--) {
      fv = (fA * *pA++ + fB * *pB++);
      *dp++ = (unc) (fv + 0.4999);
    }
  }
}

void rowComb2FU ( ppu dstM, ppf srcA, ppf srcB, int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, double ctrs)
{
  int k;
  puc dp;  
  register pfl pA, pB;
  register flt fv, fB = (flt)(ctrs), fA = (flt)(1.0 - ctrs);

  assert(dW <= sW && dH <= sH);
  
  for (dstM += dY, srcA += sY, srcB += sY; dH; dH--) {
    pA = *srcA++ + sX;
    pB = *srcB++ + sX;
    dp = *dstM++  + dX;
    for (k = dW; k; k--) {
      fv = (fA * *pA++ + fB * *pB++);
      *dp++ = (unc) (fv + 0.4999);
    }
  }
}

void rowComb2FF ( ppf dstM, ppf srcA, ppf srcB, int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, double ctrs)
{
  int k;
  pfl dp;  
  register pfl pA, pB;
  register flt fB = (flt)ctrs, fA = (flt)(1.0 - ctrs);

  assert(dW <= sW && dH <= sH);
  
  for (dstM += dY, srcA += sY, srcB += sY; dH; dH--) {
    pA = *srcA++ + sX;
    pB = *srcB++ + sX;
    dp = *dstM++  + dX;
    for (k = dW; k; k--) {
      *dp++ = fA * *pA++ + fB * *pB++;
    }
  }
}


void rowComb2Flt(ppf dstT, ppf srcA, ppf srcB, int wide,int tall, double ctrs)
{
  int k;
  pfl dp;  
  register pfl pA, pB;
  register flt fB = (flt) ctrs, fA = 1.0F - fB;

  while (tall--) {
    pA = *srcA++;
    pB = *srcB++;
    dp = *dstT++;
    k  = wide; while (k--)
      *dp++ = fA * *pA++ + fB * *pB++;
  }
}



void rowComb2FltUnc(ppu dst,ppf srcA,ppf srcB,int wide,int tall,double ctrs)
{ /* Tacitly assuming float vals do not stray far from range -0.5 to 255.5 */
  puc dp;  
  register flt *pA, *pB, fv, fA = (flt)(1.0 - ctrs), fB = (flt)ctrs;
  register int iv, k;

  for (; tall; tall--) {
    pA = *srcA++;
    pB = *srcB++;
    dp = *dst++;
    for (k = wide; k; k--) {
      fv = (fA * *pA++ + fB * *pB++);
      iv = IRINTF(fv);
      if (iv > 255)  iv = 255;  else
      if (iv < 0  )  iv = 0;
      *dp++ = (unc)  iv;
    }
  }
}


