/*****************************************************************************
 nev.c <-- rgbSmatBil.c    S.R.Lines     93.5.15-9.16
 *****************************************************************************/
#define 	 NDEBUG 1
#include	<assert.h>
#ifdef          __GNUC__
#include        <memory.h>      /* memset */
#else
#include        <string.h>      /* memset */
#endif
#include	<stdio.h>
#include	<un/unBug.h>
#include	<un/unMath.h>
#include	<un/unTypes.h>
#include	<un/unFlags.h>
#include	<m/rowZero.h>
#include	<m/mj.h>
#include	<rowSmat.h>
 /*___________________________________________________________________________*/
 /*___________________________________________________________________________*/

void
rgbSmatAffBil2Nxt
(ppf dstT, ppf dstW, ppf srcA, ppf srcB
    , ppu inpU[][4]
    , ppf fwdX, ppf fwdY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH
    , int iX, int iY, unt iW, unt iH
    , dbl trs, dbl clr
    , AffTrf *aT
    , LblFace *sLbl
)
{
    static int first = 1;
    rflt	wT;
    puc     sp;
    ppu	inpAR = inpU[0][0], inpBR = inpU[1][0];
    ppu	inpAG = inpU[0][1], inpBG = inpU[1][1];
    ppu	inpAB = inpU[0][2], inpBB = inpU[1][2];
    int     sM = iW; /* = &inpU[0][1][0] - &inpU[0][0][0]; */
    int     sP = sM + 1;
    int 	dx1 = dX + dW - 1;
    int 	dy1 = dY + dH - 1;
    int 	sx1 = sX + sW - 1;
    int 	sy1 = sY + sH - 1;
    int 	ix1 = iX + iW - 1;
    int 	iy1 = iY + iH - 1;
    int 	jI, kI, jA, kA, kB, jB, aO, bO, jT, kT;
    pfl 	pX, pY;
    flt 	vR, vG, vB, wR, wG, wB, fL, fM, fN, fO;
    flt 	xM, yM, xT, yT, xA, yA, xB, yB, cB = (flt)clr, cA = 1.0F - cB;

    flt	sigA = aT[0].scale;
    flt 	aA11 = aT[0].cosa*sigA, aA12 = -aT[0].sina*sigA, aA21 = -aA12, aA22 = aA11;
    flt 	tAx = aT[0].xdst, tAy = aT[0].ydst;
    flt 	bAx = aT[0].xsrc - aA11*tAx - aA12*tAy;
    flt	bAy = aT[0].ysrc - aA21*tAx - aA22*tAy;

    flt     sigB = aT[1].scale;
    flt     aB11 = aT[1].cosa*sigB, aB12 = -aT[1].sina*sigB, aB21 = -aB12, aB22 = aB11;
    flt     tBx = aT[1].xdst, tBy = aT[1].ydst;
    flt     bBx = aT[1].xsrc - aB11*tBx - aB12*tBy;
    flt     bBy = aT[1].ysrc - aB21*tBx - aB22*tBy;

    flt     rigC = 1.0F / aT[4].scale;
    flt     aC11 = aT[4].cosa / rigC, aC12 = -aT[4].sina / rigC, aC21 = -aC12, aC22 = aC11;
    flt     tCx = aT[4].xdst, tCy = aT[4].ydst;
    flt     bCx = aT[4].xsrc - aC11*tCx - aC12*tCy;  /* same: bCx = bAx + trs*(bBx - bAx); */
    flt     bCy = aT[4].ysrc - aC21*tCx - aC22*tCy;  /* same: bCy = bAy + trs*(bBy - bAy); */

    dbl     cAx, cAy, omt = 1.0 - trs;	    /** scale = 0.5*(aT[0].scale + aT[1].scale); **/
    dbl	    ttA = omt / aT[0].scale, ttB = trs / aT[1].scale; /* separate scales: this is correct */
    dbl     xV = (sLbl[1].nox - sLbl[0].nox)*iW;
    dbl 	yV = (sLbl[1].noy - sLbl[0].noy)*iH;          /* this "velocity" is also correct */
    dbl     xO = sLbl[0].nox*iW - 0.3055555*iW;         /* WAS 115.0 & 95.0 for iW==wH==300 */
    dbl     yO = sLbl[0].noy*iH - 0.3166666*iH;         /* but what and whence is this kludge origin? */


    if (Devopts & O_GRAY) {	/* -G: kludge origin */
        xO = aT[0].xsrc - 115.0;
        yO = aT[0].ysrc - 95.0;
    }

    rowZeroF(dstT, dX * 3, dY, dW * 3, dH);
    rowZeroF(dstW, dX, dY, dW, dH);

    if (first) { first = 0; wrn("NXT first: VELOCITY xyV: %.3f %.3f\n", xV, yV); }

    for (jA = sY, fwdX += jA, fwdY += jA; jA <= sy1; jA++) {

        cAx = aA12*jA + bAx;
        cAy = aA22*jA + bAy;

        for (kA = sX, pX = *fwdX++ + kA, pY = *fwdY++ + kA; kA <= sx1; kA++) {

            xB = *pX++;			/* using xB, xT, etc. as temp. variables */
            yB = *pY++;


            xM = (flt)(kA - trs*xB);
            yM = (flt)(jA - trs*yB);

            xT = kA - xB;
            yT = jA - yB;

            xB = aB11*xT + aB12*yT + bBx;	/* xB is now the correct x in image B */
            yB = aB21*xT + aB22*yT + bBy;

            jB = IFLOOR(yB);
            kB = IFLOOR(xB);

            /* If dest. pixloc (kT,jT) is inbounds, try to fill it with a good pixval */

            xA = (flt)(aA11*kA + cAx);
            yA = (flt)(aA21*kA + cAy);

#if 0
            if (first && jA == 140 && kA == 160) {
                first = 0;
                wrn("NXT first: VELOCITY xyV: %.3f %.3f\n", xV, yV);
                affTrfPrint(&aT[0], "NXT first: aT[0]");
                affTrfPrint(&aT[4], "NXT first: aT[4]");
                wrn("NXT first: kjA: %d %d == xyM: %.3f %.3f AND Ado: %.3f %.3f == Cdo: %.3f %.3f\n"
                    , kA, jA, xM, yM, aA11, aA12, aC11, aC12);
#endif

                if (Devopts & O_EXTR) { /* transform from dD coords using interpolated aff trf */
                    xT = (flt)(rigC*(aC11*xM + aC12*yM + bCx) - xO - trs*xV);	/* Same as default */
                    yT = (flt)(rigC*(aC21*xM + aC22*yM + bCy) - yO - trs*yV);
                }
                else if (Devopts & O_FWRD) {
                    xT = (flt)(rigC*(aC11*xM + aC12*yM) + bCx - xO - trs*xV);     /* Not the same, but matches, ie */
                    yT = (flt)(rigC*(aC21*xM + aC22*yM) + bCy - yO - trs*yV);     /* is self-consistent: A->B:B->C */
                }
                else {
                    xT = (flt)(xA*ttA + xB*ttB - xO - trs*xV);   /* KLUDGE */
                    yT = (flt)(yA*ttA + yB*ttB - yO - trs*yV);
                }

                jT = IFLOOR(yT);
                kT = IFLOOR(xT);

                if (dY <= jT && jT < dy1 && dX <= kT && kT < dx1) {

                    /* If (kB,jB) is inbounds, then get pixval from image B. */

                    if (iX <= kB && kB < ix1 && iY <= jB && jB < iy1) {

                        wT = xB - kB;
                        wR = yB - jB;
                        wG = 1.0F - wT;
                        wB = 1.0F - wR;

                        fL = wG*wB;
                        fM = wT*wB;
                        fN = wG*wR;
                        fO = wT*wR;

                        sp = &inpBR[jB][kB];
                        vR = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
                        if (wR < 0.0 || wR > 255.5F) warn("jk2:dst[%3d][%3d]= %f", jB, kB, wR);

                        sp = &inpBG[jB][kB];
                        vG = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;

                        sp = &inpBB[jB][kB];
                        vB = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
                        bO = 1;
                    }
                    else {
                        bO = 0;
                        vR = vG = vB = 0.0;
                    }

                    /* Get pixel value from image A. */
                    xA = (flt)(aA11*kA + cAx);
                    yA = (flt)(aA21*kA + cAy);

                    kI = IFLOOR(xA);
                    jI = IFLOOR(yA);

                    if (iY <= jI && jI < iy1 && iX <= kI && kI < ix1) {

                        wT = xA - kI;
                        wR = yA - jI;
                        wG = 1.0F - wT;
                        wB = 1.0F - wR;

                        fL = wG*wB;
                        fM = wT*wB;
                        fN = wG*wR;
                        fO = wT*wR;

                        sp = &inpAR[jI][kI];
                        wR = fL * sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
                        /*if (wR < 0.0F || wR > 255.5) warn("jk2:dst[%3d][%3d]= %f",jA,kA,wR); */

                        sp = &inpAG[jI][kI];
                        wG = fL * sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;

                        sp = &inpAB[jI][kI];
                        wB = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
                        aO = 1;
                    }
                    else {
                        aO = 0;
                        wR = wG = wB = 0.0F;
                    }

                    /* Now combine pixvals from img A & img B */

                    if (aO) {
                        if (bO) {
                            vR = cA*wR + cB*vR;
                            vG = cA*wG + cB*vG;
                            vB = cA*wB + cB*vB;
                        }
                        else {
                            vR = wR;
                            vG = wG;
                            vB = wB;
                        }
                    }
                    /* else if (bO) { [[do nothing]] } */

                    xT -= kT;
                    xB = 1.0F - xT;
                    yT -= jT;
                    yB = 1.0F - yT;

                    kB = kT * 3;
                    wT = xB * yB;
                    dstW[jT][kT] += wT;
                    dstT[jT][kB] += wT * vR;
                    dstT[jT][kB + 1] += wT * vG;
                    dstT[jT][kB + 2] += wT * vB;

                    kT += 1, kB += 3;
                    wT = xT * yB;
                    dstW[jT][kT] += wT;
                    dstT[jT][kB] += wT * vR;
                    dstT[jT][kB + 1] += wT * vG;
                    dstT[jT][kB + 2] += wT * vB;

                    jT++;
                    wT = xT * yT;
                    dstW[jT][kT] += wT;
                    dstT[jT][kB] += wT * vR;
                    dstT[jT][kB + 1] += wT * vG;
                    dstT[jT][kB + 2] += wT * vB;

                    kT -= 1, kB -= 3;
                    wT = xB * yT;
                    dstW[jT][kT] += wT;
                    dstT[jT][kB] += wT * vR;
                    dstT[jT][kB + 1] += wT * vG;
                    dstT[jT][kB + 2] += wT * vB;
                }
        }
    }
}

