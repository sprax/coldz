/*****************************************************************************
 nin.c -- link with mainN.c    S.R.Lines    93.8.17-94.8.16
#define 	UNBUG 	0
#define 	UNTIME	0
 *****************************************************************************/
#include        <un/unFlags.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <stdlib.h>
#include        <m/mj.h>
#include        <m/fxyComb.h>
#include        <m/rowComb.h>
#include        <m/rowCopy.h>
#include        <m/rowDiff.h>
#include        <m/rowFlow.h>
#include        <m/rowMake.h>
#include        <m/rowNwts.h>
#include        <m/rowSmat.h>
#include        <m/rowSpat.h>
#include        <m/rowPlug.h>
#include        <m/rowWarp.h>
#include        <m/rowZero.h>
#include        <winLoop.h>
#include        <winDraw.h>
#include        "inExtern.h"
#include        "inParm.h"
#include        "mob.h"

void
rgbBil2 ( pMD wwD, flt clrs[])
{
static int first = 1;
ppf     dst = wwD->dst,  wts = wwD->wts, *sRGB = wwD->sRGB;
ppf    *fvX = wwD->fvX, *fvY = wwD->fvY;
ppf    *bvX = wwD->bvX, *bvY = wwD->bvY;
pDmj    dD  = wwD->dD;  int  dX = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;
pDmj    sD  = wwD->sD;  int  sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
ppf    *dRGB = wwD->dRGB;
int     j;
dbl     trs = clrs[1], omt = 1.0-trs;

  if (Options & O_BACK) {
    if (first) { prn("rgbBil2: I AM THE WAY BACK!");
      for (j = 0; j < 2; j++)
        dRGB[j] = rowCalFlt(dX*3,dY,dW*3,dH);
    }
    if (Devopts & O_BOTH) {
      if (first) prn("rgbBil2: I AM THE WAY BACK and BACK!");
      if (Options & O_ONLY)
        rgbWarpBilFlt( dst,    sRGB[0],bvX[1],bvY[1],dX,dY,dW,dH,sX,sY,sW,sH,trs);
      else if (Devopts & O_ONLY)
        rgbWarpBilFlt( dst,    sRGB[1],fvX[1],fvY[1],dX,dY,dW,dH,sX,sY,sW,sH, omt);
      else {
        rgbWarpBilFlt( dRGB[0],sRGB[0],bvX[1],bvY[1],dX,dY,dW,dH,sX,sY,sW,sH, trs);
        rgbWarpBilFlt( dRGB[1],sRGB[1],fvX[1],fvY[1],dX,dY,dW,dH,sX,sY,sW,sH, omt);
        rowComb2FF(dst,dRGB[0],dRGB[1],dX*3,dY,dW*3,dH,dX*3,dY,dW*3,dH,trs);
      }
    }
    else {
      if (Devopts & O_FWRD /* && trs < 0.5 */ )  {	/* inverse and forward */
        if (first) prn("rgbBil2: I AM THE WAY BACK and FORTH!");
        if (Options & O_ONLY)
          rgbWarpBilFlt( dst,    sRGB[0],bvX[1],bvY[1],dX,dY,dW,dH,sX,sY,sW,sH,trs);
        else if (Devopts & O_ONLY) {
          rgbSmatBil(    dst,wts,sRGB[1],bvX[1],bvY[1],dX,dY,dW,dH,sX,sY,sW,sH,omt);
          rgbNwtsFlt( dst,wts,dX,dY,dW,dH,InFill);
          rgbPlugFlt( dst,wts,dX,dY,dW,dH,InFill,127.5);
        }
        else {
          rgbWarpBilFlt( dRGB[0],sRGB[0],bvX[1],bvY[1],dX,dY,dW,dH,sX,sY,sW,sH, trs);
          rgbSmatBil(    dRGB[1],wts,sRGB[1],bvX[1],bvY[1],dX,dY,dW,dH,sX,sY,sW,sH,omt);
          rgbNwtsFlt(    dRGB[1],wts,dX,dY,dW,dH,InFill);
          rgbPlugFlt(    dRGB[1],wts,dX,dY,dW,dH,InFill,127.5);
          rowComb2FF(dst,dRGB[0],dRGB[1],dX*3,dY,dW*3,dH,dX*3,dY,dW*3,dH,trs);
        }
      }
      if (Options & O_ONLY)
        rgbWarpBilFlt( dst,    sRGB[0],bvX[1],bvY[1],dX,dY,dW,dH,sX,sY,sW,sH,trs);
      else if (Devopts & O_ONLY)
        rgbWarpBilFlt( dst,    sRGB[1],bvX[1],bvY[1],dX,dY,dW,dH,sX,sY,sW,sH,-omt);
      else {
        rgbWarpBilFlt( dRGB[0],sRGB[0],bvX[1],bvY[1],dX,dY,dW,dH,sX,sY,sW,sH, trs);
        rgbWarpBilFlt( dRGB[1],sRGB[1],bvX[1],bvY[1],dX,dY,dW,dH,sX,sY,sW,sH,-omt);
        rowComb2FF(dst,dRGB[0],dRGB[1],dX*3,dY,dW*3,dH,dX*3,dY,dW*3,dH,trs);
      }
    }
  }
  else {	/* forward warping */
    if (first)  prn("rgbBil2: I AM THE WAY FORWARD!");
    if (first && (Devopts & O_BOTH) && !(Options & O_ONLY))
      prn("rgbBil2: I AM THE WAY FORWARD AND FORWARD!");
    if (Devopts & O_PTRS) {
      if (Options & O_ONLY)
        rgbSpatBil2(dst,wts,wwD->sPre[0],wwD->sPre[0],fvX[1],fvY[1]
                   ,dX,dY,dW,dH,sX,sY,sW,sH,trs,trs);
      else if (Devopts & O_ONLY)
        rgbSpatBil2(dst,wts,wwD->sPre[1],wwD->sPre[1],fvX[1],fvY[1]
                   ,dX,dY,dW,dH,sX,sY,sW,sH,trs,trs);
      else
        rgbSpatBil2(dst,wts,wwD->sPre[0],wwD->sPre[1],fvX[1],fvY[1]
                   ,dX,dY,dW,dH,sX,sY,sW,sH,trs,trs);
    }
    else if (Options & O_ONLY)
      rgbSmatBil( dst,wts,sRGB[0],fvX[1],fvY[1],dX,dY,dW,dH,sX,sY,sW,sH,trs);
    else if (Devopts & O_BOTH && trs > 0.5) {
      if (Devopts & O_ONLY)
        rgbSmatBil( dst,wts,sRGB[1],bvX[1],bvY[1],dX,dY,dW,dH,sX,sY,sW,sH,omt);
      else
        rgbSmatBil2(dst,wts,sRGB[1],sRGB[0],bvX[1],bvY[1],dX,dY,dW,dH,sX,sY,sW,sH,omt,omt);
    }
    else {
      rgbSmatBil2(dst,wts,sRGB[0],wwD->sRGB[1],fvX[1],fvY[1],dX,dY,dW,dH,sX,sY,sW,sH,trs,trs);
    }
    rgbNwtsFlt( dst,wts,dX,dY,dW,dH,InFill);
    rgbPlugFlt( dst,wts,dX,dY,dW,dH,InFill,127.5);
  }
  first = 0;
}



