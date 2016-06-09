/*****************************************************************************
 rowSpatBil.c    S.R.Lines     93.10.28
 Like rowSmatBil.c, except it's assumed the src's are prewarped (to srcA or
 srcF[0])
#include	<stdlib.h>
#include	<unMem.h>
#include	"rowMake.h"
#include	<unBug.h>
 *****************************************************************************/

#define 	 NDEBUG 1
#include	<assert.h>
#include	"un/unBug.h"
#include	"un/unMath.h"
#include	"un/unTypes.h"
#include	"rowZero.h"
#include	"rowSpat.h"

void rowSpatBil4(ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, flt clrs[4], dbl trs, dbl infimum)
{
    int 	dx1 = dX + dW - 1;
    int 	dy1 = dY + dH - 1;
    int 	sy1 = sY + sH - 1;
    int 	sx1 = sX + sW - 1;
    ppf 	XvA = fvX[0] + sY, YvA = fvY[0] + sY;
    ppf	srcA = srcS[0], srcB = srcS[1];
    ppf 	srcC = srcS[2], srcD = srcS[3];
    pfl 	XpA, YpA;
    int 	jA, kA, jT, kT; 	/* scanM = srcB[1] - srcB[0] - 1; */
    flt 	cA = clrs[0], cB = clrs[1], cC = clrs[2], cD = clrs[3];
    flt 	fac = (flt)trs; 	/* inf = (flt) infimum; */
    flt 	yM, xM, vT, xT, yT;
    rgs flt	v1, v2;

    rowZeroF(dstT, dX, dY, dW, dH);
    rowZeroF(dstW, dX, dY, dW, dH);

    for (jA = sY; jA <= sy1; jA++) {

        kA = sX;
        XpA = *XvA++ + kA, YpA = *YvA++ + kA;

        for (; kA <= sx1; XpA++, kA++) {

            yT = jA - fac * *YpA++;		/* NB: expect trs == 1.0 */
            jT = IFLOOR(yT);
            if (dY <= jT && jT < dy1) {	/* Target y value is within dst area */
                xT = kA - fac * *XpA;
                kT = IFLOOR(xT);
                if (dX <= kT && kT < dx1) {	/* Target x value is also within dst area */

                    vT = cA*srcA[jA][kA] + cB*srcB[jA][kA] + cC*srcC[jA][kA] + cD*srcD[jA][kA];

                    if (1) {

                        xT -= kT;     /* IE: xT = (kA-fac**XpA) - floor(kA-fac**XpA); */
                        xM = 1.0F - xT;
                        yT -= jT;
                        yM = 1.0F - yT;

#if 0       /* Now (vT/wT) would yield weighted mean of interpolated values,
               but this is postponed til after distributing it over a 2x2 pixel block: */

                        v1 = xM * yM;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * vT;

                        kT++;
                        v1 = xT * yM;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * vT;

                        jT++;
                        v1 = xT * yT;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * vT;

                        kT--;
                        v1 = xM * yT;
                        dstW[jT][kT] += v1;
                        dstT[jT][kT] += v1 * vT;

#else       /* Old way: compute weighted mean of interpolated values (vT/wT) now: */

                        v2 = vT;

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
/*___________________________________________________________________________*
 Should be same as rowSmatBil, just macroize.
 *___________________________________________________________________________*/

void rowSpatBil(ppf dstT, ppf dstW, ppf srcA, ppf fwdX, ppf fwdY
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



void rowSpatBil2(ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf fwdX, ppf fwdY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr)
{
    int dx1 = dX + dW - 1;
    int dy1 = dY + dH - 1;
    int sx1 = sX + sW - 1;
    int sy1 = sY + sH - 1;
    int	jA, kA;
    pfl	pX, pY;
    flt	xT, yT, xM, yM;
    dbl cB = clr, cA = 1.0 - cB;
    rgst int	  kT, jT;
    rgst flt	  wT, vT;

    rowZeroF(dstT, dX, dY, dW, dH);
    rowZeroF(dstW, dX, dY, dW, dH);

    for (jA = sY, fwdX += jA, fwdY += jA; jA <= sy1; jA++) {
        for (kA = sX, pX = *fwdX++ + kA, pY = *fwdY++ + kA; kA <= sx1; pX++, kA++) {

            yT = (flt)(jA - trs * *pY++);
            jT = IFLOOR(yT);

            if (dY <= jT && jT < dy1) {

                xM = *pX;
                xT = (flt)(kA - trs * xM);
                kT = IFLOOR(xT);

                if (dX <= kT && kT < dx1) {

                    vT = (flt)(srcA[jA][kA] * cA + srcB[jA][kA] * cB);

                    xT -= kT;
                    xM = 1.0F - xT;
                    yT -= jT;
                    yM = 1.0F - yT;

                    wT = xM * yM;
                    dstW[jT][kT] += wT;
                    dstT[jT][kT] += wT * vT;

                    kT++;
                    wT = xT * yM;
                    dstW[jT][kT] += wT;
                    dstT[jT][kT] += wT * vT;

                    jT++;
                    wT = xT * yT;
                    dstW[jT][kT] += wT;
                    dstT[jT][kT] += wT * vT;

                    kT--;
                    wT = xM * yT;
                    dstW[jT][kT] += wT;
                    dstT[jT][kT] += wT * vT;
                }
            }
        }
    }
}



void rowSpatBil3(ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
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
