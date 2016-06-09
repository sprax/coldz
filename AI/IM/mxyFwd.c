/*****************************************************************************
 mxyFwd.c    S.R.Lines    93.9.24-11.30
 *****************************************************************************/
#include	<un/unBug.h>
#include	<un/unFlags.h>
#include	<un/unMacro.h>
#include	<un/unTypes.h>
#include	<rowClip.h>
#include	"rowCopy.h"
#include	"rowNorm.h"
#include	"rowNwts.h"
#include	"rowPlug.h"
#include	<mj.h>
#include	<rowSmat.h>
#include	<rowSpat.h>
#include	<mxyFwd.h>

void
ixyFwd3 ( ppf dst, ppf wts, ppf srcA, ppf srcB, ppf srcC
        , ppf *voX, ppf *voY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , unl method, dbl trs, flt *cfrac, dbl infimum, dbl initval)
{
  switch (method &= (O_SLIN | O_BLNR | O_BQDR | O_BCBC)) {
    case O_BLNR:
      rgbSmatBil3(dst,wts,srcA,srcB,srcC,voX,voY
                 ,dX,dY,dW,dH,sX,sY,sW,sH,trs,cfrac,infimum);
      break;
    default: die("ixyFwd3: NOIMP for interpolation method %d\n",method);
  }
  if (! (Options & O_UNCH)) {
    rgbNwtsFlt(dst,wts,dX,dY,dW,dH,infimum);
    rgbPlugFlt(dst,wts,dX,dY,dW,dH,infimum,initval);
  }
  /**************************/
}

void
rgbFwd4 ( ppf dst, ppf wts, ppf *src, ppf *fvX, ppf *fvY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , unl method, flt *clrs, dbl infimum, dbl initval)
{
  switch (method &= (O_SLIN | O_BLNR | O_BQDR | O_BCBC)) {
    case O_BLNR:
      rgbSpatBil4(dst,wts,src,fvX,fvY,dX,dY,dW,dH,sX,sY,sW,sH,clrs,1.0);
      break;
    default: die("rgbFwd4: NOIMP for interpolation method %d\n",method);
  }
  rgbNwtsFlt(dst,wts,dX,dY,dW,dH,infimum);
  rgbPlugFlt(dst,wts,dX,dY,dW,dH,infimum,initval);
}



#if 00 /* same as rgbSmatBil.c ??? */
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
#endif

void
ixyFwd1 ( ppf dst, ppf wts, ppf srcA, ppf foX, ppf foY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , unl method, dbl trans, dbl infimum, dbl initval)
{
  switch (method &= (O_SLIN | O_BLNR | O_BQDR | O_BCBC)) {
    case O_BLNR:
      rgbSmatBil(dst,wts,srcA,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans);
      break;
    /**********
    case O_BQDR:
      rgbSmatBiq2(dst,wts,srcA,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans);
      break;
    case O_BCBC:
    case O_NERN:
      rgbSmatSin2(dst,wts,srcA,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans);
      break;
    **********/
    default: die("ixyFwd1: NOIMP for interpolation method %d\n",method);
  }
  rgbNwtsFlt(dst,wts,dX,dY,dW,dH,infimum);
  rgbPlugFlt(dst,wts,dX,dY,dW,dH,infimum,initval);
}


void
mxyFwd2 ( ppf dst, ppf wts, ppf srcA, ppf srcB, ppf foX, ppf foY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , unl method, dbl trans, dbl cfrac, dbl infimum, dbl initval)
{
  switch (method &= (O_SLIN | O_BLNR | O_BQDR | O_BCBC)) {
    case O_SLIN:
      rowSmatSin2(dst,wts,srcA,srcB,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans,cfrac);
      break;
    case O_BLNR:
      rowSmatBil2(dst,wts,srcA,srcB,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans,cfrac);
      break;
    case O_BQDR:
      rowSmatBiq2(dst,wts,srcA,srcB,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans,cfrac);
      break;
    case O_BCBC:
    default: die("warpFwd2: NOIMP for interpolation method %d\n",method);
  }
  rowNwtsFlt( dst,wts,            dX,dY,dW,dH,infimum);
  rowPlugFlt( dst,wts,            dX,dY,dW,dH,infimum,initval);
}



void
mxyFwd2g4 ( ppu duQ, ppf dfT, ppf dwT, ppf sfA, ppf sfB, ppf foX, ppf foY
          , int dX, int dY, unt dW, unt dH, int sX, int sY, unt sW, unt sH
          , dbl trs, dbl clr, dbl inFill)
{
int 	cTx,cTy,hTx,hTy,oTx,oTy, pvim;

  if (Options &  O_MORE)
      Options |=     O_HITS|O_HOLE|O_NONO;
  switch (Options & (O_HITS|O_HOLE|O_NONO)) {
    case O_NONO:	cTx = dW, cTy = 0;  break;
    case O_HITS:	hTx = dW, hTy = 0;  break;
    case O_HOLE:	oTx = dW, oTy = 0;  break;
    case O_HITS|O_HOLE|O_NONO:
    case O_NONO|O_HITS:
    case O_NONO|O_HOLE:
    case O_HITS|O_HOLE:
      cTx = dW, cTy = 0, hTx = 0, hTy = dH, oTx = dW, oTy = dH;
      break;
  }
  switch (pvim = (Options & (O_BLNR|O_BQDR|O_BCBC|O_NERN))) {
    case O_BLNR:
      rowSmatBil2(dfT,dwT,sfA,sfB,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trs,clr);
      break;
    default: die("mxyFwd2g4: NOIMP for pvim:%d\n",pvim);
  }
  if (Options & O_HOLE)
    rowNormFU(duQ,dfT,dwT,oTx,oTy,dW,dH,dX,dY,dW,dH,inFill);
  if (Options & O_HITS)
    rowMuCpFU(duQ,dwT,    hTx,hTy,dW,dH,dX,dY,dW,dH,255.0/3.0,0.5,254.5);
  if (Options & O_NONO)
    rowMuCpFU(duQ,dfT,    cTx,cTy,dW,dH,dX,dY,dW,dH,0.3333333,0.5,254.5);
  rowNwtsFlt( dfT,dwT,                  dX,dY,dW,dH,inFill);
  rowPlugFlt( dfT,dwT,                  dX,dY,dW,dH,inFill,127.0);
  rowCopyFU(  duQ,dfT,    000,000,dW,dH,dX,dY);
}

void
mxyFwd  ( ppf dst, ppf wts, ppf src, ppf foX, ppf foY
        , int dX, int dY, unt dW, unt dH
        , int sX, int sY, unt sW, unt sH
        , unl method, dbl trans, dbl infimum, dbl initval)
{
  switch (method &= (O_SLIN | O_BLNR | O_BQDR | O_BCBC)) {
    case O_SLIN:	/* singular pixval placement, no interpolation */
      rowSmatSin( dst,wts,src,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans);
      break;
    case O_BLNR:	/* bilinear pixval interpolation */
      rowSmatBil( dst,wts,src,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans);
      break;
    case O_BQDR:	/* biquadratic */
      rowSmatBiq( dst,wts,src,foX,foY,dX,dY,dW,dH,sX,sY,sW,sH,trans);
      break;
    case O_BCBC:	/* bicubic */
    default: die("mxyFwd: NOIMP for interpolation method %d\n",method);
  }
  rowNwtsFlt( dst,wts,            dX,dY,dW,dH,infimum);
  rowPlugFlt( dst,wts,            dX,dY,dW,dH,infimum,initval);
}



