/*****************************************************************************
 rgbSmatAff.c < mxyFwd.c    S.R.Lines    93.9.24-94.6.5
 *****************************************************************************/
#include	<stdio.h>
#include	<assert.h>
#include	<math.h>
#include	<un/unBug.h>
#include	<un/unFlags.h>
#include	<un/unMath.h>
#include	<un/unMacro.h>
#include	<un/unTypes.h>
#include	<rowCnvt.h>
#include	<rowCopy.h>
#include	<rowMake.h>
#include	<rowNorm.h>
#include	<rowNwts.h>
#include	<rowPlug.h>
#include	<rowSpat.h>
#include	<rowSGI.h>
#include	<rowZero.h>
#include	<mj.h>
#include	<rowSmat.h>
/*
#include	"new.h"
*/

void affTrfPrint(AffTrf *a, char *tag)
{
  fprintf(stderr,"affTrfPrint: %s\n",tag);
  fprintf(stderr,"cosa %8.4f    sina %8.4f    scale %8.4f   angle %8.4f\n"
	,a->cosa,a->sina,a->scale,a->angle);
  fprintf(stderr,"xdst %8.4f    ydst  %8.4f   xsrc  %8.4f    ysrc %8.4f\n"
	,a->xdst,a->ydst,a->xsrc,a->ysrc);
}

/*___________________________________________________________________________*/

