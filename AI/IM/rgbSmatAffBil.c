/*****************************************************************************
 ned.c <- rgbSmatBil.c    S.R.Lines     93.5.15-9.16
 *****************************************************************************/
#define 	 NDEBUG 1
#include	<assert.h>
#ifdef          __GNUC__
#include        <memory.h>      /* memset */
#else
#include        <string.h>      /* memset */
#endif
#include	<math.h>
#include	<un/unBug.h>
#include	<un/unMath.h>
#include	<un/unTypes.h>
#include	<un/unFlags.h>
#include	<m/rowZero.h>
#include	<m/mj.h>
#include	<m/rowSmat.h>
/*___________________________________________________________________________*
 *___________________________________________________________________________*/

void
rgbSmatAffBilSym2
( ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppu inpA[4], ppu inpB[4]
, ppf vATX, ppf vATY, ppf vABX, ppf vABY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, int iX, int iY, unt iW, unt iH
, flt trs, flt clr
, AffTrf *afA
, AffTrf *afB
, LblFace *sLblA
, LblFace *sLblB
)
{
static int first = 1;
rflt	wT;
puc     sp;
ppu	inpAR = inpA[0], inpBR = inpB[0];
ppu	inpAG = inpA[1], inpBG = inpB[1];
ppu	inpAB = inpA[2], inpBB = inpB[2];
int     sM  = iW; /* = &inpU[0][1][0] - &inpU[0][0][0]; */
int     sP  = sM + 1;
int 	dx1 = dX + dW - 1;
int 	dy1 = dY + dH - 1;
int 	sx1 = sX + sW - 1;
int 	sy1 = sY + sH - 1;
int 	ix1 = iX + iW - 1;
int 	iy1 = iY + iH - 1;
int 	jI, kI, jA, kA, kB, jB, aO,bO, jT,kT;
pfl 	pABX,pABY, pATX,pATY;
flt 	vR,vG,vB, wR,wG,wB, fL,fM,fN,fO;
flt 	xM, yM, xT, yT, xA, yA, xB, yB, cA = 1.0F - clr, cB = clr;

flt	sigA =  afA->scale;
flt 	aA11 =  afA->cosa*sigA, aA12 = -afA->sina*sigA, aA21 = -aA12, aA22 =  aA11;
flt 	tAx  =  afA->xdst,      tAy  =  afA->ydst;
flt 	bAx  =  afA->xsrc - aA11*tAx - aA12*tAy;
flt	bAy  =  afA->ysrc - aA21*tAx - aA22*tAy;

flt     sigB =  afB->scale;
flt     aB11 =  afB->cosa*sigB, aB12 = -afB->sina*sigB, aB21 = -aB12, aB22 =  aB11;
flt     tBx  =  afB->xdst,      tBy  =  afB->ydst;
flt     bBx  =  afB->xsrc - aB11*tBx - aB12*tBy;
flt     bBy  =  afB->ysrc - aB21*tBx - aB22*tBy;
flt     omt = 1.0F - trs;           /** scale = 0.5*(afA->scale + afB->scale); **/

flt     rigC, aC11, aC12, aC21, aC22, tCx, tCy, bCx, bCy;
dbl     cAx, cAy; 
dbl     ttA = omt/sigA, ttB = trs/sigB;     /* separate scales: this is correct */
dbl     xV  = (sLblB->nox - sLblA->nox)*iW;
dbl     yV  = (sLblB->noy - sLblA->noy)*iH;  /* this "velocity" is also correct */
dbl     xO  = sLblA->nox*iW - 115.0;
dbl     yO  = sLblA->noy*iH - 95.0;         /* but what and whence is this kludge origin? */

AffTrf  atT;
    atT.angle =  afA->angle*omt + afB->angle*trs;
    atT.scale =  afA->scale*omt + afB->scale*trs;
    atT.cosa  =  cosf(atT.angle);
    atT.sina  =  (flt)sin(atT.angle);
    atT.xdst  =  afA->xdst*omt + afB->xdst*trs;
    atT.ydst  =  afA->ydst*omt + afB->ydst*trs;
#if 1
    atT.xsrc  =  afA->xsrc*omt + afB->xsrc*trs;
    atT.ysrc  =  afA->ysrc*omt + afB->ysrc*trs;
#else
    atT.xsrc  =  afA->xsrc*omt/afA->scale + afB->xsrc*trs/afB->scale;
    atT.ysrc  =  afA->ysrc*omt/afA->scale + afB->ysrc*trs/afB->scale;
#endif

  rigC =  1.0F/atT.scale;
  aC11 =  atT.cosa/rigC, aC12 = -atT.sina/rigC, aC21 = -aC12, aC22 =  aC11;
  tCx  =  atT.xdst,      tCy  =  atT.ydst;
  bCx  =  atT.xsrc - aC11*tCx - aC12*tCy;  /* same: bCx = bAx + trs*(bBx - bAx); */
  bCy  =  atT.ysrc - aC21*tCx - aC22*tCy;  /* same: bCy = bAy + trs*(bBy - bAy); */

  if (Devopts & O_GRAY)  {	/* -G: kludge origin */
    xO = afA->xsrc - 185.0;
    yO = afA->ysrc - 135.0;
  }

  rowZeroF(dstT,dX*3,dY,dW*3,dH);
  rowZeroF(dstW,dX  ,dY,dW  ,dH);

  if (first)  { first = 0; wrn("NXT first: VELOCITY xyV: %.3f %.3f\n",xV,yV); }

  vABX += sY, vABY += sY;
  vATX += sY, vATY += sY;

  for (jA = sY; jA <= sy1; jA++) {

    cAx = aA12*jA + bAx;
    cAy = aA22*jA + bAy;

    pABX = *vABX++ + sX, pABY = *vABY++ + sX;
    pATX = *vATX++ + sX, pATY = *vATY++ + sX;

    for (kA = sX; kA <= sx1; kA++) {

#if 0					/* This is the old way, same as if vAB == vAT */
      xT  =  xM  =  kA - trs**pABX++;
      yT  =  yM  =  jA - trs**pABY++;
#else
      xM  =  kA - *pATX++;
      yM  =  jA - *pATY++;
      xT  =  kA - *pABX++;		/* using xT,yT, etc. as temp. variables */
      yT  =  jA - *pABY++;
#endif
      xB  =  aB11*xT + aB12*yT + bBx;	/* xB is now the correct x in image B */
      yB  =  aB21*xT + aB22*yT + bBy;

      jB  =  IFLOOR(yB);
      kB  =  IFLOOR(xB);

      /* If dest. pixloc (kT,jT) is inbounds, try to fill it with a good pixval */

      xA = (flt)(aA11*kA + cAx);
      yA = (flt)(aA21*kA + cAy);

#if 0
if (first && jA == 140 && kA == 160)  {
    first = 0;
    wrn("NXT first: VELOCITY xyV: %.3f %.3f\n",xV,yV);
    affTrfPrint(afA, "NXT first: afA");
    affTrfPrint(&aT, "NXT first:  aT");
    wrn("NXT first: kjA: %d %d == xyM: %.3f %.3f AND Ado: %.3f %.3f == Cdo: %.3f %.3f\n"
        ,kA,jA,xM,yM, aA11, aA12, aC11, aC12);
#endif

      if (Devopts & O_EXTR) { /* transform from dD coords using interpolated aff trf */
        xT  = (flt)(rigC*(aC11*xM + aC12*yM + bCx) - xO - trs*xV);	/* Same as default */
        yT  = (flt)(rigC*(aC21*xM + aC22*yM + bCy) - yO - trs*yV);
      }
      else if (Devopts & O_FWRD) {
        xT  = (flt)(rigC*(aC11*xM + aC12*yM) + bCx - xO - trs*xV);     /* Not the same, but matches, ie */
        yT  = (flt)(rigC*(aC21*xM + aC22*yM) + bCy - yO - trs*yV);     /* is self-consistent: A->B:B->C */
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

          wT = xB   - kB;
          wR = yB   - jB;
          wG = 1.0F - wT;
          wB = 1.0F - wR;

          fL = wG*wB;
          fM = wT*wB;
          fN = wG*wR;
          fO = wT*wR;

          sp = &inpBR[jB][kB];
          vR = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
          if (wR < 0.0F || wR > 255.5F) warn("jk2:dst[%3d][%3d]= %f",jB,kB,wR);

          sp = &inpBG[jB][kB];
          vG  = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;

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
 
          wT = xA   - kI;
          wR = yA   - jI;
          wG = 1.0F - wT;
          wB = 1.0F - wR;

          fL = wG*wB;
          fM = wT*wB;
          fN = wG*wR;
          fO = wT*wR;

          sp = &inpAR[jI][kI];
          wR = fL * sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
          /*if (wR < 0.0 || wR > 255.5) warn("jk2:dst[%3d][%3d]= %f",jA,kA,wR); */

          sp = &inpAG[jI][kI];
          wG  = fL * sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;

          sp = &inpAB[jI][kI];
          wB = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
          aO = 1;
        }
        else {
          aO = 0;
          wR = wG = wB = 0.0;
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
     
        xT             -= kT;
        xB              = 1.0F - xT;
        yT             -= jT;
        yB              = 1.0F - yT;

        kB              = kT * 3;
        wT              = xB * yB;
        dstW[jT][kT  ] += wT;
        dstT[jT][kB  ] += wT * vR;
        dstT[jT][kB+1] += wT * vG;
        dstT[jT][kB+2] += wT * vB;

        kT += 1, kB += 3;
        wT              = xT * yB;
        dstW[jT][kT  ] += wT;
        dstT[jT][kB  ] += wT * vR;
        dstT[jT][kB+1] += wT * vG;
        dstT[jT][kB+2] += wT * vB;

        jT++;
        wT              = xT * yT;
        dstW[jT][kT  ] += wT;
        dstT[jT][kB  ] += wT * vR;
        dstT[jT][kB+1] += wT * vG;
        dstT[jT][kB+2] += wT * vB;

        kT -= 1, kB -= 3;
        wT              = xB * yT;
        dstW[jT][kT  ] += wT;
        dstT[jT][kB  ] += wT * vR;
        dstT[jT][kB+1] += wT * vG;
        dstT[jT][kB+2] += wT * vB;
      }
    }
  }
}

