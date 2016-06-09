/*****************************************************************************
 rowSmatBil.c    S.R.Lines     93.5.15-9.21
 NOTES: if trs < TEENY, just rowCopy srcA to dstT?
 Must have dims(dstT) <= dims(fwdX) == dims(fwdY), and borders of srcs may be
 positive or negative (dims(srcA) <> dims(dstT)), but they must be known
 explicitly.  In general, dims(dstT) < dms(srcA) for best results.
 All all must be aligned (i.e., [0][0] must refer to same "place")
 --this makes the algorithm and subimage processing simple.

 Destination Boundaries:
 dY is the top, or coord of the first row, dWd is the rectangle's width, etc.
 Source Boundaries:
 sX is the coord of leftmost stored pxl, and sx1 is the rightmost pxl from
 which to anchor an interpolation, etc. -- for bilinear interpolation,
 think of sX as 0 and sx1 as wide-2.

 Assumes dims(dstT) == dims(dstW) == dims(invX)
 and     dims(srcA) == dims(fwdX) == dims(fwdY)

 NB: srcA & srcB are treated asymmetrically in at least one way:
 1) flow is assumed to be relative to A's coord. frame.

#include	<stdlib.h>
#include	<unMem.h>
#include	"rowMake.h"
#include	<unBug.h>
 *****************************************************************************/
#define 	 NDEBUG 1
#include	<assert.h>
#include	<un/unBug.h>
#include	<un/unMath.h>
#include	<un/unTypes.h>
#include	<m/rowZero.h>
#include	<m/mj.h>
#include	<m/rowSmat.h>

 /*___________________________________________________________________________*
  *___________________________________________________________________________*/

void rowSmatBil(ppf dstT, ppf dstW, ppf srcA, ppf fwdX, ppf fwdY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs)
{
    int	dx1 = dX + dW - 1;
    int	dy1 = dY + dH - 1;
    int	sx1 = sX + sW;
    int	sy1 = sY + sH;
    int		jA, kA;
    pfl		pX, pY;
    flt		xT, yT, vA;
    rgst	int	kT, jT;
    rgst	flt	wT;

    rowZeroF(dstT, dX, dY, dW, dH);
    rowZeroF(dstW, dX, dY, dW, dH);

    for (jA = sY, fwdX += jA, fwdY += jA; jA < sy1; jA++) {
        for (kA = sX, pX = *fwdX++ + kA, pY = *fwdY++ + kA; kA < sx1; pX++, kA++) {

            yT = (flt)(jA - trs * *pY++);
            jT = IFLOOR(yT);

            if (dY <= jT && jT < dy1) {

                xT = (flt)(kA - trs * *pX);
                kT = IFLOOR(xT);

                if (dX <= kT && kT < dx1) {

                    vA = srcA[jA][kA];
                    xT -= kT;
                    yT -= jT;

                    wT = (1.0F - xT)*(1.0F - yT);
                    dstW[jT][kT] += wT;
                    dstT[jT][kT] += wT*vA;

                    kT++;
                    wT = (xT)*(1.0F - yT);
                    dstW[jT][kT] += wT;
                    dstT[jT][kT] += wT*vA;

                    jT++;
                    wT = (xT)*(yT);
                    dstW[jT][kT] += wT;
                    dstT[jT][kT] += wT*vA;

                    kT--;
                    wT = (1.0F - xT)*(yT);
                    dstW[jT][kT] += wT;
                    dstT[jT][kT] += wT*vA;
                }
            }
        }
    }
}



