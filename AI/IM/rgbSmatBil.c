/*****************************************************************************
 rgbSmatBil.c    S.R.Lines     93.5.15-9.16
 NOTES: if trs < TEENY, just rowCopy srcA to dsT?
 Must have dims(dsT) <= dims(fwdX) == dims(fwdY), and borders of srcs may be
 positive or negative (dims(srcA) <> dims(dsT)), but they must be known
 explicitly.  In general, dims(dsT) < dms(srcA) for best results.
 All all must be aligned (i.e., [0][0] must refer to same "place")
 --this makes the algorithm and subimage processing simple.
  
 Destination Boundaries:
 dY is the top, or coord of the first row, dW is the rectangle's width, etc.
 Source Boundaries:
 sX is the coord of leftmost stored pxl, and sx1 is the rightmost pxl from
 which to anchor an interpolation, etc. -- for bilinear interpolation,
 think of sX as 0 and sx1 as wide-2.

 NB: srcA & srcB are treated asymmetrically in at least one way:
 1) flow is assumed to be relative to A's coord. frame.

#include	<stdlib.h>
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
#include	<m/rowNwts.h>
#include	<m/rowPlug.h>
#include	<m/mj.h>
#include	<m/rowSmat.h>
#include	<blerp.h>

/*___________________________________________________________________________*
 *___________________________________________________________________________*/

void
rgbSmatBil ( ppf dsT, ppf dwT, ppf srcO, ppf fwdX, ppf fwdY
           , int dX, int dY, unt dW, unt dH
           , int sX, int sY, unt sW, unt sH, dbl trans)
{
int	dx1 = dX + dW - 1;
int	dy1 = dY + dH - 1;
int	sx1 = sX + sW - 1;
int	sy1 = sY + sH - 1;
int	jO, kO, kM, jM;
pfl	pX, pY, sp;
flt	xT, yT, xM, yM, trs = (flt)trans;
int	jT, kT, kR, kG, kB;
flt	vR, vG, vB;
rgs flt	wR;

  memset((pvd)(dsT[dY]+3*dX), 0, sizeof(flt)*dW*dH*3);
  memset((pvd)(dwT[dY] + dX), 0, sizeof(flt)*dW*dH);
  
  for (srcO += sY, jO = sY, fwdX += jO, fwdY += jO; jO <= sy1; srcO++, jO++) {
    for (kO = sX, pX = *fwdX++ + kO, pY = *fwdY++ + kO; kO <= sx1; pX++, kO++) {

      yM = *pY++;
      yT = jO - trs * yM;
      jT = IFLOOR(yT);

      if (dY <= jT && jT < dy1) {

        xM = *pX;
        xT = kO - trs * xM;
        kT = IFLOOR(xT);

        if (dX <= kT && kT < dx1) {
       
          yM  = jO  -  yM;
          jM  = IFLOOR(yM);
          xM  = kO  -  xM;
          kM  = IFLOOR(xM);
          kR  = kO * 3;

          sp  = *srcO + kO*3;
          vR  = *sp;
          vG  =  sp[1];
          vB  =  sp[2];
          
          xT  -= kT;
          xM   = 1.0F - xT;
          yT  -= jT;
          yM   = 1.0F - yT;

          kR   = kT * 3;
          kG   = kR + 1;
          kB   = kG + 1;
          wR   = xM * yM;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * vR;
          dsT[jT][kG] += wR * vG;
          dsT[jT][kB] += wR * vB;
          
          jT++;
          wR           = xM * yT;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * vR;
          dsT[jT][kG] += wR * vG;
          dsT[jT][kB] += wR * vB;

          kT  ++  ;
          kR  += 3;
          kG   = kR + 1;
          kB   = kG + 1;
          wR   = xT * yT;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * vR;
          dsT[jT][kG] += wR * vG;
          dsT[jT][kB] += wR * vB;

          jT--;
          wR           = xT * yM;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * vR;
          dsT[jT][kG] += wR * vG;
          dsT[jT][kB] += wR * vB;
        }
      }
    }
  }
}

