/*****************************************************************************
 rowSmatSin.c    S.R.Lines     93.5.15-7.30
 NOTES: if trs < TEENY, just rowCopy srcA to dstT?
 Must have dims(dstT) <= dims(fwdX) == dims(fwdY), and borders of srcs may be
 positive or negative (dims(srcA) <> dims(dstT)), but they must be known
 explicitly.  In general, dims(dstT) < dms(srcA) for best results.
 All all must be aligned (i.e., [0][0] must refer to same "place")
 --this makes the algorithm and subimage processing simple.
  
 Destination Boundaries:
 dY is the top, or coord of the first row, dW is the rectangle's width, etc.
 Source Boundaries:
 sX is the coord of leftmost stored pxl, and sx1 is the rightmost pxl from
 which to anchor an interpolation, etc. -- for bilinear interpolation,
 think of sX as 0 and sx1 as wide-2.
 *****************************************************************************/
#include	<un/unMath.h>
#include	<un/unTypes.h>
#include        <rowZero.h>
#include        <mj.h>
#include        <rowSmat.h>
 
/*___________________________________________________________________________*
 rowSmatSin, rowSmatTrn
 Single pixval, rounded pixlocs -- visible quantization errors.
 Single pixval, truncated pixlocs -- even worse.
 Collisions and holes abound, and out-of-bounds is mapped on border to see it.
 Use bilinear or bicubic interpolation instead.
*___________________________________________________________________________*/