void rowSmatBil2(ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf fwdX, ppf fwdY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr)
{
    int  dx1 = dX + dW - 1;
    int  dy1 = dY + dH - 1;
    int  sx1 = sX + sW - 1;
    int  sy1 = sY + sH - 1;
    int  jA, kA, kB, jB, scanB = srcB[1] - srcB[0] - 1;
    pfl  pX, pY, pB;
    flt  xT, yT, xB, yB;
    dbl  cB = clr, cA = 1.0F - cB;
    rgst int	  kT, jT;
    rgst flt	  wT, vA;

    rowZeroF(dstT, dX, dY, dW, dH);
    rowZeroF(dstW, dX, dY, dW, dH);

    for (jA = sY, fwdX += jA, fwdY += jA; jA <= sy1; jA++) {
        for (kA = sX, pX = *fwdX++ + kA, pY = *fwdY++ + kA; kA <= sx1; pX++, kA++) {

            yB = *pY++;
            yT = (flt)(jA - trs * yB);
            jT = IFLOOR(yT);

            if (dY <= jT && jT < dy1) {

                xB = *pX;
                xT = (flt)(kA - trs * xB);
                kT = IFLOOR(xT);

                if (dX <= kT && kT < dx1) {

                    yB = jA - yB;
                    jB = IFLOOR(yB);
                    xB = kA - xB;
                    kB = IFLOOR(xB);

                    /* NB: (kB,jB) is NOT always inbounds. */
                    if (sX <= kB && kB < sx1 && sY <= jB && jB < sy1) {

                        yB -= jB;
                        xB -= kB;

#if 1       /* Assuming all rows in srcB are equally separated: */
                        pB = srcB[jB] + kB;
                        wT = *pB++;
                        wT += (*pB - wT) * xB;
                        pB += scanB;
                        vA = *pB++;
                        vA += (*pB - vA) * xB;
                        wT += (vA - wT) * yB;
#else
                        wT = srcB[jB][kB];
                        wT += (srcB[jB][kB + 1] - wT) * xB;
                        vA = (srcB[++jB][kB];
                        vA += (srcB[jB][kB + 1] - vA) * xB;
                        wT += (vA - wT) * yB;
#endif
                        assert(-0.5 < wT && wT < 255.5);
                        vA = (flt)(wT*cB + cA*srcA[jA][kA]);
                    }
                    else
                        vA = srcA[jA][kA];

                    xT -= kT;
                    xB = 1.0F - xT;
                    yT -= jT;
                    yB = 1.0F - yT;

                    wT = xB * yB;
                    dstW[jT][kT] += wT;
                    dstT[jT][kT] += wT * vA;

                    kT++;
                    wT = xT * yB;
                    dstW[jT][kT] += wT;
                    dstT[jT][kT] += wT * vA;

                    jT++;
                    wT = xT * yT;
                    dstW[jT][kT] += wT;
                    dstT[jT][kT] += wT * vA;

                    kT--;
                    wT = xB * yT;
                    dstW[jT][kT] += wT;
                    dstT[jT][kT] += wT * vA;
                }
            }
        }
    }
}



void rowSmatBil3(ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, flt clrs[3], dbl trs, dbl infimum)
{
    int	dx1 = dX + dW - 1;
    int	dy1 = dY + dH - 1;
    int	sy1 = sY + sH - 1;
    int	sx1 = sX + sW - 1;
    ppf	XvA = fvX[0] + sY, YvA = fvY[0] + sY, srcA = srcS[0] + sY;
    ppf	XvB = fvX[1] + sY, YvB = fvY[1] + sY, srcB = srcS[1] + sY;
    ppf	XvC = fvX[2] + sY, YvC = fvY[2] + sY, srcC = srcS[2] + sY;
    pfl	XpA, YpA, XpB, YpB, XpC, YpC, pM;
    int	jA, kA, jT, kT, scanM = srcB[1] - srcB[0] - 1;
    flt	cA = clrs[0], cB = clrs[1], cC = clrs[2];
    flt	fac = (flt)trs, inf = (flt)infimum;
    flt	    yM, xM, vT, wT, xT, yT;
    rgst	int jM, kM;
    rgst  flt v1, v2;

    rowZeroF(dstT, dX, dY, dW, dH);
    rowZeroF(dstW, dX, dY, dW, dH);

    for (jA = sY; jA <= sy1; jA++) {

        kA = sX;
        XpA = *XvA++ + kA, YpA = *YvA++ + kA;
        XpB = *XvB++ + kA, YpB = *YvB++ + kA;
        XpC = *XvC++ + kA, YpC = *YvC++ + kA;

        for (; kA <= sx1; XpA++, XpB++, YpB++, XpC++, YpC++, kA++) {

            yT = jA - fac * *YpA++;		/* NB: expect trs == 1.0 */
            jT = IFLOOR(yT);
            if (dY <= jT && jT < dy1) {	/* Target y value is within dst area */
                xT = kA - fac * *XpA;
                kT = IFLOOR(xT);
                if (dX <= kT && kT < dx1) {	/* Target x value is also within dst area */

                    wT = cA;
                    vT = cA*srcA[jA][kA];
#if 1
                    EXTRACT_BIL(wT, vT, xM, yM, kM, jM, scanM, XpB, YpB, srcB, cB)
                        EXTRACT_BIL(wT, vT, xM, yM, kM, jM, scanM, XpC, YpC, srcC, cC)
#else
                    xM = kA - *XpB;		/* NB (@@): srcB(yM,xM) will be same every time! */
                    kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1) {
                        yM = jA - *YpB;
                        jM = IFLOOR(yM);
                        if (sY <= jM && jM < sy1) {	/*  srcB(jM,kM) is in bounds */
                            xM -= kM;
                            yM -= jM;
                            /* Assuming all rows in srcB, etc, are separated/spaced equally: */
                            pM = srcB[jM] + kM;
                            v1 = *pM++;
                            v1 += (*pM - v1) * xM;
                            pM += scanM;
                            v2 = *pM++;
                            v2 += (*pM - v2) * xM;
                            v1 += (v2 - v1) * yM;
                            wT += cB;
                            vT += cB*v1;
                        }
                    }
                    xM = kA - *XpC;
                    kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1) {
                        yM = jA - *YpC;
                        jM = IFLOOR(yM);
                        if (sY <= jM && jM < sy1) {	/*  srcC(jM,kM) is in bounds */
                            xM -= kM;
                            yM -= jM;
                            pM = srcC[jM] + kM;
                            v1 = *pM++;
                            v1 += (*pM - v1) * xM;
                            pM += scanM;
                            v2 = *pM++;
                            v2 += (*pM - v2) * xM;
                            v1 += (v2 - v1) * yM;
                            wT += cC;
                            vT += cC*v1;
                        }
                    }
#endif

                    if (wT > inf) {

                        xT -= kT;     /* IE: xT = (kA-fac**XpA) - floor(kA-fac**XpA); */
                        xM = 1.0F - xT;
                        yT -= jT;
                        yM = 1.0F - yT;

#if 0       /* Now (vT/wT) would yield weighted mean of interpolated values,
               but this is postponed til after distributing it over a 2x2 pixel block: */

                        v2 = wT; 	/* NB: expect wT to be 1.0 == sum(clrs) */

                        v1 = v2 * xM * yM;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * vT;

                        kT++;
                        v1 = v2 * xT * yM;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * vT;

                        jT++;
                        v1 = v2 * xT * yT;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * vT;

                        kT--;
                        v1 = v2 * xM * yT;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * vT;

#else       /* Old way: compute weighted mean of interpolated values (vT/wT) now: */

                        v2 = vT / wT; 	/* NB: expect wT to be 1.0 == sum(clrs) */

                        v1 = xM * yM;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * v2;

                        kT++;
                        v1 = xT * yM;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * v2;

                        jT++;
                        v1 = xT * yT;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * v2;

                        kT--;
                        v1 = xM * yT;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * v2;
#endif
                    }
                }
            }
        }
    }
}






