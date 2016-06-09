/*****************************************************************************
 rowSmatN.c    S.R.Lines     93.9.24-11.23
 *****************************************************************************/
#include	<string.h>
#include	<un/unBug.h>
#include	<un/unMath.h>
#include	<un/unTypes.h>
#include        <rowZero.h>
#include        <mj.h>
#include        <rowSmat.h>

#define 	MAX_NUM 9

void rowSmatBilN ( ppf dstT, ppf dstW, ppf *srcS, ppf *fwdX, ppf *fwdY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH
                 , int num, flt clrs[4], dbl trs, dbl infimum)
{
  int	dx1 = dX + dW - 1;
  int	dy1 = dY + dH - 1;
  int	sy1 = sY + sH - 1;
  int	sx1 = sX + sW - 1;
  ppf	XvA = fwdX[0] + sY, YvA = fwdY[0] + sY, srcA = srcS[0] + sY;

  pfl	XpA, YpA, Xp[MAX_NUM], Yp[MAX_NUM], pM;
  int	jA, kA,  jT, kT, mm, numM = num-1, scanM = srcA[1] - srcA[0] - 1;
  flt	f0,f1,f2, g0,g1,g2;
  flt	cA = clrs[0];
  flt	fac = (flt) trs;	/* NB: expect fac == 1.0, at least as of 93.9.12 */
  flt	inf = (flt) infimum;
  flt	yM, xM, vT, wT, xT, yT;
  rgst	int jM, kM;
  rgst  flt v1, v2;


  warn("rowSmatBilN: This is still just an unchanged clone of rowSmatBiqN");
  
  if (num > MAX_NUM)
    die("rowSmatBilN: parameter num > MAX_NUM (src images)");
  
  memset((pvd)(dstW[dY] + dX), 0, sizeof(flt)*dW*dH);
  memset((pvd)(dstT[dY] + dX), 0, sizeof(flt)*dW*dH);

  jA = sY, mm = numM;
  do {
    fwdX[mm] += jA,
    fwdY[mm] += jA,
    srcS[mm] += jA;
  } while (--mm);

  for ( ; jA <= sy1; jA++) {

    kA  =  sX, mm = numM;
    do {
      Xp[mm] = *(fwdX[mm])++ + kA,
      Yp[mm] = *(fwdY[mm])++ + kA;
    } while (--mm);
    XpA = *XvA++ + kA, YpA = *YvA++ + kA;
    
    for ( ; kA <= sx1; kA++) {

      yT  = jA - fac * *YpA++;		/* NB: expect trs == 1.0 */
      jT  = IFLOOR(yT);
      if (dY <= jT && jT < dy1) {	/* Target y value is within dst area */
        xT  = kA - fac * *XpA;
        kT  = IFLOOR(xT);
        if (dX <= kT && kT < dx1) {	/* Target x value is also within dst area */

          wT  = cA;
          vT  = cA*srcA[jA][kA];

          /* @@ The following extractions of a pixvals are bilinear, not biquadratic */
          
          mm = numM;
          do {				/* while (--mm) */
            xM  = kA - *(Xp[mm])++;	/* NB: srcB(yM,xM) is independent of fac */ 
            kM  = IFLOOR(xM);
            if (sX <= kM && kM < sx1) {
              yM  = jA - *(Yp[mm])++;
              jM  = IFLOOR(yM);
              if (sY <= jM && jM < sy1) {	/*  src[mm](jM,kM) is in bounds */
                xM -= kM;
                yM -= jM;
                /* Assuming all rows in src[mm], etc, are separated/spaced equally: */
                pM  =  srcS[mm][jM] + kM;
                v1  =  *pM++;
                v1 += (*pM - v1) * xM;
                pM +=  scanM;
                v2  =  *pM++;
                v2 += (*pM - v2) * xM;
                v1 += ( v2 - v1) * yM;
                wT += clrs[mm];
                vT += clrs[mm]*v1;
              }
            }
            else (Yp[mm])++;
          }  while (--mm);
          

          if (wT > inf) {


            /* Now (vT/wT) would yield weighted mean of interpolated values,
               but this is postponed til after distributing it over a 3x3 pxl block: */
            
            xT -= kT;
            v1  = ABS(xT);
            f1  = (1.0F - v1)*(1.0F + v1);
            v1 *=  0.5F;
            f0  = (1.0F - xT)*v1;
            f2  = (1.0F + xT)*v1;
	  			/* Which are better optimized: f's or g's? */
            yT -= jT;
            v1  = ABS(yT);
            g0  = (1.0F - yT);
            g2  = (1.0F + yT);
            g1  = g2*g0;
            v1 *= 0.5F;
            g0 *= v1;
            g2 *= v1;

            v1            = wT; 	/* NB: expect wT to be 1.0 == sum(clrs) */
            
            v2            = v1*f1*g1;
            dstW[jT][kT] += v2;
            dstT[jT][kT] += v2*vT;
 
            kT++;
            v2            = v1*f2*g1;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            jT++;
            v2            = v1*f2*g2;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            kT--;
            v2            = v1*f1*g2;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            kT--;
            v2            = v1*f0*g2;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;
  
            jT--;
            v2            = v1*f0*g1;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;
 
            jT--;
            v2            = v1*f0*g0;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            kT++;
            v2            = v1*f1*g0;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;
  
            kT++;
            v2            = v1*f2*g0;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

          }
        }
      }
    }
  } 
}



