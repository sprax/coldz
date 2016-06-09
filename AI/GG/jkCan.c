/*****************************************************************************
 jkCan.c    S.R.Lines    93.9.24-94.6.5
 *****************************************************************************/
#include	<stdio.h>
#include	<assert.h>
#include	<math.h>
#include	<un/unBug.h>
#include	<un/unFlags.h>
#include	<un/unBug.h>
#include	<un/unMath.h>
#include	<un/unMacro.h>
#include	<un/unTypes.h>
#include	<m/mj.h>
#include	<m/rowCnvt.h>
#include	<m/rowCopy.h>
#include	<m/rowMake.h>
#include	<m/rowNorm.h>
#include	<m/rowNwts.h>
#include	<m/rowPlug.h>
#include	<m/rowSmat.h>
#include	<m/rowSpat.h>
#include	<m/rowSGI.h>
#include	<m/rowZero.h>
#include	<m/rowSmat.h>
#include	<m/mjMake.h>
#include	"inExtern.h"
#include	"jk.h"

#define XLEF	0.33333
#define XRIG	0.66667
#define ROT2	0.895
#define ROT4	0.91

extern double CropYF;

void
canonCropFace( ppu dst[], ppu src[], Dmj *sD, Dmj *iD
                , LblFace *dLbl, LblFace *sLbl)
{
int 	sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h, chan = sD->d;
int 	iX = iD->x, iY = iD->y, iW = iD->w, iH = iD->h;
flt 	fw = sW/(iW*2.0), fh = 120.0/300.0;
flt 	nx = sLbl->nox*sW, ny = sLbl->noy*sH;
int   	cX = (sLbl->nox - fw)*iW, cY = (sLbl->noy - CropYF)*iH;
int  	rX = cX + sW, rY = cY + sH, j;
  
/*
  prn("canonCropFace: sD: %d %d %u %u       iD: %d %d %u %u" 
	,sX,sY,sH,sW,  iX,iY,iW,iH);
  prn("canonCropFace: sLbl:  l_eye: %.5f  %.5f    r_eye: %.5f %.5f   cntr: %.5f %.5f"
	,sLbl->lex,sLbl->ley,sLbl->rex,sLbl->rey,sLbl->nox,sLbl->noy);
*/

  if (rX > iW || rY > iH) {
    wrn("canonCropFace: rX: %d > %d   or rY: %d > %d   to maximum of %d",rX,iW,rY,iH);
    if (rX > iW)  cX -= (rX - iW);
    if (rY > iH)  cY -= (rY - iH);
  }
  if (cX < 0 || cY < 0) {
    wrn("canonCropFace: cXY: %d %d  --  set to minimum of 0", cX, cY );
    if (cX < 0)  cX = 0;
    if (cY < 0)  cY = 0;
  }

  for (j = 0; j < chan; j++) {
    dst[j] = rowMalUnc(sX,sY,sW,sH);
    rowCopyUU(dst[j],src[j],sX,sY,sW,sH,cX,cY);
  }
}




void
canonEyeLocs ( AffTrf *aT, LblFace *dLbl, LblFace *sLbl
             , pDmj dD, pDmj sD, int pose, dbl fracAngle, unt flags)
{
dbl     dst_mag, src_mag, dd;
dbl     dst_dx, dst_dy, src_dx, src_dy;
dbl     diag, offd, xO,yO;

  dLbl->lex = XLEF;
  dLbl->rex = XRIG;

if (ICCV) {
  if (pose == 2) {
    dLbl->lex = XLEF;
    dLbl->rex = XLEF + (XRIG - XLEF)*ROT2;
  } else
  if (pose == 4) {
    dLbl->lex = XRIG - (XRIG - XLEF)*ROT4;
    dLbl->rex = XRIG;
  }
}

  if (flags) {
    dLbl->ley = 0.6500;
    dLbl->rey = 0.6500;
  } else {
    dLbl->ley = 0.3500;
    dLbl->rey = 0.3500;
  }
  dst_dx    = (dLbl->rex - dLbl->lex)*dD->w;
  dst_dy    = (dLbl->rey - dLbl->ley)*dD->h;
  src_dx    = (sLbl->rex - sLbl->lex)*sD->w;
  src_dy    = (sLbl->rey - sLbl->ley)*sD->h;

  dst_mag   = sqrt(dst_dx*dst_dx + dst_dy*dst_dy);
  src_mag   = sqrt(src_dx*src_dx + src_dy*src_dy);
  aT->scale = src_mag/dst_mag;

  wrd("canonEyeLocs: dst_mag:%8.4f   src_mag:%8.4f", dst_mag, src_mag);
  dd        = (dst_dx*src_dx + dst_dy*src_dy)/(dst_mag*src_mag);
  if          (dd >  1.0)  dd =  1.0;
  else if     (dd < -1.0)  dd = -1.0;
  aT->angle =  SIGN(dst_dx*src_dy - dst_dy*src_dx)*acos(dd) * fracAngle;
  aT->cosa  =  cos(aT->angle);
  aT->sina  =  sin(aT->angle);
  aT->xdst  =  dLbl->lex*dD->w;
  aT->ydst  =  dLbl->ley*dD->h;
#if 1   /* KLUDGE ? @@ 95.05.22  Monday  12:30 */
  aT->xsrc  =  sLbl->lex*sD->w;
  aT->ysrc  =  sLbl->ley*sD->h;
#else
  aT->xsrc  = (aT->scale*dLbl->lex - sLbl->lex)*sD->w;
  aT->ysrc  = (aT->scale*dLbl->ley - sLbl->ley)*sD->h;
#endif

  /*-------------------------------------------------------------------*/
  diag = cos(aT->angle);  /** /aT->scale; **/
  offd = sin(aT->angle);

  xO   = (sLbl->nox - sLbl->lex)*sD->w;
  yO   = (sLbl->noy - sLbl->ley)*sD->h;
  dLbl->nox = (( diag*xO + offd*yO)/aT->scale + dLbl->lex*dD->w)/dD->w;
  dLbl->noy = ((-offd*xO + diag*yO)/aT->scale + dLbl->ley*dD->h)/dD->h;
  xO   = (sLbl->mex - sLbl->lex)*sD->w;
  yO   = (sLbl->mey - sLbl->ley)*sD->h;
  dLbl->mex = (( diag*xO + offd*yO)/aT->scale + dLbl->lex*dD->w)/dD->w;
  dLbl->mey = ((-offd*xO + diag*yO)/aT->scale + dLbl->ley*dD->h)/dD->h;
  /***wrn("canonEyeLocs:O: %1.3f %1.3f    N: %1.3f %1.3f    meXY: %1.3f %1.3f"
     ,xO,yO,dLbl->nox,dLbl->noy,dLbl->mex,dLbl->mey); ****/
}