void
rgbFwdBil4 ( pMD wwD, flt clrs[])
{
ppf     dst = wwD->dst;
ppf     wts = wwD->wts;
ppf    *fvX = wwD->fvX;
ppf    *fvY = wwD->fvY;
pDmj    dD  = wwD->dD;
pDmj    sD  = wwD->sD;
int     dX = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;
int     sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;

  fxyComb4(                       fvX,fvY,            sX,sY,sW,sH,clrs);

  if (Options & O_ONLY)
    rgbSmatBil(dst,wts,wwD->sRGB[0],*fvX,*fvY,dX,dY,dW,dH,sX,sY,sW,sH,1.0);
  else if (Devopts & O_PTRS)
    rgbSpatBil4(dst,wts,wwD->sPre,fvX,fvY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
  else
    rgbSmatBil4(dst,wts,wwD->sRGB,fvX,fvY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
  rgbNwtsFlt( dst,wts,dX,dY,dW,dH,InFill);
  rgbPlugFlt( dst,wts,dX,dY,dW,dH,InFill,127.5);
}

void
rgbBacBil4 ( pMD wwD, flt clrs[])
{
static  int first = 1;
ppf     dstF = wwD->dst;
ppf    *sRGB = wwD->sRGB;
ppf    *dRGB = wwD->dRGB;
ppf    *bvX = wwD->bvX;
ppf    *bvY = wwD->bvY;
pDmj    dD  = wwD->dD;  int  dX = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;
pDmj    sD  = wwD->sD;  int  sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
pDmj    fD  = wwD->fD;  int  fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
int j;

    static  ppf dvX[4], dvY[4];
    if (first) {  first = 0;
      prn("rgbBacBil4 -- YOU GOT ME BACK!");
      for (j = 0; j < 4; j++) {
        dvX[j]  = rowMalFlt(fX,fY,fW,fH);
        dvY[j]  = rowMalFlt(fX,fY,fW,fH);
      }
    }

  fxyComb4(bvX,bvY, dX,dY,dW,dH, clrs);

  for (j = 1; j < 4; j++) {
    rowDiffFlt(dvX[j],bvX[j],bvX[0],0,0,dW,dH);
    rowDiffFlt(dvY[j],bvY[j],bvY[0],0,0,dW,dH);
  }

  rgbWarpBilFlt(   dRGB[0],sRGB[0],bvX[0] ,bvY[0] ,0,0,dW,dH,sX,sY,sW,sH, 1.0);

  for (j = 1; j < 4; j++)
    rgbWarpBilFlt( dRGB[j],sRGB[j],dvX[j],dvY[j],0,0,dW,dH,sX,sY,sW,sH,-1.0);

  rowCombFlt4(dstF, dRGB, dX*3, dY, dW*3, dH, clrs);
}


#if 1
void
rgbBil4 ( pMD wwD, flt clrs[])
{
static  int first = 1;
ppf     dstF = wwD->dst;
ppf    *sRGB = wwD->sRGB;
ppf    *dRGB = wwD->dRGB;
ppf    *bvX = wwD->bvX, *bvY = wwD->bvY;
ppf    *fvX = wwD->fvX, *fvY = wwD->fvY;
pDmj    dD  = wwD->dD;  int  dX = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;
pDmj    sD  = wwD->sD;  int  sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
pDmj    fD  = wwD->fD;  int  fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
int 	j;

  if (Options & O_BACK) { 
    static  ppf dvX[4], dvY[4];
    if (first) {  first = 0;
      prn("rgbBil4 -- YOU GOT ME BACK!");
      for (j = 0; j < 4; j++) {
        dvX[j]  = rowMalFlt(fX,fY,fW,fH);
        dvY[j]  = rowMalFlt(fX,fY,fW,fH);
      }
    }
    fxyComb4(bvX,bvY, dX,dY,dW,dH, clrs);

    if (Devopts & O_BOTH) {
      if (first) prn("rgbBil4 -- I AM THE WAY BACK and BOTH!");

      if (Options & O_ONLY)
        rgbWarpBilFlt( dstF,    sRGB[0],bvX[0],bvY[0],dX,dY,dW,dH,sX,sY,sW,sH, 1.0);
      else if (Devopts & O_ONLY) {
        wrn("rgbBil4:BACK:BOTH:D-ONLY -- makes sense?");
        rgbWarpBilFlt( dstF,    sRGB[3],fvX[0],fvY[0],dX,dY,dW,dH,sX,sY,sW,sH,-1.0);
      }
      else {
        if (first) prn("rgbBil4 -- GET BACK GET BACK!");
        for (j = 1; j < 4; j++) {
          rowDiffFlt(dvX[j],bvX[j],bvX[0],0,0,dW,dH);
          rowDiffFlt(dvY[j],bvY[j],bvY[0],0,0,dW,dH);
        }
        rgbWarpBilFlt(   dRGB[0],sRGB[0],bvX[0],bvY[0],dX,dY,dW,dH,sX,sY,sW,sH, 1.0);
        for (j = 1; j < 4; j++)
          rgbWarpBilFlt( dRGB[j],sRGB[j],dvX[j],dvY[j],dX,dY,dW,dH,sX,sY,sW,sH,-1.0);
        rowCombFlt4(dstF, dRGB, dX*3, dY, dW*3, dH, clrs);
      }
    }
    else {
      for (j = 1; j < 4; j++) {
        rowDiffFlt(dvX[j],bvX[j],bvX[0],0,0,dW,dH);
        rowDiffFlt(dvY[j],bvY[j],bvY[0],0,0,dW,dH);
      }
      rgbWarpBilFlt(   dRGB[0],sRGB[0],bvX[0] ,bvY[0] ,0,0,dW,dH,sX,sY,sW,sH, 1.0);
      for (j = 1; j < 4; j++)
        rgbWarpBilFlt( dRGB[j],sRGB[j],dvX[j],dvY[j],0,0,dW,dH,sX,sY,sW,sH,-1.0);
      rowCombFlt4(dstF, dRGB, dX*3, dY, dW*3, dH, clrs);
    }
  }
  else  rgbFwdBil4(wwD,clrs);
}

#else

void
rgbBil4 ( pMD wwD, flt clrs[])
{
  if (Options & O_BACK)  rgbBacBil4(wwD,clrs);
  else                   rgbFwdBil4(wwD,clrs);
}
#endif



void
rgbFwdBil6 ( pMD wwD, flt clrs[])
{
ppf     dst  = wwD->dst;
ppf     wts  = wwD->wts;	/*  5 3 2  so  D E F  is nin B C E F A D */ 
ppf    *fvX = wwD->fvX;		/*  4 0 1      A B C                     */
ppf    *fvY = wwD->fvY;
ppf    *sPre = wwD->sPre, *sRGB = wwD->sRGB, sfP[4], sfS[4], sfX[4], sfY[4];
pDmj    dD   = wwD->dD;
pDmj    sD   = wwD->sD;
int     dX   = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;
int     sX   = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;

  sfP[0] = sPre[0], sfS[0] = sRGB[0], sfX[0] = fvX[0], sfY[0] = fvY[0];
  sfP[3] = sPre[3], sfS[3] = sRGB[3], sfX[3] = fvX[3], sfY[3] = fvY[3];
  if (clrs[6] < 0.0) {
    sfP[1] = sPre[4], sfP[2] = sPre[5];
    sfS[1] = sRGB[4], sfS[2] = sRGB[5];
    sfX[1] =  fvX[4], sfX[2] =  fvX[5];
    sfY[1] =  fvY[4], sfY[2] =  fvY[5];
  }
  else {
    sfP[1] = sPre[1], sfP[2] = sPre[2];
    sfS[1] = sRGB[1], sfS[2] = sRGB[2];
    sfX[1] =  fvX[1], sfX[2] =  fvX[2];
    sfY[1] =  fvY[1], sfY[2] =  fvY[2];
  }

  fxyComb4(sfX,sfY,sX,sY,sW,sH,clrs);
  if (Devopts & O_PTRS)
    rgbSpatBil4(dst,wts,sfP,sfX,sfY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
  else
    rgbSmatBil4(dst,wts,sfS,sfX,sfY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
  rgbNwtsFlt( dst,wts,dX,dY,dW,dH,InFill);
  rgbPlugFlt( dst,wts,dX,dY,dW,dH,InFill,127.5);
}

void
rgbFwdBil9 ( pMD wwD, flt clrs[])
{
ppf     dst  = wwD->dst;
ppf     wts  = wwD->wts;        /*  5 2 3  so  D E F  is nin B C E F A D */
ppf    *fvX = wwD->fvX; /*  4 0 1      A B C                     */
ppf    *fvY = wwD->fvY;
ppf    *sPre = wwD->sPre, *sRGB = wwD->sRGB, sfP[8], sfS[8], sfX[8], sfY[8];
pDmj    dD   = wwD->dD;
pDmj    sD   = wwD->sD;
int     dX   = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;
int     sX   = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;


  if (clrs[9] >= 0.0) {
    if (clrs[10] >= 0.0) {
      sfP[0] = sPre[0], sfS[0] = sRGB[0], sfX[0] = fvX[0], sfY[0] = fvY[0];
      sfP[1] = sPre[1], sfS[1] = sRGB[1], sfX[1] = fvX[1], sfY[1] = fvY[1];
      sfP[2] = sPre[2], sfS[2] = sRGB[2], sfX[2] = fvX[2], sfY[2] = fvY[2];
      sfP[3] = sPre[3], sfS[3] = sRGB[3], sfX[3] = fvX[3], sfY[3] = fvY[3];
    }
    else {
      sfP[0] = sPre[0], sfS[0] = sRGB[0], sfX[0] = fvX[0], sfY[0] = fvY[0];
      sfP[1] = sPre[1], sfS[1] = sRGB[1], sfX[1] = fvX[1], sfY[1] = fvY[1];
      sfP[2] = sPre[8], sfS[2] = sRGB[8], sfX[2] = fvX[8], sfY[2] = fvY[8];
      sfP[3] = sPre[7], sfS[3] = sRGB[7], sfX[3] = fvX[7], sfY[3] = fvY[7];
    }
  }
  else {
    if (clrs[10] >= 0.0) {
      sfP[0] = sPre[0], sfS[0] = sRGB[0], sfX[0] = fvX[0], sfY[0] = fvY[0];
      sfP[1] = sPre[5], sfS[1] = sRGB[5], sfX[1] = fvX[5], sfY[1] = fvY[5];
      sfP[2] = sPre[4], sfS[2] = sRGB[4], sfX[2] = fvX[4], sfY[2] = fvY[4];
      sfP[3] = sPre[3], sfS[3] = sRGB[3], sfX[3] = fvX[3], sfY[3] = fvY[3];
    }
    else {
      sfP[0] = sPre[0], sfS[0] = sRGB[0], sfX[0] = fvX[0], sfY[0] = fvY[0];
      sfP[1] = sPre[5], sfS[1] = sRGB[5], sfX[1] = fvX[5], sfY[1] = fvY[5];
      sfP[2] = sPre[6], sfS[2] = sRGB[6], sfX[2] = fvX[6], sfY[2] = fvY[6];
      sfP[3] = sPre[7], sfS[3] = sRGB[7], sfX[3] = fvX[7], sfY[3] = fvY[7];
    }
  }

  fxyComb4(sfX,sfY,sX,sY,sW,sH,clrs);
  if (Devopts & O_PTRS)
    rgbSpatBil4(dst,wts,sfP,sfX,sfY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
  else
    rgbSmatBil4(dst,wts,sfS,sfX,sfY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
  rgbNwtsFlt( dst,wts,dX,dY,dW,dH,InFill);
  rgbPlugFlt( dst,wts,dX,dY,dW,dH,InFill,127.5);
}


#if 0	/* 2-D version: 4 images at a time */
void
rgbFwdBil12 ( pMD wwD, flt clrs[])
{
ppf     dst  = wwD->dst;
ppf     wts  = wwD->wts;        /*  5 2 3  so  D E F  is nin B C E F A D */
ppf    *fvX = wwD->fvX; /*  4 0 1      A B C                     */
ppf    *fvY = wwD->fvY;
ppf    *sPre = wwD->sPre, *sRGB = wwD->sRGB, sfP[8], sfS[8], sfX[8], sfY[8];
pDmj    dD   = wwD->dD;
pDmj    sD   = wwD->sD;
int     dX   = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;
int     sX   = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;


  if (clrs[12] >= 0.0) {
    if (clrs[13] >= 2.0) {
      sfP[0] = sPre[ 3], sfS[0] = sRGB[ 3], sfX[0] = fvX[ 0], sfY[0] = fvY[ 0];
      sfP[1] = sPre[ 2], sfS[1] = sRGB[ 2], sfX[1] = fvX[11], sfY[1] = fvY[11];
      sfP[2] = sPre[11], sfS[2] = sRGB[11], sfX[2] = fvX[12], sfY[2] = fvY[12];
      sfP[3] = sPre[10], sfS[3] = sRGB[10], sfX[3] = fvX[13], sfY[3] = fvY[13];
    }
    else if (clrs[13] >= 0.0) {
      sfP[0] = sPre[0], sfS[0] = sRGB[0],  sfX[0] = fvX[0], sfY[0] = fvY[0];
      sfP[1] = sPre[1], sfS[1] = sRGB[1],  sfX[1] = fvX[1], sfY[1] = fvY[1];
      sfP[2] = sPre[2], sfS[2] = sRGB[2],  sfX[2] = fvX[2], sfY[2] = fvY[2];
      sfP[3] = sPre[3], sfS[3] = sRGB[3],  sfX[3] = fvX[3], sfY[3] = fvY[3];
    }
    else {
      sfP[0] = sPre[0], sfS[0] = sRGB[0], sfX[0] = fvX[0], sfY[0] = fvY[0];
      sfP[1] = sPre[1], sfS[1] = sRGB[1], sfX[1] = fvX[1], sfY[1] = fvY[1];
      sfP[2] = sPre[8], sfS[2] = sRGB[8], sfX[2] = fvX[8], sfY[2] = fvY[8];
      sfP[3] = sPre[7], sfS[3] = sRGB[7], sfX[3] = fvX[7], sfY[3] = fvY[7];
    }
  }
  else {
    if (clrs[13] >= 2.0) {
      sfP[0] = sPre[ 3], sfS[0] = sRGB[ 3], sfX[0] = fvX[ 0], sfY[0] = fvY[ 0];
      sfP[1] = sPre[ 4], sfS[1] = sRGB[ 4], sfX[1] = fvX[15], sfY[1] = fvY[15];
      sfP[2] = sPre[ 9], sfS[2] = sRGB[ 9], sfX[2] = fvX[14], sfY[2] = fvY[14];
      sfP[3] = sPre[10], sfS[3] = sRGB[10], sfX[3] = fvX[13], sfY[3] = fvY[13];
    }
    else if (clrs[13] >= 0.0) {
      sfP[0] = sPre[0], sfS[0] = sRGB[0], sfX[0] = fvX[0], sfY[0] = fvY[0];
      sfP[1] = sPre[5], sfS[1] = sRGB[5], sfX[1] = fvX[5], sfY[1] = fvY[5];
      sfP[2] = sPre[4], sfS[2] = sRGB[4], sfX[2] = fvX[4], sfY[2] = fvY[4];
      sfP[3] = sPre[3], sfS[3] = sRGB[3], sfX[3] = fvX[3], sfY[3] = fvY[3];
    }
    else {
      sfP[0] = sPre[0], sfS[0] = sRGB[0], sfX[0] = fvX[0], sfY[0] = fvY[0];
      sfP[1] = sPre[5], sfS[1] = sRGB[5], sfX[1] = fvX[5], sfY[1] = fvY[5];
      sfP[2] = sPre[6], sfS[2] = sRGB[6], sfX[2] = fvX[6], sfY[2] = fvY[6];
      sfP[3] = sPre[7], sfS[3] = sRGB[7], sfX[3] = fvX[7], sfY[3] = fvY[7];
    }
  }

  fxyComb4(sfX,sfY,sX,sY,sW,sH,clrs);
  if (Devopts & O_PTRS)
    rgbSpatBil4(dst,wts,sfP,sfX,sfY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
  else
    rgbSmatBil4(dst,wts,sfS,sfX,sfY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
  rgbNwtsFlt( dst,wts,dX,dY,dW,dH,InFill);
  rgbPlugFlt( dst,wts,dX,dY,dW,dH,InFill,127.5);
}

#else	/* 3-D version: 8 images at a time */

void
rgbFwdBil12 ( pMD wwD, flt clrs[])
{
ppf     dst  = wwD->dst;
ppf     wts  = wwD->wts;        /*  5 2 3  so  D E F  is nin B C E F A D */
ppf    *fvX = wwD->fvX; /*  4 0 1      A B C                     */
ppf    *fvY = wwD->fvY;
ppf    *sPre = wwD->sPre, *sRGB = wwD->sRGB, sfP[8], sfS[8], sfX[8], sfY[8];
pDmj    dD   = wwD->dD;
pDmj    sD   = wwD->sD;
int     dX   = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;
int     sX   = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;

  sfP[0] = sPre[0], sfS[0] = sRGB[0], sfX[0] = fvX[0], sfY[0] = fvY[0];
  sfP[1] = sPre[1], sfS[1] = sRGB[1], sfX[1] = fvX[1], sfY[1] = fvY[1];
  sfP[2] = sPre[2], sfS[2] = sRGB[2], sfX[2] = fvX[2], sfY[2] = fvY[2];
  sfP[3] = sPre[3], sfS[3] = sRGB[3], sfX[3] = fvX[3], sfY[3] = fvY[3];

  if (clrs[12] < 0.0) {
    sfP[4] = sPre[ 8], sfS[4] = sRGB[ 8], sfX[4] = fvX[ 8], sfY[4] = fvY[ 8];
    sfP[5] = sPre[ 9], sfS[5] = sRGB[ 9], sfX[5] = fvX[ 9], sfY[5] = fvY[ 9];
    sfP[6] = sPre[10], sfS[6] = sRGB[10], sfX[6] = fvX[10], sfY[6] = fvY[10];
    sfP[7] = sPre[11], sfS[7] = sRGB[11], sfX[7] = fvX[11], sfY[7] = fvY[11];
  }
  else {
    sfP[4] = sPre[ 4], sfS[4] = sRGB[ 4], sfX[4] = fvX[ 4], sfY[4] = fvY[ 4];
    sfP[5] = sPre[ 5], sfS[5] = sRGB[ 5], sfX[5] = fvX[ 5], sfY[5] = fvY[ 5];
    sfP[6] = sPre[ 6], sfS[6] = sRGB[ 6], sfX[6] = fvX[ 6], sfY[6] = fvY[ 6];
    sfP[7] = sPre[ 7], sfS[7] = sRGB[ 7], sfX[7] = fvX[ 7], sfY[7] = fvY[ 7];
  }

  fxyComb8(sfX,sfY,sX,sY,sW,sH,clrs);
  if (Devopts & O_PTRS)
    rgbSpatBil8(dst,wts,sfP,sfX,sfY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
  else
    rgbSmatBil8(dst,wts,sfS,sfX,sfY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
  rgbNwtsFlt( dst,wts,dX,dY,dW,dH,InFill);
  rgbPlugFlt( dst,wts,dX,dY,dW,dH,InFill,127.5);
}
#endif



void
rgbFwdBil8 ( pMD wwD, flt clrs[])
{
ppf     dst = wwD->dst;
ppf     wts = wwD->wts;
ppf    *fvX = wwD->fvX;
ppf    *fvY = wwD->fvY;
pDmj    dD  = wwD->dD;
pDmj    sD  = wwD->sD;
int     dX = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;
int     sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;

  fxyComb8(                       fvX,fvY,            sX,sY,sW,sH,clrs);
  if (Devopts & O_PTRS)
    rgbSpatBil8(dst,wts,wwD->sPre,fvX,fvY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
  else
    rgbSmatBil8(dst,wts,wwD->sRGB,fvX,fvY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
  rgbNwtsFlt( dst,wts,dX,dY,dW,dH,InFill);
  rgbPlugFlt( dst,wts,dX,dY,dW,dH,InFill,127.5);
}


void
rgbBacBil8 ( pMD wwD, flt clrs[])
{
static  int first = 1;
ppf     dstF = wwD->dst;
ppf    *sRGB = wwD->sRGB;
ppf    *dRGB = wwD->dRGB;
ppf    *bvX = wwD->bvX;
ppf    *bvY = wwD->bvY;
pDmj    dD  = wwD->dD;  int  dX = dD->x, dY = dD->y, dW = dD->w, dH = dD->h;
pDmj    sD  = wwD->sD;  int  sX = sD->x, sY = sD->y, sW = sD->w, sH = sD->h;
pDmj    fD  = wwD->fD;  int  fX = fD->x, fY = fD->y, fW = fD->w, fH = fD->h;
int j;

    static  ppf dvX[8], dvY[8];
    if (first) {  first = 0;
      prn("rgbBil8 -- YOU GOT ME BACK!");
      for (j = 0; j < 8; j++) {
        dvX[j]  = rowMalFlt(fX,fY,fW,fH);
        dvY[j]  = rowMalFlt(fX,fY,fW,fH);
      }
    }

  fxyComb8(bvX,bvY, dX,dY,dW,dH, clrs);

  for (j = 1; j < 8; j++) {
    rowDiffFlt(dvX[j],bvX[j],bvX[0],0,0,dW,dH);
    rowDiffFlt(dvY[j],bvY[j],bvY[0],0,0,dW,dH);
  }

  rgbWarpBilFlt(   dRGB[0],sRGB[0],bvX[0] ,bvY[0] ,0,0,dW,dH,sX,sY,sW,sH, 1.0);

  for (j = 1; j < 8; j++)
    rgbWarpBilFlt( dRGB[j],sRGB[j],dvX[j],dvY[j],0,0,dW,dH,sX,sY,sW,sH,-1.0);

  rowCombFlt8(dstF, dRGB, dX*3, dY, dW*3, dH, clrs);
}

void
rgbBil8 ( pMD wwD, flt clrs[])
{
  if (Options & O_BACK)  rgbBacBil8(wwD,clrs);
  else                   rgbFwdBil8(wwD,clrs);
}