void rowSmatBil4(ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, flt clrs[4], dbl trs, dbl infimum)
{
    int 	dx1 = dX + dW - 1;
    int 	dy1 = dY + dH - 1;
    int 	sy1 = sY + sH - 1;
    int 	sx1 = sX + sW - 1;
    ppf 	XvA = fvX[0] + sY, YvA = fvY[0] + sY, srcA = srcS[0] + sY;
    ppf 	XvB = fvX[1] + sY, YvB = fvY[1] + sY, srcB = srcS[1] + sY;
    ppf 	XvC = fvX[2] + sY, YvC = fvY[2] + sY, srcC = srcS[2] + sY;
    ppf 	XvD = fvX[3] + sY, YvD = fvY[3] + sY, srcD = srcS[3] + sY;
    pfl 	XpA, YpA, XpB, YpB, XpC, YpC, XpD, YpD, pM;
    int 	jA, kA, jT, kT, scanM = srcB[1] - srcB[0] - 1;
    flt 	cA = clrs[0], cB = clrs[1], cC = clrs[2], cD = clrs[3];
    flt 	fac = (flt)trs, inf = (flt)infimum;
    flt 	yM, xM, vT, wT, xT, yT;
    rgs int	jM, kM;
    rgs flt	v1, v2;

    rowZeroF(dstT, dX, dY, dW, dH);
    rowZeroF(dstW, dX, dY, dW, dH);

    for (jA = sY; jA <= sy1; jA++) {

        kM = kA = sX;
        XpA = *XvA++ + kM, YpA = *YvA++ + kM;
        XpB = *XvB++ + kM, YpB = *YvB++ + kM;
        XpC = *XvC++ + kM, YpC = *YvC++ + kM;
        XpD = *XvD++ + kM, YpD = *YvD++ + kM;

        for (; kA <= sx1; XpA++, XpB++, YpB++, XpC++, YpC++, XpD++, YpD++, kA++) {

            yT = jA - fac * *YpA++;		/* NB: expect trs == 1.0 */
            jT = IFLOOR(yT);
            if (dY <= jT && jT < dy1) {	/* Target y value is within dst area */
                xT = kA - fac * *XpA;
                kT = IFLOOR(xT);
                if (dX <= kT && kT < dx1) {	/* Target x value is also within dst area */

                    wT = cA;
                    vT = wT*srcA[jA][kA];

                    EXTRACT_BIL(wT, vT, xM, yM, kM, jM, scanM, XpB, YpB, srcB, cB)
                        EXTRACT_BIL(wT, vT, xM, yM, kM, jM, scanM, XpC, YpC, srcC, cC)
                        EXTRACT_BIL(wT, vT, xM, yM, kM, jM, scanM, XpD, YpD, srcD, cD)

                        if (wT > inf) {

                            xT -= kT;     /* IE: xT = (kA-fac**XpA) - floor(kA-fac**XpA); */
                            xM = 1.0F - xT;
                            yT -= jT;
                            yM = 1.0F - yT;

#if 0       /* Now (vT/wT) would yield weighted mean of interpolated values,
               but this is postponed til after distributing it over a 2x2 pixel block: */
                            v2 = wT; 	/* NB: expect wT to be 1.0 == sum(clrs) */

                            v1 = v2 * xM * yM;
                            dstW[jT][kT] += v1;
                            dstT[jT][kT] += v1 * vT;

                            kT++;
                            v1 = v2 * xT * yM;
                            dstW[jT][kT] += v1;
                            dstT[jT][kT] += v1 * vT;

                            jT++;
                            v1 = v2 * xT * yT;
                            dstW[jT][kT] += v1;
                            dstT[jT][kT] += v1 * vT;

                            kT--;
                            v1 = v2 * xM * yT;
                            dstW[jT][kT] += v1;
                            dstT[jT][kT] += v1 * vT;

#else       /* Old way: compute weighted mean of interpolated values (vT/wT) now: */

                            v2 = vT / wT; 	/* NB: expect wT to be 1.0 == sum(clrs) */

                            v1 = xM * yM;
                            dstW[jT][kT] += v1;
                            dstT[jT][kT] += v1 * v2;

                            kT++;
                            v1 = xT * yM;
                            dstW[jT][kT] += v1;
                            dstT[jT][kT] += v1 * v2;

                            jT++;
                            v1 = xT * yT;
                            dstW[jT][kT] += v1;
                            dstT[jT][kT] += v1 * v2;

                            kT--;
                            v1 = xM * yT;
                            dstW[jT][kT] += v1;
                            dstT[jT][kT] += v1 * v2;
#endif
                        }
                }
            }
        }
    }
}