void
rgbSmatBil2 ( ppf dsT, ppf dwT, ppf srcO, ppf srcM, ppf fwdX, ppf fwdY
             , int dX, int dY, unt dW, unt dH
             , int sX, int sY, unt sW, unt sH, dbl trans, dbl clr)
{
int	dx1 = dX + dW - 1;
int	dy1 = dY + dH - 1;
int	sx1 = sX + sW - 1;
int	sy1 = sY + sH - 1;
int	jO, kO, kM, jM, scanM = srcM[1] - srcM[0] - 2;
pfl	pX, pY, pM;
flt	xT, yT, xM, yM, cM = (flt)clr, cO = 1.0F - cM, trs = (flt)trans;
int	jT, kT, kR, kG, kB;
flt	vR, vG, vB;
rgs flt	wR, wG, wB;

  memset((pvd)(dsT[dY]+3*dX), 0, sizeof(flt)*dW*dH*3);
  memset((pvd)(dwT[dY] + dX), 0, sizeof(flt)*dW*dH);
  
  for (jO = sY, fwdX += jO, fwdY += jO; jO <= sy1; jO++) {
    for (kO = sX, pX = *fwdX++ + kO, pY = *fwdY++ + kO; kO <= sx1; pX++, kO++) {

      yM = *pY++;
      yT = jO - trs * yM;
      jT = IFLOOR(yT);

      if (dY <= jT && jT < dy1) {

        xM = *pX;
        xT = kO - trs * xM;
        kT = IFLOOR(xT);

        if (dX <= kT && kT < dx1) {
       
          yM  = jO  -  yM;
          jM  = IFLOOR(yM);
          xM  = kO  -  xM;
          kM  = IFLOOR(xM);
          kR  = kO * 3;

          /* NB: (kM,jM) is NOT always inbounds. */
          if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {

            yM -= jM;
            xM -= kM;

            /* Assuming all rows in srcM are equally separated: */
            pM  =   srcM[jM]  + kM*3;
            wR  =  *pM++;
            wG  =  *pM++;
            wB  =  *pM++;
            wR += (*pM++ - wR) * xM;
            wG += (*pM++ - wG) * xM;
            wB += (*pM   - wB) * xM;
            pM +=  scanM;
            vR  =  *pM++;
            vG  =  *pM++;
            vB  =  *pM++;
            vR += (*pM++ - vR) * xM;
            vG += (*pM++ - vG) * xM;
            vB += (*pM   - vB) * xM;
            wR += ( vR   - wR) * yM;
            wG += ( vG   - wG) * yM;
            wB += ( vB   - wB) * yM;

            vR  = wR*cM + cO*srcO[jO][kR++];
            vG  = wG*cM + cO*srcO[jO][kR++];
            vB  = wB*cM + cO*srcO[jO][kR  ];    /* kR will be reset */
          }
          else {
            vR = srcO[jO][kR++];
            vG = srcO[jO][kR++];
            vB = srcO[jO][kR  ];
          }
          
          xT  -= kT;
          xM   = 1.0F - xT;
          yT  -= jT;
          yM   = 1.0F - yT;

          kR   = kT * 3;
          kG   = kR + 1;
          kB   = kG + 1;
          wR   = xM * yM;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * vR;
          dsT[jT][kG] += wR * vG;
          dsT[jT][kB] += wR * vB;
          
          jT++;
          wR           = xM * yT;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * vR;
          dsT[jT][kG] += wR * vG;
          dsT[jT][kB] += wR * vB;

          kT  ++  ;
          kR  += 3;
          kG   = kR + 1;
          kB   = kG + 1;
          wR   = xT * yT;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * vR;
          dsT[jT][kG] += wR * vG;
          dsT[jT][kB] += wR * vB;

          jT--;
          wR           = xT * yM;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * vR;
          dsT[jT][kG] += wR * vG;
          dsT[jT][kB] += wR * vB;
        }
      }
    }
  }
}

