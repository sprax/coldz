/*****************************************************************************
 rgbSmatBil3.c    S.R.Lines     93.5.15-9.16
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
#include        <unBug.h>
#include        <unMath.h>
#include        <unTypes.h>
#include        <unFlags.h>
#include        <m/rowZero.h>
#include        <m/mj.h>
#include        <m/rowSmat.h>


#define SRC_A   1
#define SRC_B   2
#define SRC_C   4

/*___________________________________________________________________________*
 *___________________________________________________________________________*/

void
rgbSmatAffBil3
( ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf srcC
, ppu inpA[4], ppu inpB[4], ppu inpC[4]
, ppf vATX, ppf vATY, ppf vABX, ppf vABY, ppf vACX, ppf vACY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, int iX, int iY, unt iW, unt iH
, dbl trs, dbl clr, flt *cfr
, AffTrf *afA, AffTrf *afB, AffTrf *afC
, LblFace *sLblA, LblFace *sLblB, LblFace *sLblC
)
{
static int first = 1;
rflt	wT;
puc     sp;
ppu	inpAR = inpA[0], inpBR = inpB[0], inpCR = inpC[0];
ppu	inpAG = inpA[1], inpBG = inpB[1], inpCG = inpC[1];
ppu	inpAB = inpA[2], inpBB = inpB[2], inpCB = inpC[2];
int     sM  = iW; /* = &inpU[0][1][0] - &inpU[0][0][0]; */
int     sP  = sM + 1;
int 	dx1 = dX + dW - 1;
int 	dy1 = dY + dH - 1;
int 	sx1 = sX + sW - 1;
int 	sy1 = sY + sH - 1;
int 	ix1 = iX + iW - 1;
int 	iy1 = iY + iH - 1;
int 	jO,kO, jA,kA, jB,kB, jC,kC, jT,kT;
unt     son;
pfl 	pABX,pABY, pACX,pACY, pATX,pATY;
flt 	wR,wG,wB, fL,fM,fN,fO;
flt 	xM,yM, xT,yT, xA,yA, xB,yB, xC,yC, cA = cfr[0], cB = cfr[1], cC = cfr[2];

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

flt     sigC =  afC->scale;
flt     aC11 =  afC->cosa*sigC, aC12 = -afC->sina*sigC, aC21 = -aC12, aC22 =  aC11;
flt     tCx  =  afC->xdst,      tCy  =  afC->ydst;
flt     bCx  =  afC->xsrc - aC11*tCx - aC12*tCy;
flt     bCy  =  afC->ysrc - aC21*tCx - aC22*tCy;

flt     rigF, aF11, aF12, aF21, aF22, tFx, tFy, bFx, bFy;
flt 	redA,grnA,bluA, redB,grnB,bluB, redC,grnC,bluC, redT,grnT,bluT;
dbl     cAx,cAy, omt = 1.0 - trs;           /** scale = 0.5*(afA->scale + afB->scale); **/
dbl     ttA = cfr[0]/sigA, ttB = cfr[1]/sigB, ttC = cfr[2]/sigC;     /* separate scales */
#if 1
dbl     xV  = (sLblC->nox - sLblA->nox)*iW;
dbl     yV  = (sLblC->noy - sLblA->noy)*iH;          /* this "velocity" is also correct */
#else
dbl     xV  = (sLblB->nox - sLblA->nox)*iW;
dbl     yV  = (sLblB->noy - sLblA->noy)*iH;          /* this "velocity" is also correct */
#endif
flt     xO  = sLblA->nox*iW - 0.3833333F*iW;         /* WAS 115.0 & 95.0 for iW==wH==300 */
flt     yO  = sLblA->noy*iH - 0.3166666F*iH;         /* but what and whence is this kludge origin? */

AffTrf  atT;
    atT.angle = (flt)(afA->angle*omt + afB->angle*trs);
    atT.scale = (flt)(afA->scale*omt + afB->scale*trs);
    atT.cosa  = (flt)cos(atT.angle);
    atT.sina  = (flt)sin(atT.angle);
    atT.xdst  = (flt)(afA->xdst*omt + afB->xdst*trs);
    atT.ydst  = (flt)(afA->ydst*omt + afB->ydst*trs);
#if 1
    atT.xsrc  = (flt)(afA->xsrc*omt + afB->xsrc*trs);
    atT.ysrc  = (flt)(afA->ysrc*omt + afB->ysrc*trs);
#else
    atT.xsrc  = (flt)(afA->xsrc*omt/afA->scale + afB->xsrc*trs/afB->scale);
    atT.ysrc  = (flt)(afA->ysrc*omt/afA->scale + afB->ysrc*trs/afB->scale);
#endif

  rigF =  1.0F/atT.scale;
  aF11 =  atT.cosa/rigF, aF12 = -atT.sina/rigF, aF21 = -aF12, aF22 =  aF11;
  tFx  =  atT.xdst,      tFy  =  atT.ydst;
  bFx  =  atT.xsrc - aF11*tFx - aF12*tFy;  /* same: bFx = bAx + trs*(bBx - bAx); */
  bFy  =  atT.ysrc - aF21*tFx - aF22*tFy;  /* same: bFy = bAy + trs*(bBy - bAy); */

  if (Devopts & O_GRAY)  {	/* -G: kludge origin */
    xO = afA->xsrc - 185.0F;
    yO = afA->ysrc - 135.0F;
  }

  rowZeroF(dstT,dX*3,dY,dW*3,dH);
  rowZeroF(dstW,dX  ,dY,dW  ,dH);

  if (first)  { first = 0; wrn("NXT first: VELOCITY xyV: %.3f %.3f\n",xV,yV); }

  vABX += sY, vABY += sY;
  vACX += sY, vACY += sY;
  vATX += sY, vATY += sY;

  for (jO = sY; jO <= sy1; jO++) {

    cAx = aA12*jO + bAx;
    cAy = aA22*jO + bAy;

    pABX = *vABX++ + sX, pABY = *vABY++ + sX;
    pACX = *vACX++ + sX, pACY = *vACY++ + sX;
    pATX = *vATX++ + sX, pATY = *vATY++ + sX;

    for (kO = sX; kO <= sx1; kO++) {

      xM  =  kO - *pATX++;
      yM  =  jO - *pATY++;

      /* Get the pixlocs in A, B, & C. */

      xT  = kO - *pABX++;              /* using xT,yT, etc. as temp. variables */
      yT  = jO - *pABY++;
      xB  = aB11*xT + aB12*yT + bBx;   /* xB is now the correct x in image B */
      yB  = aB21*xT + aB22*yT + bBy;
      jB  = IFLOOR(yB);
      kB  = IFLOOR(xB);

      xT  = kO - *pACX++;              /* using xT,yT, etc. as temp. variables */
      yT  = jO - *pACY++;
      xC  = aC11*xT + aC12*yT + bCx;   /* xC is now the correct x in image C */
      yC  = aC21*xT + aC22*yT + bCy;
      jC  = IFLOOR(yC);
      kC  = IFLOOR(xC);

      xA  = (flt)(aA11*kO + cAx);             /* xA is now the correct x in image A */
      yA  = (flt)(aA21*kO + cAy);
      jA  = IFLOOR(yA);
      kA  = IFLOOR(xA);

      /* Compute a dest. pixloc (kT,jT) by hook, crook, or mysticism */
#if 0
if (first && jO == 140 && kO == 160)  {
    first = 0;
    wrn("NXT first: VELOCITY xyV: %.3f %.3f\n",xV,yV);
    affTrfPrint(afA, "NXT first: afA");
    affTrfPrint(&aT, "NXT first:  aT");
    wrn("NXT first: kjO: %d %d == xyM: %.3f %.3f AND Ado: %.3f %.3f == Cdo: %.3f %.3f\n"
        ,kO,jO,xM,yM, aA11, aA12, aF11, aF12);
#endif

      if (Devopts & O_EXTR) { /* transform from dD coords using interpolated aff trf */
        xT  = (flt)(rigF*(aF11*xM + aF12*yM + bFx) - xO - trs*xV);	/* Same as default */
        yT  = (flt)(rigF*(aF21*xM + aF22*yM + bFy) - yO - trs*yV);
      }
      else if (Devopts & O_FWRD) {
        xT  = (flt)(rigF*(aF11*xM + aF12*yM) + bFx - xO - trs*xV);     /* Not the same, but matches, ie */
        yT  = (flt)(rigF*(aF21*xM + aF22*yM) + bFy - yO - trs*yV);     /* is self-consistent: A->B:B->C */
      }
      else {
        xT = (flt)(xA*ttA + xB*ttB + xC*ttC - xO - trs*xV);   /* KLUDGE */
        yT = (flt)(yA*ttA + yB*ttB + yC*ttC - yO - trs*yV);
      }

      jT = IFLOOR(yT);
      kT = IFLOOR(xT);

      /* If dest. pixloc (kT,jT) is inbounds, try to fill it with a good pixval */
      if (dY <= jT && jT < dy1 && dX <= kT && kT < dx1) {

        son  =  0;

        if (iY <= jA && jA < iy1 && iX <= kA && kA < ix1) {
          son = SRC_A;     /* If (kA,jA) is inbounds, then get pixval from image B. */
          wT = xA   - kA;
          wR = yA   - jA;
          wG = 1.0F - wT;
          wB = 1.0F - wR;
          fL = wG*wB;
          fM = wT*wB;
          fN = wG*wR;
          fO = wT*wR;
          sp = &inpAR[jA][kA]; redA = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
          sp = &inpAG[jA][kA]; grnA = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
          sp = &inpAB[jA][kA]; bluA = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
          /*if (wR < 0.0 || wR > 255.5) warn("jk2:dst[%3d][%3d]= %f",jO,kO,wR); */
        }
        if (iX <= kB && kB < ix1 && iY <= jB && jB < iy1) {
          son |= SRC_B;    /* If (kB,jB) is inbounds, then get pixval from image B. */
          wT = xB   - kB;
          wR = yB   - jB;
          wG = 1.0F - wT;
          wB = 1.0F - wR;
          fL = wG*wB;
          fM = wT*wB;
          fN = wG*wR;
          fO = wT*wR;
          sp = &inpBR[jB][kB]; redB = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
          sp = &inpBG[jB][kB]; grnB = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
          sp = &inpBB[jB][kB]; bluB = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
        }
        if (iX <= kC && kC < ix1 && iY <= jC && jC < iy1) {
          son |= SRC_C;    /* If (kC,jC) is inbounds, then get pixval from image C. */
          wT = xC   - kC;
          wR = yC   - jC;
          wG = 1.0F - wT;
          wB = 1.0F - wR;
          fL = wG*wB;
          fM = wT*wB;
          fN = wG*wR;
          fO = wT*wR;
          sp = &inpCR[jC][kC]; redC = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
          sp = &inpCG[jC][kC]; grnC = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
          sp = &inpCB[jC][kC]; bluC = fL*sp[0] + fM*sp[1] + fN*sp[sM] + fO*sp[sP] + 0.5F;
        }


          /* Now combine pixvals from input images A, B, C */

        switch(son) {
          case (SRC_A|SRC_B|SRC_C):
                      /* wT = cA + cB + cC; */
                      /* if (wT < inf) { wT = inf/wT; redT = wT*(cA*redA + cB*redB + cC*redC), ... */
                      redT = cA*redA + cB*redB + cC*redC;
                      grnT = cA*grnA + cB*grnB + cC*grnC;
                      bluT = cA*bluA + cB*bluB + cC*bluC;
                      break;
          case (SRC_A|SRC_B):
                      wT = 1.0F/(cA + cB);
                      /* if (wT < inf)
                         { wT = inf/wT; redT = wT*(cA*redA + cB*redB), ... */
                      redT = (cA*redA + cB*redB)*wT;
                      grnT = (cA*grnA + cB*grnB)*wT;
                      bluT = (cA*bluA + cB*bluB)*wT;
                      break;
          case (SRC_A|SRC_C):
                      wT = cA + cC;   /* wT = cA + cC; */
                      /* if (wT < inf)
                         { wT = inf/wT; redT = wT*(cA*redA + cC*redC), ... */
                      redT = (cA*redA + cC*redC)/wT;
                      grnT = (cA*grnA + cC*grnC)/wT;
                      bluT = (cA*bluA + cC*bluC)/wT;
                      break;
          case SRC_A: /* wT = MAX(inf,cA); redT = wT*redA, grnT = wT*grnA, bluT = wT*bluA;  break; */
                      redT = redA, grnT = grnA, bluT = bluA;  break;
          case SRC_B: /* wT = MAX(inf,cB); redT = wT*redB, grnT = wT*grnB, bluT = wT*bluB;  break; */
                      redT = redB, grnT = grnB, bluT = bluB;  break;
          case SRC_C: /* wT = MAX(inf,cC); redT = wT*redC, grnT = wT*grnC, bluT = wT*bluC;  break; */
                      redT = redC, grnT = grnC, bluT = bluC;  break;
          case (SRC_B|SRC_C):
                      die("rgbSmatAffBil3: Only srcs B & C ?? How the hell did this happen?");
                      wT = cB + cC;
                      /* if (wT < inf) { wT = inf/wT; redT = wT*(cB*redB + cC*redC), ... */
                      redT = cB*redB + cC*redC;
                      grnT = cB*grnB + cC*grnC;
                      bluT = cB*bluB + cC*bluC;
                      break;
          default: die("rgbSmatAffBil3: the bad son has sinned (%d)", son);
        }


     
        xT             -= kT;
        xB              = 1.0F - xT;
        yT             -= jT;
        yB              = 1.0F - yT;

        kB              = kT * 3;
        wT              = xB * yB;
        dstW[jT][kT  ] += wT;
        dstT[jT][kB  ] += wT * redT;
        dstT[jT][kB+1] += wT * grnT;
        dstT[jT][kB+2] += wT * bluT;

        kT += 1, kB += 3;
        wT              = xT * yB;
        dstW[jT][kT  ] += wT;
        dstT[jT][kB  ] += wT * redT;
        dstT[jT][kB+1] += wT * grnT;
        dstT[jT][kB+2] += wT * bluT;

        jT++;
        wT              = xT * yT;
        dstW[jT][kT  ] += wT;
        dstT[jT][kB  ] += wT * redT;
        dstT[jT][kB+1] += wT * grnT;
        dstT[jT][kB+2] += wT * bluT;

        kT -= 1, kB -= 3;
        wT              = xB * yT;
        dstW[jT][kT  ] += wT;
        dstT[jT][kB  ] += wT * redT;
        dstT[jT][kB+1] += wT * grnT;
        dstT[jT][kB+2] += wT * bluT;
      }
    }
  }
}



void
rgbSmatBil3 ( ppf dsT, ppf dwT, ppf srcA, ppf srcB, ppf srcC
            , ppf *vOX, ppf *vOY
            , int dX, int dY, unt dW, unt dH
            , int sX, int sY, unt sW, unt sH, dbl trs, flt *cfrac, dbl infimum)
{
int	dx1 = dX + dW - 1;
int	dy1 = dY + dH - 1;
int	sx1 = sX + sW - 1;
int	sy1 = sY + sH - 1;
int	jO, kO, kM, jM, scanM = srcB[1] - srcB[0] - 2;
unt 	son;
ppf     vOTX = vOX[0] + sY, vOBX = vOX[1] + sY, vOCX = vOX[2] + sY;
ppf     vOTY = vOY[0] + sY, vOBY = vOY[1] + sY, vOCY = vOY[2] + sY;
pfl	pOTX, pOTY,  pOBX, pOBY,  pOCX, pOCY, pM, psA;
int	jT, kT, kR, kG, kB;
flt	redT, grnT, bluT, redA,grnA,bluA, redB,grnB,bluB, redC,grnC,bluC;
flt	xT, yT, xM, yM, wT;
flt 	cA = cfrac[0], cB = cfrac[1], cC = cfrac[2];
rflt	wR, wG, wB;

  memset((pvd)(dsT[dY]+3*dX), 0, sizeof(flt)*dW*dH*3);
  memset((pvd)(dwT[dY] + dX), 0, sizeof(flt)*dW*dH);
  
  for (jO = sY; jO <= sy1; jO++) {

    pOTX = *vOTX++ + sX, pOTY = *vOTY++ + sX;
    pOBX = *vOBX++ + sX, pOBY = *vOBY++ + sX;
    pOCX = *vOCX++ + sX, pOCY = *vOCY++ + sX;

    for (kO = sX; kO <= sx1; pOTX++, pOBX++, pOBY++, pOCX++, pOCY++, kO++) {

      yT = (flt)(jO - trs * *pOTY++);
      jT = IFLOOR(yT);

      if (dY <= jT && jT < dy1) {

        xT = (flt)(kO - trs * *pOTX);
        kT = IFLOOR(xT);

        if (dX <= kT && kT < dx1) {
       
          yM   = jO  -  *pOBY;
          jM   = IFLOOR(yM);

          xM   = kO  -  *pOBX;
          kM   = IFLOOR(xM);

          kR   = kO * 3;
          psA  = srcA[jO];	/* @@ efficiency -- move !! */
          redA = psA[kR++];
          grnA = psA[kR++];
          bluA = psA[kR  ];    /* kR will be reset */
          son  = SRC_A;

          /* NB: (kM,jM) is NOT always inbounds. */
          if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {

            yM -= jM;
            xM -= kM;

            /* Assuming all rows in srcB are equally separated: */
            pM  =  srcB[jM]  + kM*3;
            wR  =  *pM++;
            wG  =  *pM++;
            wB  =  *pM++;
            wR += (*pM++ - wR) * xM;
            wG += (*pM++ - wG) * xM;
            wB += (*pM   - wB) * xM;
            pM +=  scanM;
            redT  =  *pM++;
            grnT  =  *pM++;
            bluT  =  *pM++;
            redT += (*pM++ - redT) * xM;
            grnT += (*pM++ - grnT) * xM;
            bluT += (*pM   - bluT) * xM;
            wR += ( redT   - wR) * yM;
            wG += ( grnT   - wG) * yM;
            wB += ( bluT   - wB) * yM;
            assert(-0.5 < wR && wR < 255.5);	/* Normally #define NDEBUG */
            assert(-0.5 < wG && wG < 255.5);
            assert(-0.5 < wB && wB < 255.5);
            redB  = wR;
            grnB  = wG;
            bluB  = wB;
            son  |= SRC_B;
          }
#if 0
          if (son & SRC_B) {
            redT = redB, grnT = grnB, bluT = bluB;
 /*******
            redT = cA*redA + (cB + cC)*redB;
            grnT = cA*grnA + (cB + cC)*grnB;
            bluT = cA*bluA + (cB + cC)*bluB;
 ********/
          }
          else redT = redA, grnT = grnA, bluT = bluA;

#else

          yM  = jO  -  *pOCY;
          jM  = IFLOOR(yM);
  
          xM  = kO  -  *pOCX;
          kM  = IFLOOR(xM);

          /* NB: (kM,jM) is NOT always inbounds. */
          if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {
          
            yM -= jM;
            xM -= kM;

            /* Assuming all rows in srcB are equally separated: */
            pM  =  srcC[jM]  + kM*3;
            wR  =  *pM++;
            wG  =  *pM++;
            wB  =  *pM++;
            wR += (*pM++ - wR) * xM;
            wG += (*pM++ - wG) * xM;
            wB += (*pM   - wB) * xM;
            pM +=  scanM;
            redT  =  *pM++;
            grnT  =  *pM++;
            bluT  =  *pM++;
            redT += (*pM++ - redT) * xM;
            grnT += (*pM++ - grnT) * xM;
            bluT += (*pM   - bluT) * xM;
            wR += ( redT   - wR) * yM;
            wG += ( grnT   - wG) * yM;
            wB += ( bluT   - wB) * yM;
            assert(-0.5F < wR && wR < 255.5F);    /* Normally #define NDEBUG */
            assert(-0.5F < wG && wG < 255.5F);
            assert(-0.5F < wB && wB < 255.5F);
            redC  = wR;
            grnC  = wG;
            bluC  = wB;
            son |= SRC_C;
          }
          
          switch(son) {
            case (SRC_A|SRC_B|SRC_C):
                        /* wT = cA + cB + cC; */
                        /* if (wT < inf)
                           { wT = inf/wT; redT = wT*(cA*redA + cB*redB + cC*redC), ... */
                        redT = cA*redA + cB*redB + cC*redC;
                        grnT = cA*grnA + cB*grnB + cC*grnC;
                        bluT = cA*bluA + cB*bluB + cC*bluC;
                        break;
            case (SRC_A|SRC_B):
                        wT = 1.0F/(cA + cB);
                        /* if (wT < inf)
                           { wT = inf/wT; redT = wT*(cA*redA + cB*redB), ... */
                        redT = (cA*redA + cB*redB)*wT;
                        grnT = (cA*grnA + cB*grnB)*wT;
                        bluT = (cA*bluA + cB*bluB)*wT;
                        break;
            case (SRC_A|SRC_C):
                        wT = cA + cC;	/* wT = cA + cC; */
                        /* if (wT < inf)
                           { wT = inf/wT; redT = wT*(cA*redA + cC*redC), ... */
                        redT = (cA*redA + cC*redC)/wT;
                        grnT = (cA*grnA + cC*grnC)/wT;
                        bluT = (cA*bluA + cC*bluC)/wT;
                        break;
            case SRC_A: /* wT = MAX(inf,cA); redT = wT*redA, grnT = wT*grnA, bluT = wT*bluA;  break; */
                        redT = redA, grnT = grnA, bluT = bluA;  break;
            case SRC_B: /* wT = MAX(inf,cB); redT = wT*redB, grnT = wT*grnB, bluT = wT*bluB;  break; */
                        redT = redB, grnT = grnB, bluT = bluB;  break;
            case SRC_C: /* wT = MAX(inf,cC); redT = wT*redC, grnT = wT*grnC, bluT = wT*bluC;  break; */
                        redT = redC, grnT = grnC, bluT = bluC;  break;
            case (SRC_B|SRC_C):
                        die("rgbSmatBil3: Only srcs B & C ?? How the hell did this happen?");
                        wT = cB + cC;
                        /* if (wT < inf) { wT = inf/wT; redT = wT*(cB*redB + cC*redC), ... */
                        redT = cB*redB + cC*redC;
                        grnT = cB*grnB + cC*grnC;
                        bluT = cB*bluB + cC*bluC;
                        break;
            default: die("rgbSmatBil3: the bad son has sinned (%d)", son);
          }

#endif

          xT  -= kT;
          xM   = 1.0F - xT;
          yT  -= jT;
          yM   = 1.0F - yT;

          kR   = kT * 3;
          kG   = kR + 1;
          kB   = kG + 1;
          wR           = xM * yM;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * redT;
          dsT[jT][kG] += wR * grnT;
          dsT[jT][kB] += wR * bluT;
          
          jT++;
          wR           = xM * yT;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * redT;
          dsT[jT][kG] += wR * grnT;
          dsT[jT][kB] += wR * bluT;

          kT  ++  ;
          kR  += 3;
          kG   = kR + 1;
          kB   = kG + 1;
          wR           = xT * yT;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * redT;
          dsT[jT][kG] += wR * grnT;
          dsT[jT][kB] += wR * bluT;

          jT--;
          wR           = xT * yM;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * redT;
          dsT[jT][kG] += wR * grnT;
          dsT[jT][kB] += wR * bluT;
        }
      }
    }
  }
}