#if 0000
#if 1
void rowSmatBil3(ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, flt clrs[3], dbl trs, dbl infimum)
{
    int	dx1 = dX + dW - 1;
    int	dy1 = dY + dH - 1;
    int	sy1 = sY + sH - 1;
    int	sx1 = sX + sW - 1;
    ppf	XvA = fvX[0] + sY, YvA = fvY[0] + sY, srcA = srcS[0] + sY;
    ppf	XvB = fvX[1] + sY, YvB = fvY[1] + sY, srcB = srcS[1] + sY;
    ppf	XvC = fvX[2] + sY, YvC = fvY[2] + sY, srcC = srcS[2] + sY;
    pfl	XpA, YpA, XpB, YpB, XpC, YpC, pM;
    int	jA, kA, jT, kT, scanM = srcB[1] - srcB[0] - 1;
    flt	cA = clrs[0], cB = clrs[1], cC = clrs[2];
    flt	fac = (flt)trs, inf = (flt)infimum;
    flt	    yM, xM, vT, wT, xT, yT;
    rgst	int jM, kM;
    rgst  flt v1, v2;

    memset((pvd)(dstW[dY] + dX), 0, sizeof(flt)*dW*dH);
    memset((pvd)(dstT[dY] + dX), 0, sizeof(flt)*dW*dH);

    for (jA = sY; jA <= sy1; jA++) {

        kA = sX;
        XpA = *XvA++ + kA, YpA = *YvA++ + kA;
        XpB = *XvB++ + kA, YpB = *YvB++ + kA;
        XpC = *XvC++ + kA, YpC = *YvC++ + kA;

        for (; kA <= sx1; XpA++, XpB++, YpB++, XpC++, YpC++, kA++) {

            yT = jA - fac * *YpA++;		/* NB: expect trs == 1.0 */
            jT = IFLOOR(yT);
            if (dY <= jT && jT < dy1) {	/* Target y value is within dst area */
                xT = kA - fac * *XpA;
                kT = IFLOOR(xT);
                if (dX <= kT && kT < dx1) {	/* Target x value is also within dst area */

                    wT = cA;
                    vT = cA*srcA[jA][kA];

                    xM = kA - *XpB;		/* NB (@@): srcB(yM,xM) will be same every time! */
                    kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1) {
                        yM = jA - *YpB;
                        jM = IFLOOR(yM);
                        if (sY <= jM && jM < sy1) {	/*  srcB(jM,kM) is in bounds */
                            xM -= kM;
                            yM -= jM;
                            /* Assuming all rows in srcB, etc, are separated/spaced equally: */
                            pM = srcB[jM] + kM;
                            v1 = *pM++;
                            v1 += (*pM - v1) * xM;
                            pM += scanM;
                            v2 = *pM++;
                            v2 += (*pM - v2) * xM;
                            v1 += (v2 - v1) * yM;
                            wT += cB;
                            vT += cB*v1;
                        }
                    }

                    xM = kA - *XpC;
                    kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1) {
                        yM = jA - *YpC;
                        jM = IFLOOR(yM);
                        if (sY <= jM && jM < sy1) {	/*  srcC(jM,kM) is in bounds */
                            xM -= kM;
                            yM -= jM;
                            pM = srcC[jM] + kM;
                            v1 = *pM++;
                            v1 += (*pM - v1) * xM;
                            pM += scanM;
                            v2 = *pM++;
                            v2 += (*pM - v2) * xM;
                            v1 += (v2 - v1) * yM;
                            wT += cC;
                            vT += cC*v1;
                        }
                    }


                    if (wT > inf) {

#if 0       /* Now (vT/wT) would yield weighted mean of interpolated values,
               but this is postponed til after distributing it over a 2x2 pixel block: */

                        xT -= kT;
                        xM = 1.0F - xT;
                        yT -= jT;
                        yM = 1.0F - yT;

                        v2 = wT; 	/* NB: expect wT to be 1.0 == sum(clrs) */

                        v1 = v2 * xM * yM;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * vT;

                        kT++;
                        v1 = v2 * xT * yM;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * vT;

                        jT++;
                        v1 = v2 * xT * yT;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * vT;

                        kT--;
                        v1 = v2 * xM * yT;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * vT;

#else       /* Old way: compute weighted mean of interpolated values (vT/wT) now: */

                        xT -= kT;
                        xM = 1.0F - xT;
                        yT -= jT;
                        yM = 1.0F - yT;

                        v2 = vT / wT; 	/* NB: expect wT to be 1.0 == sum(clrs) */

                        v1 = xM * yM;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * v2;

                        kT++;
                        v1 = xT * yM;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * v2;

                        jT++;
                        v1 = xT * yT;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * v2;

                        kT--;
                        v1 = xM * yT;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * v2;
#endif
                    }
                }
            }
        }
    }
}
#endif