void
rgbSmatAffBil2T
( ppf dstT, ppf dstW, ppf srcA, ppf srcB
, ppu inpU[][4], ppf vATX, ppf vATY, ppf vABX, ppf vABY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, int iX, int iY, unt iW, unt iH
, dbl trans, dbl clr
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
flt 	xM, yM, xT, yT, xA, yA, xB, yB, trs = (flt)trans, cA = (flt)(1.0 - clr), cB = (flt)clr;

flt	sigA =  aT[0].scale;
flt 	aA11 =  aT[0].cosa*sigA, aA12 = -aT[0].sina*sigA, aA21 = -aA12, aA22 =  aA11;
flt 	tAx  =  aT[0].xdst,      tAy  =  aT[0].ydst;
flt 	bAx  =  aT[0].xsrc - aA11*tAx - aA12*tAy;
flt	bAy  =  aT[0].ysrc - aA21*tAx - aA22*tAy;

flt     sigB =  aT[1].scale;
flt     aB11 =  aT[1].cosa*sigB, aB12 = -aT[1].sina*sigB, aB21 = -aB12, aB22 =  aB11;
flt     tBx  =  aT[1].xdst,      tBy  =  aT[1].ydst;
flt     bBx  =  aT[1].xsrc - aB11*tBx - aB12*tBy;
flt     bBy  =  aT[1].ysrc - aB21*tBx - aB22*tBy;

flt     rigC =  (flt)(1.0/aT[4].scale);
flt     aC11 =  aT[4].cosa/rigC, aC12 = -aT[4].sina/rigC, aC21 = -aC12, aC22 =  aC11;
flt     tCx  =  aT[4].xdst,      tCy  =  aT[4].ydst;
flt     bCx  =  aT[4].xsrc - aC11*tCx - aC12*tCy;  /* same: bCx = bAx + trs*(bBx - bAx); */
flt     bCy  =  aT[4].ysrc - aC21*tCx - aC22*tCy;  /* same: bCy = bAy + trs*(bBy - bAy); */

flt     cAx,cAy, omt = 1.0F - trs;	    /** scale = 0.5*(aT[0].scale + aT[1].scale); **/
flt	ttA = omt/aT[0].scale, ttB = trs/aT[1].scale; /* separate scales: this is correct */
flt	xV  = (sLbl[1].nox - sLbl[0].nox)*iW;
flt 	yV  = (sLbl[1].noy - sLbl[0].noy)*iH;          /* this "velocity" is also correct */
flt	xO  = sLbl[0].nox*iW - 115.0F;
flt 	yO  = sLbl[0].noy*iH - 95.0F;         /* but what and whence is this kludge origin? */

  if (Devopts & O_GRAY)  {	
    wrn("rgbSmatAffBil2T [%s]: -G kludge origin", __FILE__);
    xO = aT[0].xsrc - 185.0F;
    yO = aT[0].ysrc - 135.0F;
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

      xB = *pABX++;			/* using xB, xT, etc. as temp. variables */
      yB = *pABY++;

#if 1
      xM = kA - *pATX++;
      yM = jA - *pATY++;
#else
      xM = kA - trs*xB;
      yM = jA - trs*yB;
#endif

      xT  = kA - xB;
      yT  = jA - yB;

      xB  = aB11*xT + aB12*yT + bBx;	/* xB is now the correct x in image B */
      yB  = aB21*xT + aB22*yT + bBy;

      jB  = IFLOOR(yB);
      kB  = IFLOOR(xB);

      /* If dest. pixloc (kT,jT) is inbounds, try to fill it with a good pixval */

      xA = aA11*kA + cAx;
      yA = aA21*kA + cAy;

#if 0
if (first && jA == 140 && kA == 160)  {
    first = 0;
    wrn("NXT first: VELOCITY xyV: %.3f %.3f\n",xV,yV);
    affTrfPrint(&aT[0], "NXT first: aT[0]");
    affTrfPrint(&aT[4], "NXT first: aT[4]");
    wrn("NXT first: kjA: %d %d == xyM: %.3f %.3f AND Ado: %.3f %.3f == Cdo: %.3f %.3f\n"
        ,kA,jA,xM,yM, aA11, aA12, aC11, aC12);
#endif

      if (Devopts & O_EXTR) { /* transform from dD coords using interpolated aff trf */
        xT  = rigC*(aC11*xM + aC12*yM + bCx) - xO - trs*xV;	/* Same as default */
        yT  = rigC*(aC21*xM + aC22*yM + bCy) - yO - trs*yV;
      }
      else if (Devopts & O_GRAY) {
        xT  = rigC*(aC11*xM + aC12*yM) + bCx - xO - trs*xV;     /* Not the same, but matches, ie */
        yT  = rigC*(aC21*xM + aC22*yM) + bCy - yO - trs*yV;     /* is self-consistent: A->B:B->C */
      }
      else {
        xT = xA*ttA + xB*ttB - xO - trs*xV;   /* KLUDGE */
        yT = yA*ttA + yB*ttB - yO - trs*yV;
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
          if (wR < 0.0 || wR > 255.5) warn("jk2:dst[%3d][%3d]= %f",jB,kB,wR);

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
        xA = aA11*kA + cAx;
        yA = aA21*kA + cAy;

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




int rgbSmatBil2Tri
( ppf dsT, ppf dwT, ppf srcO, ppf srcP, ppf foX, ppf foY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH, dbl trans, dbl clr, dbl over
, flt A[], flt F[])	/* F = f(A) */
{
regflt	ss,tt;
ppf 	vO, vP, vX, vY;
pfl 	pO, pP, pX, pY;
int     dx1 = dX + dW - 1;
int     dy1 = dY + dH - 1;
int     sx1 = sX + sW - 1;
int     sy1 = sY + sH - 1, y2_gt_y3 = 0, scanP = srcP[1] - srcP[0] - 2;
int 	jO,jI,jM,jF,jP,jT, kO,kI,kF,kP,kT, kR,kG,kB;
flt 	fX, fY, xP,yP, xT,yT, cP = (flt)clr, cO = 1.0F - cP;
flt	yM, s21, s31, s32, xI, xF, gL,hL, gR,hR, gRL,hRL, overF = 1.0;
flt     vR, vG, vB, wR, wG, wB;
flt     x1  = A[0],  y1 = A[1];	/* A = [Ax,Ay], 2x3 matrix of xy row vectors*/
flt     x2  = A[2],  y2 = A[3];	/* Expecting y1 <= y2 & y3 and the angle  */
flt     x3  = A[4],  y3 = A[5]; /* from side12 to side23 > 0 */
flt 	g1  = F[0],  h1 = F[1];	/* F = f(A) = [g(Ax),h(Ay)] */
flt 	g2  = F[2],  h2 = F[3];	/* ie: F[1] = g(Ax1), F[2] = h(Ax2) */
flt 	g3  = F[4],  h3 = F[5];
flt	g21 = (g2 - g1), g31 = (g3 - g1); /* g32 = (g3 - g2); */
flt	h21 = (h2 - h1), h31 = (h3 - h1); /* h32 = (h3 - h2); */
flt 	x21 = (x2 - x1), x31 = (x3 - x1); /* x32 = (x3 - x2); */
flt     trs = (flt)trans;

  memset((pvd)(dsT[dY] + dX*3), 0, sizeof(flt)*dW*dH*3);
  memset((pvd)(dwT[dY] + dX  ), 0, sizeof(flt)*dW*dH  );

  s21 = y2 - y1,  s31 = y3 - y1;
  ss  = (flt)atan2f(s21,x21);
  tt  = (flt)atan2f(s31,x31);
  s32 = tt - ss;
  if (s21 < -0.01 || s31 < -0.01 || s32 < 0.0)
    wrn(
"rsmt Axy1:%4.1f %4.1f: s21:%+4.3f  s31:%+4.3f  a31:%+4.3f - a21:%+4.3f = a32:%+4.3f"
       ,   x1,   y1,    s21,        s31,        tt,          ss,          s32);

  assert(y1  <=  y2);
  assert(y1  <=  y3);
#if 0
  assert(0.0 <=  atan2f(y3-y2,x3-x2)); /* Relax these assumptions later. */
#endif

  jI = ICEIL(y1);       /* Ensure (ss >= 0);  was: jI = IRINTF(y1); */
  if (y2 <= y3)  jM = IRINTF(y2), jF = IRINTF(y3), yM = y2;
  else           jM = IRINTF(y3), jF = IRINTF(y2), yM = y3, y2_gt_y3 = 1;
  if (jI < jM) {	/* implies s21 & s31 won't blow up */
    s21  = 1.0F/(y2 - y1);
    s31  = 1.0F/(y3 - y1);

    if (s21 > 1.0 || s31 > 1.0)
      warn("s21:%.3f > 1.0 || s31:%.3f > 1.0 at %d <= jO:%d <= %d"
	  ,s21,s31,jI,jI,jM);
    for (jO = jI, vO= &srcO[jO], vP= &srcP[jO], vX= &foX[jO], vY= &foY[jO]; jO <= jM; jO++) {
      ss   = (jO - y1);
      tt   = ss * s31;
      hL   = h1 + h31*tt;
      gL   = g1 + g31*tt;
      xI   = x1 + x31*tt;
      tt   = ss * s21;
      hR   = h1 + h21*tt;
      gR   = g1 + g21*tt;
      xF   = x1 + x21*tt;
      kI   = IRINTF(xI);  kF = IRINTF(xF);    /* Later: switch if x3 < x2 */

      pO   = *vO++ + kI, pP = *vP++ + kI;
      pX   = *vX++ + kI, pY = *vY++ + kI;

      if (kI > kF) {
        die("rsmt upper: kI:%d > kF:%d at j:%d",kI,kF,jO);
      }
      else { 	/* (kI <= kF) */
        if (kI != kF)   ss  = 1.0F/(kF - kI);
        else            ss  = 0.0F;
        gRL  = (gR - gL);
        hRL  = (hR - hL);
        for (tt=0.0F, kO = kI; kO <= kF; tt += ss, kO++)  {
#if 1
          fX     =  foX[jO][kO];
          fY     =  foY[jO][kO];
#else
          fX     =  gL + gRL*tt;
          fY     =  hL + hRL*tt;
#endif
          yP = fY;
          yT = jO - trs * yP;
          jT = IFLOOR(yT);

          if (dY <= jT && jT < dy1) {

            xP = fX;
            xT = kO - trs * xP;
            kT = IFLOOR(xT);

            if (dX <= kT && kT < dx1) {

              yP   = jO  -  yP;
              jP   = IFLOOR(yP);
              xP   = kO  -  xP;
              kP   = IFLOOR(xP);
              kR   = kO * 3;

              /* NB: (kP,jP) is NOT always inbounds. */
              if (sX <= kP && kP < sx1 && sY <= jP && jP < sy1) {

                yP -= jP;
                xP -= kP;

                /* Assuming all rows in srcP are equally separated: */
                pP  =  srcP[jP]  + kP*3;
                wR  =  *pP++;
                wG  =  *pP++;
                wB  =  *pP++;
                wR += (*pP++ - wR) * xP;
                wG += (*pP++ - wG) * xP;
                wB += (*pP   - wB) * xP;
                pP +=  scanP;
                vR  =  *pP++;
                vG  =  *pP++;
                vB  =  *pP++;
                vR += (*pP++ - vR) * xP;
                vG += (*pP++ - vG) * xP;
                vB += (*pP   - vB) * xP;
                wR += ( vR   - wR) * yP;
                wG += ( vG   - wG) * yP;
                wB += ( vB   - wB) * yP;

                if (Devopts & O_BQDR) {
                  vR  = 255.0;	/* wR*cP + cO*srcO[jO][kR++]; */
                  vG  = 255.0;	/* wG*cP + cO*srcO[jO][kR++]; */
                  vB  =   0.0;	/* wB*cP + cO*srcO[jO][kR  ]; */
                } else { 
                  vR  = wR*cP + cO*srcO[jO][kR++];
                  vG  = wG*cP + cO*srcO[jO][kR++];
                  vB  = wB*cP + cO*srcO[jO][kR  ];
                }
              }
              else {
                vR  =   0.0;	/* wR*cP + cO*srcO[jO][kR++]; */
                vG  = 255.0;	/* wG*cP + cO*srcO[jO][kR++]; */
                vB  = 255.0;	/* wB*cP + cO*srcO[jO][kR  ]; */
              }

              xT  -= kT;
              xP   = 1.0F - xT;
              yT  -= jT;
              yP   = 1.0F - yT;

#if 1
              kR           = kT * 3;
              kG           = kR + 1;
              kB           = kG + 1;
              wR           = xP * yP * overF;
              dwT[jT][kT] += wR;
              dsT[jT][kR] += wR * vR;
              dsT[jT][kG] += wR * vG;
              dsT[jT][kB] += wR * vB;

              jT++;
              wR           = xP * yT;
              dwT[jT][kT] += wR      * overF;
              dsT[jT][kR] += wR * vR;
              dsT[jT][kG] += wR * vG;
              dsT[jT][kB] += wR * vB;
    
              kT++;
              kR          += 3;
              kG           = kR + 1;
              kB           = kG + 1;
              wR           = xT * yT * overF;
              dwT[jT][kT] += wR;
              dsT[jT][kR] += wR * vR;
              dsT[jT][kG] += wR * vG;
              dsT[jT][kB] += wR * vB;
                        
              jT--;
              wR           = xT * yP * overF;
              dwT[jT][kT] += wR;
              dsT[jT][kR] += wR * vR;
              dsT[jT][kG] += wR * vG;
              dsT[jT][kB] += wR * vB;
#else
              kR           = kT * 3;
              kG           = kR + 1;
              kB           = kG + 1;
              wR           = xP * yP * overF;
              dwT[jT][kT]  = wR;
              dsT[jT][kR]  = wR * vR;
              dsT[jT][kG]  = wR * vG;
              dsT[jT][kB]  = wR * vB;

              jT++;
              wR           = xP * yT;
              dwT[jT][kT]  = wR      * overF;
              dsT[jT][kR]  = wR * vR;
              dsT[jT][kG]  = wR * vG;
              dsT[jT][kB]  = wR * vB;

              kT++;
              kR          += 3;
              kG           = kR + 1;
              kB           = kG + 1;
              wR           = xT * yT * overF;
              dwT[jT][kT]  = wR;
              dsT[jT][kR]  = wR * vR;
              dsT[jT][kG]  = wR * vG;
              dsT[jT][kB]  = wR * vB;

              jT--;
              wR           = xT * yP * overF;
              dwT[jT][kT]  = wR;
              dsT[jT][kR]  = wR * vR;
              dsT[jT][kG]  = wR * vG;
              dsT[jT][kB]  = wR * vB;
#endif
            }
          }
        }
      }
    }
  }
  else --jM;	/* Compensate for jO about to be initialized to jM+1 */

#if 00
/** warn("rgbSmatBil2Tri: Now for lower triangle! -- jM,jF: %d %d",jM,jF); **/
/** if (y3 < y2) then side12 continues as L, but side32 replaces side13 **/
/** if (y2 < y3) then side13 continues as R, but side23 replaces side12 **/

  if (jM < jF) {	/* Implies y2 != y3, so s32 won't blow up. */
    s32  = 1.0F/(y3 - y2);
    if (y2_gt_y3) {	/* side12 continues as R, side32 replaces side13 as L */

      s21  = 1.0F/(y2 - y1);

      for (jI=jM+1, jO = jI, vX = &foX[jO], vY = &foY[jO]; jO <= jF; jO++) {

        tt   =     - s32*(jO - yM);	/* NB: y3 < y2, so s32 < 0 */
        hL   =  h3 - h32*tt;
        gL   =  g3 - g32*tt;
        xI   =  x3 - x32*tt;

        tt   =       s21*(jO - y1);
        hR   =  h1 + h21*tt;
        gR   =  g1 + g21*tt;
        xF   =  x1 + x21*tt;

        kI   = IRINTF(xI);  kF = IRINTF(xF);    /* Later: switch if x3 < x2 */
        if (kI < kF) {
          gRL  = (gR - gL);
          hRL  = (hR - hL);
          ss   = 1.0F/(kF - kI);
          for (tt=0.0F, kO=kI, pX= *vX++ + kI, pY= *vY++ + kI; kO <= kF; tt+=ss, kO++)  {
            *pX++  =  gL + gRL*tt;
            *pY++  =  hL + hRL*tt;
          }
        }
/**************************************** @@ beg temporary test ***************/
        else if (kI == kF) {
          (*vX++)[kI]  =  (gL + gR)*0.5;
          (*vY++)[kI]  =  (hL + hR)*0.5;
        }
        else {
          vX++, vY++; warn("rsmt LOWER 1232: kI:%d > kF:%d at j:%d",kI,kF,jO);
        }
/**************************************** @@ end temporary test ***************/
      }
    }
    else {  /* (! y2_gt_y3), so side13 continues as L, side23 replaces side 12 as R */

      s31  = 1.0F/(y3 - y1);

      for (jI=jM+1, jO = jI, vX = &foX[jO], vY = &foY[jO]; jO <= jF; jO++) {

        tt   =       s31*(jO - y1);
        hL   =  h1 + h31*tt;
        gL   =  g1 + g31*tt;
        xI   =  x1 + x31*tt;

        tt   =       s32*(jO - yM);
        hR   =  h2 + h32*tt;
        gR   =  g2 + g32*tt;
        xF   =  x2 + x32*tt;

        kI   = IRINTF(xI);
        kF   = ICEIL(xF);    /* was: IRINTF(xF);  Later: switch if x3 < x2 */
        if (kI < kF) {
          gRL  = (gR - gL);
          hRL  = (hR - hL);
          ss   = 1.0F/(kF - kI);
          for (tt=0.0F, kO=kI, pX= *vX++ + kI, pY= *vY++ + kI; kO <= kF; tt+=ss, kO++)  {
            *pX++  =  gL + gRL*tt;
            *pY++  =  hL + hRL*tt;
          }
        }
/**************************************** @@ beg temporary test ***************/
        else if (kI == kF) {
          (*vX++)[kI]  =  (gL + gR)*0.5;
          (*vY++)[kI]  =  (hL + hR)*0.5;
        }
        else {
          vX++, vY++; warn("rsmt LOWER 1323: kI:%d > kF:%d at j:%d",kI,kF,jO);
          assert(0==1);
        }
/**************************************** @@ end temporary test ***************/
      }
    }
  }
#endif 
return 1;
}


void
rgbRepDstIfSrcFlt(ppf dst, ppf dwt, ppf src, ppf swt, int dX, int dY, unt dW, unt dH, dbl inf)
{
regflt  ff;
int 	k, k3, dX3 = dX*3;
pfl 	dp, dw, sp, sw; 
flt 	inff = (flt)inf;

  dst += dY, dwt += dY, src += dY, swt += dY;

  for (swt += dY; dH; dH--) {

    dp = *dst++ + dX3;
    sp = *src++ + dX3;
    dw = *dwt++ + dX ;
    sw = *swt++ + dX ;

    k = dW; while (k--) {
      if ((ff  = sw[k]) > inff)  {
        dw[k]  = 1.0F;
        k3     = k*3;   dp[k3]  =  sp[k3] / ff;
        k3++;           dp[k3]  =  sp[k3] / ff;
        k3++;           dp[k3]  =  sp[k3] / ff;
      }
    }
  }
}



void
ixyFwd2 ( ppf dst, ppf wts, ppf srcA, ppf srcB, ppf foX, ppf foY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , unl method, dbl trans, dbl cfrac, dbl infimum, dbl initval)
{
  switch (method &= (O_SLIN | O_BLNR | O_BQDR | O_BCBC)) {
    case O_BLNR:
      rgbSmatBil2(dst,wts,srcA,srcB,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans,cfrac);
      break;
    /**********
    case O_BQDR:
      rgbSmatBiq2(dst,wts,srcA,srcB,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans,cfrac);
      break;
    case O_BCBC:
    case O_NERN:
      rgbSmatSin2(dst,wts,srcA,srcB,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans,cfrac);
      break;
    **********/
    default: die("ixyFwd2: NOIMP for interpolation method %d\n",method);
  }
  rgbNwtsFlt(dst,wts,dX,dY,dW,dH,infimum);
  rgbPlugFlt(dst,wts,dX,dY,dW,dH,infimum,initval);
}


void
ixyFwd2Trip
( ppf dst, ppf wts, ppf dsp, ppf wtp, ppf srcA, ppf srcB
, ppf foX, ppf foY, ppf raX, ppf raY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, unl method, dbl trans, dbl cfrac, dbl infimum, dbl initval, dbl over
, flt trip[][12], int numTri, unt flags
)
{
/*
static int first = 1;
*/
int     q;
flt     A[6], F[6];

  switch (method &= (O_SLIN | O_BLNR | O_BQDR | O_BCBC)) {
    case O_BLNR:
      rgbSmatBil2(dst,wts,srcA,srcB,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans,cfrac);
      break;
    default: die("ixyFwd2: noimp for interpolation method %d\n",method);
  }
  rgbNwtsFlt(dst,wts,dX,dY,dW,dH,infimum);

  for (q = 0; q < numTri; q++) {
    triOrder(A,F,trip[q]);
    if (Devopts & O_BLNR) 
      rgbSmatBil2Tri( dsp,wtp,srcA,srcB,raX,raY,dX,dY,dW,dH,sX,sY,sW,sH,trans,cfrac,over,A,F);
    else
      rgbSmatBil2Tri( dsp,wtp,srcA,srcB,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans,cfrac,over,A,F);
    rgbRepDstIfSrcFlt(dst,wts,dsp,wtp,dX,dY,dW,dH,infimum);
  }
  rgbPlugFlt(dst,wts,dX,dY,dW,dH,infimum,initval);

#if 0
  if (Devopts & O_TARG) {	/* -R */
    rgbPlugFlt(dst,wts,dX,dY,dW,dH,infimum,initval);
  }
#endif
}




