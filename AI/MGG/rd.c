/*****************************************************************************
 rd.c    S.R.Lines    94.4.20
 *****************************************************************************/
#define		UNTIME  0
#define		UNBUG	0
#include	<stddef.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>
#include	<errno.h>
#include	<math.h>
#include	<unBug.h>
#include	<unMacro.h>
#include	<unSys.h>
#include	<unMem.h>
#include	<unTypes.h>
#include	<im/libim.h>
#include	"rowSGI.h"
#include	<SGI/image.h>
#include 	<SGI/row_gutil.h>
#include	"new.h"

unt
readSgiFltBrdCp ( punl *ulpp, ppf ret[4], unt *dW, unt *dH
, int *L, int *R, int *T, int *B
, int oX, int oY, char *path, unt flags)
{
IMAGE   *sim;
ush     *redBuf, *grnBuf, *bluBuf, *alpBuf, *redBeg, *grnBeg, *bluBeg, *alpBeg;
unt     sW,sH,sD,fW,fH,  oW,oH;
int     yy, bY,bX, fX = -*L, fY = -*T;
punl	ulp;

  if ( ! (sim=iopen(path,"r")))
    die("readSgiFltBrd: iopen(%s) failed\n",path);

  *ulpp = (punl)malloc(sim->xsize*sim->ysize*sizeof(unsigned long));
  ulp = *ulpp + sim->xsize*(sim->ysize-1);

  mallocAss(redBuf,ush,sim->xsize);	/* need exactly this space */
  sD = sim->zsize;
  sW = sim->xsize;
  sH = sim->ysize;
  if (sW <= oX || sH <= oY )
    die("readSgiFltBrd: start point %d %d not in %dx%d image %s",oX,oY,sW,sH,path);
  sW -= oX;
  sH -= oY;

  if ( *dW < 1 || *dW > sW )  *dW = sW;
  if ( *dH < 1 || *dH > sH )  *dH = sH;
  fW = *dW + *L + *R;
  fH = *dH + *T + *B;

  yy = sW  -  *dW;		/* Already constrained >= 0 */
  *R = MIN(yy,*R);
  *L = MIN(oX,*L);
  oW = *L + *R + *dW;
  bX = oX - *L;
  redBeg = &redBuf[bX];

  yy = sH  -  *dH;
  *B = MIN(yy,*B);
  *T = MIN(oY,*T);
  oH = *T + *B + *dH;
  bY = oY - *T;

  wrn("rd: *LRTBdWHoXY= %3d %3d %3d %3d %3u %3u %3d %3d",*L,*R,*T,*B,*dW,*dH,oX,oY);
  wrn("rd:  bXY osWosH= %3d %3d %3u %3u %3u %3u" ,bX,bY,oW,sW+oX,oH,sH+oY);

  assert(*L >= 0 && *R >= 0 && *T >= 0  && *B >= 0 );
  assert(bX >= 0 && bY >= 0 && oW <= sW+oX && oH <= sH+oY);

  switch (sD) {
    case  1: { pfl gry;
      ret[0] = rowMalFlt(fX,fY,fW,fH);
      /* ret[1] = NULL; */
      for (gry = &ret[0][-*T][-*L], yy = bY+oH; --yy >= bY ; gry += fW, ulp -= oW) {
        getrow(sim,redBuf,yy,1);	/* Read the whole row...    */
        fltUsh(gry,redBeg,oW);		/* Convert only part of it. */
        bwtocpack( redBeg,ulp,oW);	/* @@ replace with my conventions... */
      }
    } break;

    case  3: { pfl red, grn, blu;
      mallocAss(grnBuf,ush,sim->xsize); grnBeg = &grnBuf[bX];
      mallocAss(bluBuf,ush,sim->xsize); bluBeg = &bluBuf[bX];
      ret[0] = rowMalFlt(fX,fY,fW,fH);
      ret[1] = rowMalFlt(fX,fY,fW,fH);
      ret[2] = rowMalFlt(fX,fY,fW,fH);
      ret[3] = rowMalFlt(fX,fY,fW,fH);
      red = &ret[1][-*T][-*L];
      grn = &ret[2][-*T][-*L];
      blu = &ret[3][-*T][-*L];

      for (yy = bY+oH ; --yy >= bY ; red+=fW, grn+=fW, blu+=fW, ulp -= oW) {
        getrow(sim,redBuf,yy,0);
        fltUsh(red,redBeg,oW);
        getrow(sim,grnBuf,yy,1);	/* Read the whole row...    */
        fltUsh(grn,grnBeg,oW);		/* Convert only part of it. */
        getrow(sim,bluBuf,yy,2);
        fltUsh(blu,bluBeg,oW);
        rgbtocpack(redBeg,grnBeg,bluBeg,ulp,oW);
      }
      yy   = fW*oH;
      gryRgbFlt(ret[0],ret[1],ret[2],ret[3],fX,fY,fW,fH);
    } break;

    case  4: { pfl red, grn, blu, alp;
      mallocAss(grnBuf,ush,sim->xsize); grnBeg = &grnBuf[bX];
      mallocAss(bluBuf,ush,sim->xsize); bluBeg = &bluBuf[bX];
      mallocAss(alpBuf,ush,sim->xsize); alpBeg = &alpBuf[bX];
      ret[0] = rowMalFlt(fX,fY,fW,fH);
      ret[1] = rowMalFlt(fX,fY,fW,fH);
      ret[2] = rowMalFlt(fX,fY,fW,fH);
      ret[3] = rowMalFlt(fX,fY,fW,fH);
      ret[4] = rowMalFlt(fX,fY,fW,fH);
      red = &ret[1][-*T][-*L];
      grn = &ret[2][-*T][-*L];
      blu = &ret[3][-*T][-*L];
      alp = &ret[4][-*T][-*L];

      for (yy = bY+oH ; --yy >= bY ; red+=fW, grn+=fW, blu+=fW, alp+=fW, ulp -= oW) {
        getrow(sim,redBuf,yy,0);
        fltUsh(red,redBeg,oW);
        getrow(sim,grnBuf,yy,1);        /* Read the whole row...    */
        fltUsh(grn,grnBeg,oW);          /* Convert only part of it. */
        getrow(sim,bluBuf,yy,2);
        fltUsh(blu,bluBeg,oW);
        getrow(sim,alpBuf,yy,3);
        fltUsh(alp,alpBeg,oW);
        rgbatocpack(redBeg,grnBeg,bluBeg,alpBeg,ulp,oW);
      }
      yy   = fW*oH;
      gryRgbFlt(ret[0],ret[1],ret[2],ret[3],fX,fY,fW,fH);
    } break;
    default: die("readSgiFltBrd [%s]: NOIMP for SGI image z: %u",__FILE__,sD);
  }
  free(redBuf);
  iclose(sim);
  if      (sD == 3) return 4;
  else if (sD <= 1) return 1;
  else              return 0;
}