#if 1
void
rgbSmatAffBil2
( ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf fwdX, ppf fwdY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH, dbl trans, dbl clr
, AffTrf *aT
)
{
rgst int	  kT, jT;
rgst flt	  wT;
flt 	vR,vG,vB, wR,wG,wB;
int 	dx1 = dX + dW - 1;
int 	dy1 = dY + dH - 1;
int 	sx1 = sX + sW - 1;
int 	sy1 = sY + sH - 1;
int 	jA, kA, kB, jB, scanB = srcB[1] - srcB[0] - 2;
pfl 	pX, pY, pB;
flt 	xM, yM, xT, yT, xB, yB, cB = (flt)(clr), cA = 1.0F - cB;
flt 	a11 = aT->cosa*aT->scale,  a12 = -aT->sina*aT->scale;
flt 	a21 = aT->sina*aT->scale,  a22 =  aT->cosa*aT->scale;
flt 	tx  = aT->xdst,  ty  =  aT->ydst;
flt 	ux  = aT->xsrc,  uy  =  aT->ysrc;
flt 	xO  = 0.0F,      yO  =  0.0F;
flt     trs = (flt)(trans);

  rowZeroF(dstT,dX*3,dY,dW*3,dH);
  rowZeroF(dstW,dX  ,dY,dW  ,dH);
  
#define HACK 1
#if HACK
  xO = (ux - a11*tx - a12*ty + (tx - ux)*trs)*0.5F;
  yO = 0.0; 	/* uy - a21*tx - a22*ty; */
#else
  xO = ux - a11*tx - a12*ty + (tx - ux)*trs;
  yO = uy - a21*tx - a22*ty;
#endif

  for (jA = sY, fwdX += jA, fwdY += jA; jA <= sy1; jA++) {
    for (kA = sX, pX = *fwdX++ + kA, pY = *fwdY++ + kA; kA <= sx1; kA++) {

      xB = *pX++;
      yB = *pY++;

      xM = kA - trs*xB;
      yM = jA - trs*yB;
      xT = a11*xM  +  a12*yM + xO;
      yT = a21*xM  +  a22*yM + yO;

      jT = IFLOOR(yT);
      kT = IFLOOR(xT);

      if (dY <= jT && jT < dy1 && dX <= kT && kT < dx1) {
       
        yB  = jA  -  yB;
        jB  = IFLOOR(yB);
        xB  = kA  -  xB;
        kB  = IFLOOR(xB);

        /* NB: (kB,jB) is NOT always inbounds. */
        if (sX <= kB && kB < sx1 && sY <= jB && jB < sy1) {

          yB -= jB;
          xB -= kB;
          
          /* Assuming all rows in srcB are equally separated: */
          pB  =  srcB[jB]    + kB*3;
          wR  =  *pB++;
          wG  =  *pB++;
          wB  =  *pB++;
          wR += (*pB++ - wR) * xB;
          wG += (*pB++ - wG) * xB;
          wB += (*pB   - wB) * xB;
          pB +=  scanB;
          vR  =  *pB++;
          vG  =  *pB++;
          vB  =  *pB++;
          vR += (*pB++ - vR) * xB;
          vG += (*pB++ - vG) * xB;
          vB += (*pB   - vB) * xB;
          wR += ( vR   - wR) * yB;
          wG += ( vG   - wG) * yB;
          wB += ( vB   - wB) * yB;
          kB  = kA*3;
          vR  = wR*cB + cA*srcA[jA][kB++];
          vG  = wG*cB + cA*srcA[jA][kB++];
          vB  = wB*cB + cA*srcA[jA][kB  ];    /* kB will be reset */
        }
        else {
          kB = kA*3;
          vR = srcA[jA][kB  ];
          vG = srcA[jA][kB+1];
          vB = srcA[jA][kB+2];
        }

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
#endif


void
rowSmatAffBil2
( ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf fwdX, ppf fwdY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH, dbl trans, dbl clr
, AffTrf *aT
)
{
rgst int	  kT, jT;
rgst flt	  wT, vA;
int 	dx1 = dX + dW - 1;
int 	dy1 = dY + dH - 1;
int 	sx1 = sX + sW - 1;
int 	sy1 = sY + sH - 1;
int 	jA, kA, kB, jB, scanB = srcB[1] - srcB[0] - 1;
pfl 	pX, pY, pB;
flt 	xM, yM, xT, yT, xB, yB, cB = (flt)clr, cA = 1.0F - cB;
flt 	a11 = aT->cosa*aT->scale,  a12 = -aT->sina*aT->scale;
flt 	a21 = aT->sina*aT->scale,  a22 =  aT->cosa*aT->scale;
flt 	tx  = aT->xdst,  ty  =  aT->ydst;
flt 	ux  = aT->xsrc,  uy  =  aT->ysrc;
flt 	xO  = 0.0F,      yO  =  0.0F;
flt     trs = (flt)(trans);

  rowZeroF(dstT,dX,dY,dW,dH);
  rowZeroF(dstW,dX,dY,dW,dH);
  
  xO = ux - a11*tx - a12*ty + (tx - ux)*(1.0F - trs);
  yO = uy - a21*tx - a22*ty;

  for (jA = sY, fwdX += jA, fwdY += jA; jA <= sy1; jA++) {
    for (kA = sX, pX = *fwdX++ + kA, pY = *fwdY++ + kA; kA <= sx1; kA++) {

      xB = *pX++;
      yB = *pY++;

      xM = kA - trs*xB;
      yM = jA - trs*yB;
      xT = a11*xM  +  a12*yM + xO;
      yT = a21*xM  +  a22*yM + yO;

      jT = IFLOOR(yT);
      kT = IFLOOR(xT);

      if (dY <= jT && jT < dy1 && dX <= kT && kT < dx1) {
       
        yB  = jA  -  yB;
        jB  = IFLOOR(yB);
        xB  = kA  -  xB;
        kB  = IFLOOR(xB);

        /* NB: (kB,jB) is NOT always inbounds. */
        if (sX <= kB && kB < sx1 && sY <= jB && jB < sy1) {

          yB -= jB;
          xB -= kB;

          /* Assuming all rows in srcB are equally separated: */
          pB  =  srcB[jB]  + kB;
          wT  =  *pB++;
          wT += (*pB - wT) * xB;
          pB +=  scanB;
          vA  =  *pB++;
          vA += (*pB - vA) * xB;
          wT += ( vA - wT) * yB;
          assert(-0.5 < wT && wT < 255.5);
          vA = wT*cB + cA*srcA[jA][kA];
        }
        else
          vA = srcA[jA][kA];

        xT           -= kT;
        xB            = 1.0F - xT;
        yT           -= jT;
        yB            = 1.0F - yT;

        wT            = xB * yB;
        dstW[jT][kT] += wT;
        dstT[jT][kT] += wT * vA;

        kT++;
        wT            = xT * yB;
        dstW[jT][kT] += wT;
        dstT[jT][kT] += wT * vA;

        jT++;
        wT            = xT * yT;
        dstW[jT][kT] += wT;
        dstT[jT][kT] += wT * vA;

        kT--;
        wT            = xB * yT;
        dstW[jT][kT] += wT;
        dstT[jT][kT] += wT * vA;
      }
    }
  }
}


/*___________________________________________________________________________*/

void
rgbFwd2New
( ppf dst, ppf wts, ppf srcA, ppf srcB, ppf foX, ppf foY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, int iX, int iY, unt iW, unt iH
, AffTrf *aT, pDmj DD, pDmj SD
, int method, dbl trans, dbl cfrac, dbl infimum, dbl initval
)
{
/***
int 	j,z, chan=4;
***/

  switch (method &= (O_SLIN | O_BLNR | O_BQDR | O_BCBC)) {
    case O_BLNR:
      rgbSmatAffBil2(dst,wts,srcA,srcB,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans,cfrac,aT);
      break;
    default: die("rgbFwd2New: NOIMP for interpolation method %d\n",method);
  }
  rgbNwtsFlt(dst,wts,dX,dY,dW,dH,infimum);
  rgbPlugFlt(dst,wts,dX,dY,dW,dH,infimum,initval);
}

/*___________________________________________________________________________*/

void
gryFwd2New
( ppf dst, ppf wts, ppf srcA, ppf srcB, ppf foX, ppf foY
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, int iX, int iY, unt iW, unt iH
, AffTrf *aT, pDmj DD, pDmj SD
, int method, dbl trans, dbl cfrac, dbl infimum, dbl initval
)
{

  switch (method &= (O_SLIN | O_BLNR | O_BQDR | O_BCBC)) {
    case O_BLNR:
      rowSmatAffBil2(dst,wts,srcA,srcB,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans,cfrac,&aT[4]);
      break;
    default: die("gryFwd2New: NOIMP for interpolation method %d\n",method);
  }
  rowNwtsFlt( dst,wts,            dX,dY,dW,dH,infimum);
  rowPlugFlt( dst,wts,            dX,dY,dW,dH,infimum,initval);
}

/*___________________________________________________________________________*/
/*___________________________________________________________________________*/

void
affTrfFromTwoPts ( AffTrf *aT, Rexy *twoDstPts, Rexy *twoSrcPts
                 , dbl fracAngle, dbl fracTranslate)
{                                 /** fracTranslate not used **/
dbl     dst_dx, dst_dy, src_dx, src_dy;
dbl     dst_mag, src_mag, dd;

  dst_dx    = twoDstPts->x2 - twoDstPts->x1;
  dst_dy    = twoDstPts->y2 - twoDstPts->y1;
  src_dx    = twoSrcPts->x2 - twoSrcPts->x1;
  src_dy    = twoSrcPts->y2 - twoSrcPts->y1;

  dst_mag   = sqrt(dst_dx*dst_dx + dst_dy*dst_dy);
  src_mag   = sqrt(src_dx*src_dx + src_dy*src_dy);
  aT->scale = (flt)(src_mag/dst_mag);	/* @@ THIS HAS ALWAYS SEEMED WRONG... */
/*wrn("affTrfFromTwoPts: dst_mag:%8.4f   src_mag:%8.4f", dst_mag, src_mag);*/
  dd        = (dst_dx*src_dx + dst_dy*src_dy)/(dst_mag*src_mag);
  if          (dd >  1.0)  dd =  1.0;
  else if     (dd < -1.0)  dd = -1.0;
  aT->angle = (flt)(SIGN(dst_dx*src_dy - dst_dy*src_dx)*acos(dd) * fracAngle);
  aT->cosa  = (flt)cos(aT->angle);
  aT->sina  = (flt)sin(aT->angle);
  aT->xdst  = twoDstPts->x1 ;
  aT->ydst  = twoDstPts->y1 ;
  aT->xsrc  = twoSrcPts->x1 ;
  aT->ysrc  = twoSrcPts->y1 ;
/*wrn("affTrfFromTwoPts: fracAngle: %4.2f --> angle: %4.2f",fracAngle,aT->angle);*/
}

/*___________________________________________________________________________*/

ppu
matchEyeLoc ( ppu src, pDmj dD, pDmj sD, AffTrf *aT)
{
  ppu dst = rowMalUnc(dD->l,dD->t,dD->w,dD->h);
  rowWarpAff(dst,src,dD,sD,aT);
  /* AffTrfLbl(labels2,&aT,1.0,del); **** not needed */
  return dst;
}

/*___________________________________________________________________________*/

void
rowWarpAff (ppu dst, ppu src, pDmj dD, pDmj sD, AffTrf *aT)
{
puc     dp, sp;
int     j, k, ix, iy;
int     wP = sD->w+1;
int     wM = sD->w-1;
int     hM = sD->h-1;
int 	sW = &src[1][0] - &src[0][0];
int 	dW = dD->w + dD->l; 
int 	dH = dD->h + dD->t;
flt     fx, fy, mx, my, ox, oy;
dbl     a11, a12, a21, a22, tx, ty, ux, uy;
dbl 	bx, by, cx, cy;

  a11 =  aT->cosa*aT->scale;   a12 = -aT->sina*aT->scale;
  a21 =  aT->sina*aT->scale;   a22 =  aT->cosa*aT->scale;
  tx  =  aT->xdst;   ty  =  aT->ydst;
  ux  =  aT->xsrc;   uy  =  aT->ysrc;

  bx = ux - a11*tx - a12*ty;
  by = uy - a21*tx - a22*ty;
  for (j = dD->t; j < dH; j++) {
    cx = a12*j + bx;
    cy = a22*j + by;
    for (dp = dst[j], k = dD->l; k < dW; k++) {

      ox = (flt)(a11*k + cx);
      oy = (flt)(a21*k + cy);

      ix = IFLOOR(ox);
      iy = IFLOOR(oy);

      if (0 <= ix && ix < wM && 0 <= iy && iy < hM) {
        fx = ox   - ix;
        fy = oy   - iy;
        mx = 1.0F - fx;
        my = 1.0F - fy;
        sp = src[iy]+ix;
        *dp++ = (uch)(mx*my * *sp
              + fx*my *  sp[1]
              + mx*fy *  sp[sW]
              + fx*fy *  sp[wP] + 0.5F);
        /** if (0>dp[-1] || dp[-1]>255) warn("dst[%3d][%3d]= %d",j,k,dp[-1]); **/
      }
      else *dp++ = 0;
    }
  }
}

/*___________________________________________________________________________*/

void
rowTrfAffFlt (ppf dst, ppf src, pDmj dD, pDmj sD, AffTrf *aT)
{
pfl     dp, sp;
int     j, k, ix, iy;
int     wP = sD->w+1;
int     wM = sD->w-1;
int     hM = sD->h-1;
int 	sW = &src[1][0] - &src[0][0];
int 	dW = dD->w + dD->l; 
int 	dH = dD->h + dD->t;
flt     fx, fy, mx, my, ox, oy;
dbl     a11, a12, a21, a22, tx, ty, ux, uy;
dbl 	bx, by, cx, cy;

  a11 =  aT->cosa*aT->scale;   a12 = -aT->sina*aT->scale;
  a21 =  aT->sina*aT->scale;   a22 =  aT->cosa*aT->scale;
  tx  =  aT->xdst;   ty  =  aT->ydst;
  ux  =  aT->xsrc;   uy  =  aT->ysrc;

  bx = ux - a11*tx - a12*ty;
  by = uy - a21*tx - a22*ty;
  for (j = dD->t; j < dH; j++) {
    cx = a12*j + bx;
    cy = a22*j + by;
    for (dp = dst[j], k = dD->l; k < dW; k++) {

      ox = (flt)(a11*k + cx);
      oy = (flt)(a21*k + cy);

      ix = IFLOOR(ox);
      iy = IFLOOR(oy);

      if (0 <= ix && ix < wM && 0 <= iy && iy < hM) {
        fx = ox   - ix;
        fy = oy   - iy;
        mx = 1.0F - fx;
        my = 1.0F - fy;
        sp = src[iy]+ix;
        *dp++ = mx*my * *sp
              + fx*my *  sp[1]
              + mx*fy *  sp[sW]
              + fx*fy *  sp[wP];
        /** if (0>dp[-1] || dp[-1]>255) warn("dst[%3d][%3d]= %d",j,k,dp[-1]); **/
      }
      else *dp++ = 0;
    }
  }
}


/* 2D rotation and scale warping through 2 point correspondences */
void
warpRotScaleImg ( ppu dst, ppu src, int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, delEyeLoc *del
                , dbl fracAngle, dbl fracTrans)
{
AffTrf  aT;
Dmj 	dD,sD;
Rexy    dstR, srcR;

  dstR.x1 = del->nlx;
  dstR.y1 = del->nly;
  dstR.x2 = del->nrx;
  dstR.y2 = del->nry;

  srcR.x1 = del->olx;
  srcR.y1 = del->oly;
  srcR.x2 = del->orx;
  srcR.y2 = del->ory;

  affTrfFromTwoPts(&aT, &dstR, &srcR,fracAngle,fracTrans);

  DIM_PAR(dD,dX,dY,dW,dH,1);
  DIM_PAR(sD,sX,sY,sW,sH,1);

  rowWarpAff(dst,src,&dD,&sD,&aT);
}

void
rowResizeBilFlt(ppf dst, ppf src, pDmj dD, pDmj sD)
{
int 	ix,iy,j,k,sW = &src[1][0] - &src[0][0], sP = sW+1;
int 	dW = dD->l + dD->w, dH = dD->t + dD->h;
flt 	fx,fy,mx,my,rx,ry;
flt 	xScale = (flt)(sD->w)/(dD->w);
flt 	yScale = (flt)(sD->h)/(dD->h);
pfl 	dp,sp,p;

  for (j = dD->t; j < dH; j++)  {
    dp = dst[j] + dD->l;
    fy = yScale*j;
    iy = IFLOOR(fy);
    ry = fy - iy;
    my = 1.0F - ry;
    sp = src[iy] + sD->l;

    for (k = dD->l; k < dW; k++ ) {
      fx = xScale*k;
      ix = IFLOOR(fx);
      rx = fx - ix;
      mx = 1.0F - rx;
      p  = sp+ix;

      /*assert(0 <= ix && ix < xM && 0 <= iy && iy < yM);*/

      *dp++ = mx*my * *p
            + rx*my *  p[1]
            + mx*ry *  p[sW]
            + rx*ry *  p[sP] + 0.5F;
      /*if (0>dp[-1] || dp[-1]>255) warn("dst[%3d][%3d]= %d",j,k,dp[-1]);*/
    }
  }
}

