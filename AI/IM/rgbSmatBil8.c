/*****************************************************************************
 rgbSmatBil8.c    S.R.Lines     93.5.15-9.16
 using the total weight method
 *****************************************************************************/
#define          NDEBUG 1
#include        <assert.h>
#ifdef          __GNUC__
#include        <memory.h>      /* memset */
#else
#include        <string.h>      /* memset */
#endif
#include        <math.h>
#include        <un/unBug.h>
#include        <un/unMath.h>
#include        <un/unTypes.h>
#include        <un/unFlags.h>
#include        <m/rowZero.h>
#include        <m/mj.h>
#include        <m/rowSmat.h>
#define SRC_A   1
#define SRC_B   2
#define SRC_C   4
#define SRC_D   8


void
rgbSmatBil8(ppf dsT, ppf dwT, ppf sRGB[], ppf vOX[], ppf vOY[]
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, flt cfrac[], dbl trs)
{
    rflt	wR, wG, wB;
    int	dx1 = dX + dW - 1;
    int	dy1 = dY + dH - 1;
    int	sx1 = sX + sW - 1;
    int	sy1 = sY + sH - 1;
    ppf	srcA = sRGB[0], srcB = sRGB[1], srcC = sRGB[2], srcD = sRGB[3];
    ppf	srcE = sRGB[4], srcF = sRGB[5], srcG = sRGB[6], srcH = sRGB[7];
    ppf     vOTX = vOX[0] + sY, vOBX = vOX[1] + sY, vOCX = vOX[2] + sY, vODX = vOX[3] + sY;
    ppf     vOTY = vOY[0] + sY, vOBY = vOY[1] + sY, vOCY = vOY[2] + sY, vODY = vOY[3] + sY;
    ppf     vOEX = vOX[4] + sY, vOFX = vOX[5] + sY, vOGX = vOX[6] + sY, vOHX = vOX[7] + sY;
    ppf     vOEY = vOY[4] + sY, vOFY = vOY[5] + sY, vOGY = vOY[6] + sY, vOHY = vOY[7] + sY;
    pfl	pOTX, pOTY, pOBX, pOBY, pOCX, pOCY, pODX, pODY, pOEX, pOEY, pOFX, pOFY, pOGX, pOGY, pOHX, pOHY;
    int	jT, kT, kR, kG, kB, jO, kO, kM, jM, scanM = srcB[1] - srcB[0] - 2;
    flt	redT, grnT, bluT, redA, grnA, bluA, redB, grnB, bluB, redC, grnC, bluC, redD, grnD, bluD;
    flt	redE, grnE, bluE, redF, grnF, bluF, redG, grnG, bluG, redH, grnH, bluH;
    flt	*dsP, *pM, *psA, xT, yT, xM, yM;
    flt 	cA = cfrac[0], cB = cfrac[1], cC = cfrac[2], cD = cfrac[3];
    flt 	cE = cfrac[4], cF = cfrac[5], cG = cfrac[6], cH = cfrac[7];
    flt 	totW;

    memset((pvd)(dsT[dY] + 3 * dX), 0, sizeof(flt)*dW*dH * 3);
    memset((pvd)(dwT[dY] + dX), 0, sizeof(flt)*dW*dH);

    /*
      wrn("rgbSmatBil8 -- here I is!");
    */

    for (jO = sY; jO <= sy1; jO++) {

        pOTX = *vOTX++ + sX, pOTY = *vOTY++ + sX;
        pOBX = *vOBX++ + sX, pOBY = *vOBY++ + sX;
        pOCX = *vOCX++ + sX, pOCY = *vOCY++ + sX;
        pODX = *vODX++ + sX, pODY = *vODY++ + sX;
        pOEX = *vOEX++ + sX, pOEY = *vOEY++ + sX;
        pOFX = *vOFX++ + sX, pOFY = *vOFY++ + sX;
        pOGX = *vOGX++ + sX, pOGY = *vOGY++ + sX;
        pOHX = *vOHX++ + sX, pOHY = *vOHY++ + sX;

        for (kO = sX; kO <= sx1; pOTX++, pOTY++, pOBX++, pOBY++, pOCX++, pOCY++, pODX++, pODY++
            , pOEX++, pOEY++, pOFX++, pOFY++, pOGX++, pOGY++, pOHX++, pOHY++, kO++) {

            yT = (flt)(jO - trs * *pOTY);
            jT = IFLOOR(yT);

            if (dY <= jT && jT < dy1) {

                xT = (flt)(kO - trs * *pOTX);
                kT = IFLOOR(xT);

                if (dX <= kT && kT < dx1) {

                    totW = 1.0;

                    yM = jO - *pOBY;  jM = IFLOOR(yM);
                    xM = kO - *pOBX;  kM = IFLOOR(xM);
                    kR = kO * 3;
                    psA = srcA[jO];	/* @@ efficiency -- move !! */
                    redA = psA[kR++];
                    grnA = psA[kR++];
                    bluA = psA[kR];    /* kR will be reset */
                    if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {
                        yM -= jM;
                        xM -= kM;
                        /* Assuming all rows in srcB are equally separated: */
                        pM = srcB[jM] + kM * 3;
                        wR = *pM++;
                        wG = *pM++;
                        wB = *pM++;
                        wR += (*pM++ - wR) * xM;
                        wG += (*pM++ - wG) * xM;
                        wB += (*pM - wB) * xM;
                        pM += scanM;
                        redT = *pM++;
                        grnT = *pM++;
                        bluT = *pM++;
                        redT += (*pM++ - redT) * xM;
                        grnT += (*pM++ - grnT) * xM;
                        bluT += (*pM - bluT) * xM;
                        wR += (redT - wR) * yM;
                        wG += (grnT - wG) * yM;
                        wB += (bluT - wB) * yM;
                        assert(-0.5 < wR && wR < 255.5);	/* Normally #define NDEBUG */
                        assert(-0.5 < wG && wG < 255.5);
                        assert(-0.5 < wB && wB < 255.5);
                        redB = wR;
                        grnB = wG;
                        bluB = wB;
                    }
                    else {
                        totW -= cB;
                        redB = 0.0;
                        grnB = 0.0;
                        bluB = 0.0;
                    }

                    yM = jO - *pOCY;  jM = IFLOOR(yM);
                    xM = kO - *pOCX;  kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {
                        yM -= jM;
                        xM -= kM;
                        /* Assuming all rows in srcB are equally separated: */
                        pM = srcC[jM] + kM * 3;
                        wR = *pM++;
                        wG = *pM++;
                        wB = *pM++;
                        wR += (*pM++ - wR) * xM;
                        wG += (*pM++ - wG) * xM;
                        wB += (*pM - wB) * xM;
                        pM += scanM;
                        redT = *pM++;
                        grnT = *pM++;
                        bluT = *pM++;
                        redT += (*pM++ - redT) * xM;
                        grnT += (*pM++ - grnT) * xM;
                        bluT += (*pM - bluT) * xM;
                        wR += (redT - wR) * yM;
                        wG += (grnT - wG) * yM;
                        wB += (bluT - wB) * yM;
                        assert(-0.5 < wR && wR < 255.5);    /* Normally #define NDEBUG */
                        assert(-0.5 < wG && wG < 255.5);
                        assert(-0.5 < wB && wB < 255.5);
                        redC = wR;
                        grnC = wG;
                        bluC = wB;
                    }
                    else {
                        totW -= cC;
                        redC = 0.0;
                        grnC = 0.0;
                        bluC = 0.0;
                    }

                    yM = jO - *pODY;  jM = IFLOOR(yM);
                    xM = kO - *pODX;  kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {
                        yM -= jM;
                        xM -= kM;
                        /* Assuming all rows in srcB are equally separated: */
                        pM = srcD[jM] + kM * 3;
                        wR = *pM++;
                        wG = *pM++;
                        wB = *pM++;
                        wR += (*pM++ - wR) * xM;
                        wG += (*pM++ - wG) * xM;
                        wB += (*pM - wB) * xM;
                        pM += scanM;
                        redT = *pM++;
                        grnT = *pM++;
                        bluT = *pM++;
                        redT += (*pM++ - redT) * xM;
                        grnT += (*pM++ - grnT) * xM;
                        bluT += (*pM - bluT) * xM;
                        wR += (redT - wR) * yM;
                        wG += (grnT - wG) * yM;
                        wB += (bluT - wB) * yM;
                        assert(-0.5 < wR && wR < 255.5);    /* Normally #define NDEBUG */
                        assert(-0.5 < wG && wG < 255.5);
                        assert(-0.5 < wB && wB < 255.5);
                        redD = wR;
                        grnD = wG;
                        bluD = wB;
                    }
                    else {
                        totW -= cD;
                        redD = 0.0;
                        grnD = 0.0;
                        bluD = 0.0;
                    }

                    yM = jO - *pOEY;  jM = IFLOOR(yM);
                    xM = kO - *pOEX;  kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {
                        yM -= jM;
                        xM -= kM;
                        /* Assuming all rows in srcB are equally separated: */
                        pM = srcE[jM] + kM * 3;
                        wR = *pM++;
                        wG = *pM++;
                        wB = *pM++;
                        wR += (*pM++ - wR) * xM;
                        wG += (*pM++ - wG) * xM;
                        wB += (*pM - wB) * xM;
                        pM += scanM;
                        redT = *pM++;
                        grnT = *pM++;
                        bluT = *pM++;
                        redT += (*pM++ - redT) * xM;
                        grnT += (*pM++ - grnT) * xM;
                        bluT += (*pM - bluT) * xM;
                        wR += (redT - wR) * yM;
                        wG += (grnT - wG) * yM;
                        wB += (bluT - wB) * yM;
                        assert(-0.5 < wR && wR < 255.5);	/* Normally #define NDEBUG */
                        assert(-0.5 < wG && wG < 255.5);
                        assert(-0.5 < wB && wB < 255.5);
                        redE = wR;
                        grnE = wG;
                        bluE = wB;
                    }
                    else {
                        totW -= cE;
                        redE = 0.0;
                        grnE = 0.0;
                        bluE = 0.0;
                    }

                    yM = jO - *pOFY;  jM = IFLOOR(yM);
                    xM = kO - *pOFX;  kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {
                        yM -= jM;
                        xM -= kM;
                        /* Assuming all rows in srcB are equally separated: */
                        pM = srcF[jM] + kM * 3;
                        wR = *pM++;
                        wG = *pM++;
                        wB = *pM++;
                        wR += (*pM++ - wR) * xM;
                        wG += (*pM++ - wG) * xM;
                        wB += (*pM - wB) * xM;
                        pM += scanM;
                        redT = *pM++;
                        grnT = *pM++;
                        bluT = *pM++;
                        redT += (*pM++ - redT) * xM;
                        grnT += (*pM++ - grnT) * xM;
                        bluT += (*pM - bluT) * xM;
                        wR += (redT - wR) * yM;
                        wG += (grnT - wG) * yM;
                        wB += (bluT - wB) * yM;
                        assert(-0.5 < wR && wR < 255.5);    /* Normally #define NDEBUG */
                        assert(-0.5 < wG && wG < 255.5);
                        assert(-0.5 < wB && wB < 255.5);
                        redF = wR;
                        grnF = wG;
                        bluF = wB;
                    }
                    else {
                        totW -= cF;
                        redF = 0.0;
                        grnF = 0.0;
                        bluF = 0.0;
                    }

                    yM = jO - *pOGY;  jM = IFLOOR(yM);
                    xM = kO - *pOGX;  kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {
                        yM -= jM;
                        xM -= kM;
                        /* Assuming all rows in srcB are equally separated: */
                        pM = srcG[jM] + kM * 3;
                        wR = *pM++;
                        wG = *pM++;
                        wB = *pM++;
                        wR += (*pM++ - wR) * xM;
                        wG += (*pM++ - wG) * xM;
                        wB += (*pM - wB) * xM;
                        pM += scanM;
                        redT = *pM++;
                        grnT = *pM++;
                        bluT = *pM++;
                        redT += (*pM++ - redT) * xM;
                        grnT += (*pM++ - grnT) * xM;
                        bluT += (*pM - bluT) * xM;
                        wR += (redT - wR) * yM;
                        wG += (grnT - wG) * yM;
                        wB += (bluT - wB) * yM;
                        assert(-0.5 < wR && wR < 255.5);    /* Normally #define NDEBUG */
                        assert(-0.5 < wG && wG < 255.5);
                        assert(-0.5 < wB && wB < 255.5);
                        redG = wR;
                        grnG = wG;
                        bluG = wB;
                    }
                    else {
                        totW -= cG;
                        redG = 0.0;
                        grnG = 0.0;
                        bluG = 0.0;
                    }

                    yM = jO - *pOHY;  jM = IFLOOR(yM);
                    xM = kO - *pOHX;  kM = IFLOOR(xM);
                    if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {
                        yM -= jM;
                        xM -= kM;
                        /* Assuming all rows in srcB are equally separated: */
                        pM = srcH[jM] + kM * 3;
                        wR = *pM++;
                        wG = *pM++;
                        wB = *pM++;
                        wR += (*pM++ - wR) * xM;
                        wG += (*pM++ - wG) * xM;
                        wB += (*pM - wB) * xM;
                        pM += scanM;
                        redT = *pM++;
                        grnT = *pM++;
                        bluT = *pM++;
                        redT += (*pM++ - redT) * xM;
                        grnT += (*pM++ - grnT) * xM;
                        bluT += (*pM - bluT) * xM;
                        wR += (redT - wR) * yM;
                        wG += (grnT - wG) * yM;
                        wB += (bluT - wB) * yM;
                        assert(-0.5 < wR && wR < 255.5);    /* Normally #define NDEBUG */
                        assert(-0.5 < wG && wG < 255.5);
                        assert(-0.5 < wB && wB < 255.5);
                        redH = wR;
                        grnH = wG;
                        bluH = wB;
                    }
                    else {
                        totW -= cH;
                        redH = 0.0;
                        grnH = 0.0;
                        bluH = 0.0;
                    }

                    redT = (cA*redA + cB*redB + cC*redC + cD*redD + cE*redE + cF*redF + cG*redG + cH*redH) / totW;
                    grnT = (cA*grnA + cB*grnB + cC*grnC + cD*grnD + cE*grnE + cF*grnF + cG*grnG + cH*grnH) / totW;
                    bluT = (cA*bluA + cB*bluB + cC*bluC + cD*bluD + cE*bluE + cF*bluF + cG*bluG + cH*bluH) / totW;

                    xT -= kT;
                    xM = 1.0F - xT;
                    yT -= jT;
                    yM = 1.0F - yT;

                    kR = kT * 3;
                    kG = kR + 1;
                    kB = kG + 1;
                    wR = xM * yM;



                    dwT[jT][kT] += wR;
#if 1
                    dsP = dsT[jT];
                    dsP[kR] += wR * redT;
                    dsP[kG] += wR * grnT;
                    dsP[kB] += wR * bluT;
#else
                    dsP = dsT[jT] + kR;
                    *dsP++ += wR * redT;
                    *dsP++ += wR * grnT;
                    *dsP += wR * bluT;
#endif

                    jT++;
                    dsP = dsT[jT];
                    wR = xM* yT;
                    dwT[jT][kT] += wR;
                    dsP[kR] += wR * redT;
                    dsP[kG] += wR * grnT;
                    dsP[kB] += wR * bluT;

                    kT++;
                    kR += 3;
                    kG = kR + 1;
                    kB = kG + 1;
                    wR = xT * yT;
                    dwT[jT][kT] += wR;
                    dsT[jT][kR] += wR * redT;
                    dsT[jT][kG] += wR * grnT;
                    dsT[jT][kB] += wR * bluT;

                    jT--;
                    wR = xT * yM;
                    dwT[jT][kT] += wR;
                    dsT[jT][kR] += wR * redT;
                    dsT[jT][kG] += wR * grnT;
                    dsT[jT][kB] += wR * bluT;
                }
            }
        }
    }
}