void rowSmatSin ( ppf dstT, ppf dstW, ppf srcA, ppf fwdX, ppf fwdY
                , int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, dbl trs)
{
  int	dx1 = dX + dW;
  int	dy1 = dY + dH;
  int	sy1 = sY + sH;
  int	sx1 = sX + sW;
  int	jA, kA;
  pfl	pX, pY;
  rgst  flt ff, fac = (flt) trs;
  rgst	int kT, jT;

  rowZeroF(dstT,dX,dY,dW,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (jA = sY, fwdX += jA, fwdY += jA; jA < sy1; jA++) {
    for (kA = sX, pX = *fwdX++ + kA, pY = *fwdY++ + kA; kA < sx1; pX++, kA++) {
      ff = fac * *pY++;
      jT = jA - IRINTF(ff);
      if (dY <= jT && jT < dy1) {
        ff = fac * *pX;
        kT = kA - IRINTF(ff);
        if (dX <= kT && kT < dx1) {
          dstT[jT][kT] += srcA[jA][kA];	/* Single pixval: no clip or round */
          dstW[jT][kT] += 1.0F;
        }
      }
    }
  } 
}


void rowSmatSin2 ( ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf fwdX, ppf fwdY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr)
{
  int	dx1 = dX + dW;
  int	dy1 = dY + dH;
  int	sy1 = sY + sH;
  int	sx1 = sX + sW;
  int	jA, kA, kB, jB;
  flt	*pX, *pY, cB = (flt)clr, cA = 1.0F - cB;
  rgst	int kT, jT;
  rgst  flt ff, fac = (flt) trs;

  rowZeroF(dstT,dX,dY,dW,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (jA = sY, fwdX += jA, fwdY += jA; jA < sy1; jA++) {
    for (kA = sX, pX = *fwdX++ + kA, pY = *fwdY++ + kA; kA < sx1; pX++, kA++) {
      ff  = *pY++;
      jB  = jA - IRINTF(ff);
      ff *= fac;
      jT  = jA - IRINTF(ff);
      if (dY <= jT && jT < dy1) {
        ff  = *pX;
        kB  = kA - IRINTF(ff);
        ff *= fac;
        kT  = kA - IRINTF(ff);
        
        if (dX <= kT && kT < dx1) {	/* Single pixval: no clip or round */
          dstW[jT][kT] += 1.0F;
          if (sX <= kB && kB < sx1 && sY <= jB && jB < sy1)
            dstT[jT][kT] += cA*srcA[jA][kA] + srcB[jB][kB]*cB;
          else
            dstT[jT][kT] += srcA[jA][kA];
        }
      }
    }
  } 
}




void rowSmatSin3 ( ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, flt clrs[4], dbl trs, dbl infimum)
{
  int	dx1 = dX + dW;
  int	dy1 = dY + dH;
  int	sy1 = sY + sH;
  int	sx1 = sX + sW;
  int	jT, kT;
  ppf	XvA = fvX[0] + sY, YvA = fvY[0] + sY, srcA = srcS[0] + sY;
  ppf	XvB = fvX[1] + sY, YvB = fvY[1] + sY, srcB = srcS[1] + sY;
  ppf	XvC = fvX[2] + sY, YvC = fvY[2] + sY, srcC = srcS[2] + sY;
  pfl	XpA, YpA, XpB, YpB, XpC, YpC;
  flt	cA = clrs[0], cB = clrs[1], cC = clrs[2], fac = (flt) trs;
  flt	vA, wT;
  rgst	int jN, kN, jA, kA;
  rgst  flt ff;

  rowZeroF(dstT,dX,dY,dW,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (jA = sY; jA < sy1; jA++) {

    kA  =  sX;
    XpA = *XvA++ + kA, YpA = *YvA++ + kA;
    XpB = *XvB++ + kA, YpB = *YvB++ + kA;
    XpC = *XvC++ + kA, YpC = *YvC++ + kA;

    for ( ; kA < sx1; XpA++, XpB++, YpB++, XpC++, YpC++, kA++) {

      ff  = fac * *YpA++;
      jT  = jA - IRINTF(ff);
      if (dY <= jT && jT < dy1) {
        ff  = fac * *XpA;
        kT  = kA - IRINTF(ff);
        if (dX <= kT && kT < dx1) {

          wT  = cA;
          vA  = cA*srcA[jA][kA];

          ff  = *XpB;
          kN  =  kA - IRINTF(ff);
          if (sX <= kN && kN < sx1) {
            ff  = *YpB;
            jN  =  jA - IRINTF(ff);
            if (sY <= jN && jN < sy1) {
              wT += cB;
              vA += cB*srcB[jN][kN];
            }
          }

          ff  = *XpC;
          kN  =  kA - IRINTF(ff);
          if (sX <= kN && kN < sx1) {
            ff  = *YpC;
            jN  =  jA - IRINTF(ff);
            if (sY <= jN && jN < sy1) {
              wT += cC;
              vA += cC*srcC[jN][kN];
            }
          }

          dstW[jT][kT] += wT;
          dstT[jT][kT] += vA;
        }
      }
    }
  } 
}









void rowSmatSin4 ( ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, flt clrs[4], dbl trs, dbl infimum)
{
  int	dx1 = dX + dW;
  int	dy1 = dY + dH;
  int	sy1 = sY + sH;
  int	sx1 = sX + sW;
  int	jT, kT;
  ppf	XvA = fvX[0] + sY, YvA = fvY[0] + sY, srcA = srcS[0] + sY;
  ppf	XvB = fvX[1] + sY, YvB = fvY[1] + sY, srcB = srcS[1] + sY;
  ppf	XvC = fvX[2] + sY, YvC = fvY[2] + sY, srcC = srcS[2] + sY;
  ppf	XvD = fvX[3] + sY, YvD = fvY[3] + sY, srcD = srcS[3] + sY;
  pfl	XpA, YpA, XpB, YpB, XpC, YpC, XpD, YpD;
  flt	cA = clrs[0], cB = clrs[1], cC = clrs[2], cD = clrs[3], fac = (flt) trs;
  flt	vA, wT;
  rgst	int jN, kN, jA, kA;
  rgst  flt ff;

  rowZeroF(dstT,dX,dY,dW,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (jA = sY; jA < sy1; jA++) {

    kA  =  sX;
    XpA = *XvA++ + kA, YpA = *YvA++ + kA;
    XpB = *XvB++ + kA, YpB = *YvB++ + kA;
    XpC = *XvC++ + kA, YpC = *YvC++ + kA;
    XpD = *XvD++ + kA, YpD = *YvD++ + kA;

    for ( ; kA < sx1; XpA++, XpB++, YpB++, XpC++, YpC++, XpD++, YpD++, kA++) {

      ff  = fac * *YpA++;
      jT  = jA - IRINTF(ff);
      if (dY <= jT && jT < dy1) {
        ff  = fac * *XpA;
        kT  = kA - IRINTF(ff);
        if (dX <= kT && kT < dx1) {

          wT  = cA;
          vA  = cA*srcA[jA][kA];

          ff  = *XpB;
          kN  =  kA - IRINTF(ff);
          if (sX <= kN && kN < sx1) {
            ff  = *YpB;
            jN  =  jA - IRINTF(ff);
            if (sY <= jN && jN < sy1) {
              wT += cB;
              vA += cB*srcB[jN][kN];
            }
          }

          ff  = *XpC;
          kN  =  kA - IRINTF(ff);
          if (sX <= kN && kN < sx1) {
            ff  = *YpC;
            jN  =  jA - IRINTF(ff);
            if (sY <= jN && jN < sy1) {
              wT += cC;
              vA += cC*srcC[jN][kN];
            }
          }

          ff  = *XpD;
          kN  =  kA - IRINTF(ff);
          if (sX <= kN && kN < sx1) {
            ff  = *YpD;
            jN  =  jA - IRINTF(ff);
            if (sY <= jN && jN < sy1) {
              wT += cD;
              vA += cD*srcD[jN][kN];
            }
          }

          dstW[jT][kT] += wT;
          dstT[jT][kT] += vA;
        }
      }
    }
  } 
}





/*_____________ The rest is here basically for the sake of comparison _______________*/


/*** rowSmatCls:  dst = src instead of dst += src, i.e., collisions */

void rowSmatCls ( ppf dstT, ppf dstW, ppf srcA, ppf fwdX, ppf fwdY
                , int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, dbl trs)
{
  int	dx1 = dX + dW;
  int	dy1 = dY + dH;
  int	sy1 = sY + sH;
  int	sx1 = sX + sW;
  int	jA, kA;
  pfl	pX, pY;
  rgst  flt ff, fac = (flt) trs;
  rgst	int kT, jT;

  rowZeroF(dstT,dX,dY,dW,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (jA = sY, fwdX += jA, fwdY += jA; jA < sy1; jA++) {
    for (kA = sX, pX = *fwdX++ + kA, pY = *fwdY++ + kA; kA < sx1; pX++, kA++) {
      ff = fac * *pY++;
      jT = jA - IRINTF(ff);
      if (dY <= jT && jT < dy1) {
        ff = fac * *pX;
        kT = kA - IRINTF(ff);
        if (dX <= kT && kT < dx1) {
          dstT[jT][kT]  = srcA[jA][kA];	/* Single pixval: no clip or round */
          dstW[jT][kT]  = 1.0F;
        }
      }
    }
  } 
}



void rowSmatTrn ( ppf dstT, ppf dstW, ppf srcA, ppf fwdX, ppf fwdY
                , int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, dbl trs)
{
  int	dx1 = dX + dW;
  int	dy1 = dY + dH;
  int	sy1 = sY + sH;
  int	sx1 = sX + sW;
  int	jA, kA;
  pfl	pX, pY;
  flt	fac = (flt) trs;
  rgst	int kT, jT;

  rowZeroF(dstT,dX,dY,dW,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (jA = sY, fwdX += jA, fwdY += jA; jA < sy1; jA++) {
    for (kA = sX, pX = *fwdX++ + kA, pY = *fwdY++ + kA; kA < sx1; pX++, kA++) {
      jT = (int)(jA - fac * *pY++);
      if (dY <= jT && jT < dy1) {
        kT = (int)(kA - fac * *pX);      
        if (dX <= kT && kT < dx1) {
          dstT[jT][kT] += srcA[jA][kA];	/* Single pixval: no clip or round */
          dstW[jT][kT] += 1.0F;
        }
      }
    }
  } 
}