void rowSmatBil4(ppf dsT, ppf dwT, ppf *srcS, ppf *fvX, ppf *fvY
    , int dX, int dY, int dxL, int dyL
    , int sX, int sY, int sxL, int syL, flt clrs[4], dbl trs, dbl infimum)
{
    int	dx1 = dX + dxL - 1;
    int	dy1 = dY + dyL - 1;
    int	sy1 = sY + syL - 1;
    int	sx1 = sX + sxL - 1;
    ppf	XvA = fvX[0], YvA = fvY[0], srcA = srcS[0] + sY;
    ppf	XvB = fvX[1], YvB = fvY[1], srcB = srcS[1] + sY;
    ppf	XvC = fvX[2], YvC = fvY[2], srcC = srcS[2] + sY;
    ppf	XvD = fvX[3], YvD = fvY[3], srcD = srcS[3] + sY;
    pfl	XpA, YpA, XpB, YpB, XpC, YpC, XpD, YpD, pM;
    int	jA, kA, jT, kT, scanM = srcB[1] - srcB[0] - 1;
    flt	cA = clrs[0], cB = clrs[1], cC = clrs[2], cD = clrs[3];
    flt	fac = (flt)trs, inf = (flt)infimum;
    flt	    yM, xM, vT, wT, xT, yT;
    rgst	int jM, kM;
    rgst  flt v1, v2;

    memset((pvd)(dwT[dY] + dX), 0, sizeof(flt)*dxL*dyL);
    memset((pvd)(dsT[dY] + dX), 0, sizeof(flt)*dxL*dyL);

    for (jA = sY; jA <= sy1; jA++) {

        kA = sX;
        XpA = *XvA++ + kA, YpA = *YvA++ + kA;
        XpB = *XvB++ + kA, YpB = *YvB++ + kA;
        XpC = *XvC++ + kA, YpC = *YvC++ + kA;
        XpD = *XvD++ + kA, YpD = *YvD++ + kA;

        for (; kA <= sx1; XpA++, XpB++, YpB++, XpC++, YpC++, XpD++, YpD++, kA++) {

            yT = jA - fac * *YpA++;		/* NB: expect trs == 1.0 */
            jT = IFLOOR(yT);
            if (dY <= jT && jT < dy1) {	/* Target y value is within dst area */
                xT = kA - fac * *XpA;
                kT = IFLOOR(xT);
                if (dX <= kT && kT < dx1) {	/* Target x value is also within dst area */

                    wT = cA;
                    vT = cA*srcA[jA][kA];

                    xM = kA - *XpB;		/* NB (@@): srcB(yM,xM) will be same every time! */
                    kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1) {
                        yM = jA - *YpB;
                        jM = IFLOOR(yM);
                        if (sY <= jM && jM < sy1) {	/*  srcB(jM,kM) is in bounds */
                            xM -= kM;
                            yM -= jM;
                            /* Assuming all rows in srcB, etc, are separated/spaced equally: */
                            pM = srcB[jM] + kM;
                            v1 = *pM++;
                            v1 += (*pM - v1) * xM;
                            pM += scanM;
                            v2 = *pM++;
                            v2 += (*pM - v2) * xM;
                            v1 += (v2 - v1) * yM;
                            wT += cB;
                            vT += cB*v1;
                        }
                    }

                    xM = kA - *XpC;
                    kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1) {
                        yM = jA - *YpC;
                        jM = IFLOOR(yM);
                        if (sY <= jM && jM < sy1) {	/*  srcC(jM,kM) is in bounds */
                            xM -= kM;
                            yM -= jM;
                            pM = srcC[jM] + kM;
                            v1 = *pM++;
                            v1 += (*pM - v1) * xM;
                            pM += scanM;
                            v2 = *pM++;
                            v2 += (*pM - v2) * xM;
                            v1 += (v2 - v1) * yM;
                            wT += cC;
                            vT += cC*v1;
                        }
                    }

                    xM = kA - *XpD;
                    kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1) {
                        yM = jA - *YpD;
                        jM = IFLOOR(yM);
                        if (sY <= jM && jM < sy1) {	/*  srcD(jM,kM) is in bounds */
                            xM -= kM;
                            yM -= jM;
                            pM = srcD[jM] + kM;
                            v1 = *pM++;
                            v1 += (*pM - v1) * xM;
                            pM += scanM;
                            v2 = *pM++;
                            v2 += (*pM - v2) * xM;
                            v1 += (v2 - v1) * yM;
                            wT += cD;
                            vT += cD*v1;
                        }
                    }

                    if (wT > inf) {

#if 0       /* Now (vT/wT) would yield weighted mean of interpolated values,
               but this is postponed til after distributing it over a 2x2 pixel block: */

                        xT -= kT;
                        xM = 1.0F - xT;
                        yT -= jT;
                        yM = 1.0F - yT;

                        v2 = wT; 	/* NB: expect wT to be 1.0 == sum(clrs) */

                        v1 = v2 * xM * yM;
                        dwT[jT][kT] += v1;
                        dsT[jT][kT] += v1 * vT;

                        kT++;
                        v1 = v2 * xT * yM;
                        dwT[jT][kT] += v1;
                        dsT[jT][kT] += v1 * vT;

                        jT++;
                        v1 = v2 * xT * yT;
                        dwT[jT][kT] += v1;
                        dsT[jT][kT] += v1 * vT;

                        kT--;
                        v1 = v2 * xM * yT;
                        dwT[jT][kT] += v1;
                        dsT[jT][kT] += v1 * vT;

#else       /* Old way: compute weighted mean of interpolated values (vT/wT) now: */

                        xT -= kT;
                        xM = 1.0F - xT;
                        yT -= jT;
                        yM = 1.0F - yT;

                        v2 = vT / wT; 	/* NB: expect wT to be 1.0 == sum(clrs) */

                        v1 = xM * yM;
                        dwT[jT][kT] += v1;
                        dsT[jT][kT] += v1 * v2;

                        kT++;
                        v1 = xT * yM;
                        dwT[jT][kT] += v1;
                        dsT[jT][kT] += v1 * v2;

                        jT++;
                        v1 = xT * yT;
                        dwT[jT][kT] += v1;
                        dsT[jT][kT] += v1 * v2;

                        kT--;
                        v1 = xM * yT;
                        dwT[jT][kT] += v1;
                        dsT[jT][kT] += v1 * v2;
#endif
                    }
                }
            }
        }
    }
}

#endif