void
canonWarpExtract ( ppu dst[], ppu src[], pDmj dD, pDmj sD
                 , AffTrf *aT, LblFace *sLbl, unt flags)
{
puc     sp;
int     j, k, ix, iy;
unt     z;
int     wP = sD->w+1;
int     wM = sD->w-1;
int     hM = sD->h-1;
int     sW = &src[0][1][0] - &src[0][0][0];
int     dW = dD->w + dD->l;
int     dH = dD->h + dD->t;
flt     fx, fy, mx, my, ox, oy;
dbl     a11, a12, a21, a22, tx, ty;
dbl     bx, by, cx, cy;
/*
int	xN = sLbl->nox*sD->w;
int	yN = sLbl->noy*sD->h;
*/

  /*mjPrint(dD,"canonWarpExtract:dD");*/
  for (j = 0; j < dD->d; j++) {
    dst[j] = rowMalUnc(dD->x,dD->y,dD->w,dD->h);
  }

  a11 =  aT->cosa*aT->scale;   a12 = -aT->sina*aT->scale;
  a21 =  aT->sina*aT->scale;   a22 =  aT->cosa*aT->scale;
  tx  =  aT->xdst;   ty  =  aT->ydst;

  bx = sLbl->lex*sD->w - a11*tx - a12*ty;
  by = sLbl->ley*sD->h - a21*tx - a22*ty;

/**
warn("canonWarpExtract: t[%.3f %.3f] b[%.3f %.3f] sWH: %u %u j:%d %d k: %d %d\n\
    looking for %d %d"
     ,tx,ty,bx,by,sD->w,sD->h,dD->x,dW,dD->y,dH,xN,yN);
**/

  for (j = dD->y; j < dH; j++) {
    cx = a12*j + bx;
    cy = a22*j + by;
    for (k = dD->x; k < dW; k++) {

      ox = a11*k + cx;
      oy = a21*k + cy;

      ix = IFLOOR(ox);
      iy = IFLOOR(oy);
      /**
      if (ix == xN && iy == yN)
         fprintf(stderr,"YO, xy: %d %d hit from kj: %d %d, fracXY: %f %f\n"
		,xN,yN,k,j,(fx = (flt)k/dW), (fy = (flt)j/dH));
      **/

      if (0 <= ix && ix < wM && 0 <= iy && iy < hM) {
        fx = ox   - ix;
        fy = oy   - iy;
        mx = 1.0F - fx;
        my = 1.0F - fy;
        for (z = 0; z < dD->d; z++) {
          sp = src[z][iy]+ix;
          dst[z][j][k] = mx*my * *sp
                       + fx*my *  sp[1]
                       + mx*fy *  sp[sW]
                       + fx*fy *  sp[wP] + 0.5;
          /** if (0>dp[-1] || dp[-1]>255) warn("dst[%3d][%3d]= %d",j,k,dp[-1]); **/        }
      }
      else {
        for (z = 0; z < dD->d; z++)
          dst[z][j][k] = 0;
      }
    }
  }
}


void
canonExtractFace( ppu dst[], ppu src[], Dmj *dD, Dmj *sD, AffTrf *aT
                , LblFace *dLbl, LblFace *sLbl
                , int *cX, int *cY
		, int *dX, int *dY, unt *dW, unt *dH
		, int *sL, int *sT, int *sR, int *sB
		, unt flags, int pose)
{
int     oX, oY, gX, gY, q;
unt     oW, oH;
dbl	dx, dy, dist, ul_x, ul_y, lr_x, lr_y;
dbl 	aboveEyes=0.64, belowEyes=1.96;

  dx   = (dLbl->rex - dLbl->lex)* dD->w;
  dy   = (dLbl->rey - dLbl->ley)* dD->h;
  dist = sqrt(dx*dx + dy*dy);

#if 0
  ul_x = dLbl->lex*dD->w - dist*1.00;
  lr_x = dLbl->rex*dD->w + dist*1.00;
#else
  ul_x = (dLbl->lex + dLbl->rex - 1.0)*dD->w*0.5;
  lr_x = (dLbl->lex + dLbl->rex + 1.0)*dD->w*0.5;
#endif


  if (ICCV) {
    ul_x = 0;
    lr_x = dD->w;
  }

  /* prn("canonExtractFace: ul_x,lr_x:  %.3f %.3f",ul_x,lr_x); */

  if (InFill != ArgDbl[2])  {
    aboveEyes = ArgDbl[2];
    belowEyes = 2.6 - aboveEyes;
  }
  if (flags) {
    ul_y = (dLbl->ley - 0.6500)*dD->h;  /* eyeRexy->y1 - 0.3500*dD->h; */
    lr_y = (dLbl->rey + 0.3500)*dD->h;
  } else {
    ul_y = (dLbl->ley - 0.3500)*dD->h;  /* eyeRexy->y1 - 0.3500*dD->h; */
    lr_y = (dLbl->rey + 0.6500)*dD->h;
  }

  oX   = IRINT(ul_x);
  oY   = IRINT(ul_y);
  oW   = IRINT(lr_x - ul_x);
  oH   = IRINT(lr_y - ul_y);
  gX   = oX;
  gY   = oY;

  wrd("canonExtractFace: dist: %g    RAW oXY:  %d  %d", dist, oX,oY);

  *dX  = 0;	*sL  = *dX - gX;
  *dY  = 0;	*sT  = *dY - gY;
  *dW  = oW;	*sR  = (sD->x + sD->w) - (*dW + gX);
  *dH  = oH;	*sB  = (sD->y + sD->h) - (*dH + gY);

  oW   = sD->x + sD->w - *dW - 1;	/* Maximum allowable value of gX */
  q = gX;
  if (gX < sD->x) 
      gX = sD->x;
  if (gX > oW)    
      gX = oW;
  if (gX != q)
    warn("extractFace: gX:%d < 0 or gX > sX+sW-*dW-1:%d, clipped to %d",q,oW,gX);

  oH   = sD->y + sD->h - *dH - 1;	/* Maximum allowable value of gY */
  q = gY;
  if (gY < sD->y)  
      gY = sD->y;
  if (gY > oH)     
      gY = oH;
  if (gY != q)
    warn("extractFace: gY:%d < 0 or gY > sY+sH-*dH-1:%d, clipped to %d",q,oH,gY);

  dD->l = *cX = gX;
  dD->t = *cY = gY;
}


/*___________________________________________________________________________*/



void
aveCtrPnt ( dbl *xC, dbl *yC, pDmj dD, pDmj sD, pDmj iD
          , AffTrf *aT, LblFace *dLbl, LblFace *sLbl, dbl trsBeg, dbl trsEnd)
{
#if 0
unt     dW = dD->w, dH = dD->h, iW = iD->w, iH = iD->h;
dbl     bAx,bAy, bBx,bBy, cAx,cAy;
dbl     scale = 0.5*(aT[0].scale + aT[1].scale);
dbl     a11,a12,a21,a22, diag,offd, oX = 0.67, oY = 0.5;
dbl     xP, xA = (sLbl[0].nox - oX)*dW;
dbl     yP, yA = (sLbl[0].noy - oY)*dH;

  if (! (Options & O_CLIP))  scale = aT[0].scale;
  diag = cos(aT[0].angle)/scale;
  offd = sin(aT[0].angle)/scale;

  xP  =  xA*diag + yA*offd;
  xP  = -xA*offd + yA*diag;
  wrn("aveCtrPnt: xyP: %.3f %.3f",xP,yP);



  bAx = oX*dW - diag*sLbl[0].nox*dW - offd*sLbl[0].noy*dH;
  bAy = oY*dH + offd*sLbl[0].nox*dW - diag*sLbl[0].noy*dH;

  bBx = oX*dW - diag*sLbl[1].nox*dW - offd*sLbl[1].noy*dH;
  bBy = oY*dH + offd*sLbl[1].nox*dW - diag*sLbl[1].noy*dH;

  cAx = 0.5*(bAx + bBx);
  cAy = 0.5*(bAy + bBy);
  *xC = cAx;
  *yC = cAy;
  wrn("aveCtrPnt: bA: %.3f %.3f   bB: %.3f %.3f  xyC: %.3f %.3f"
        ,bAx,bAy,bBx,bBy,*xC,*yC);
#endif
}


