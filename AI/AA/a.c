/*****************************************************************************
 a.c    S.R.Lines    94.4.20
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
#include	<un/unBug.h>
#include	<un/unMacro.h>
#include	<un/unSys.h>
#include	<un/unMem.h>
#include	<un/unTypes.h>
#include	<libim.h>
#include	"rowSGI.h"
#include	<m/mj.h>
#include	"new.h"
#include	<SGI/image.h>

unt
readSgiFltBrd ( ppf ret[4], unt *dW, unt *dH, int *L, int *R, int *T, int *B
              , int oX, int oY, char *path, unt flags)
{
IMAGE   *sim;
ush     *rowBuf, *rowBeg;
unt     sW,sH,sD,fW,fH,  oW,oH;
int     yy, bY,bX, fX = -*L, fY = -*T;

  if ( ! (sim=iopen(path,"r")))
    die("readSgiFltBrd: iopen(%s) failed\n",path);

  sD = sim->zsize;
  sW = sim->xsize;
  sH = sim->ysize;
  if (sW <= (unt)oX || sH <= (unt)oY )
    die("readSgiFltBrd: start point %d %d not in %dx%d image %s",oX,oY,sW,sH,path);
  mallocAss(rowBuf,ush,sW);	/* need exactly this space */
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
  bX = oX - *L;  rowBeg = &rowBuf[bX];

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
    case  1: { pfl dp;
      ret[0] = rowMalFlt(fX,fY,fW,fH);
      /* ret[1] = NULL; */
      for (dp = &ret[0][-*T][-*L], yy = bY+oH; --yy >= bY ; dp += fW) {
        getrow(sim,rowBuf,yy,1);	/* Read the whole row...    */
        fltUsh(dp,rowBeg,oW);		/* Convert only part of it. */
      }
    } break;
#if 1
    case  3: { pfl red, grn, blu;
wrn("read case 3");
      ret[0] = rowMalFlt(fX,fY,fW,fH);
      ret[1] = rowMalFlt(fX,fY,fW,fH);
      ret[2] = rowMalFlt(fX,fY,fW,fH);
      ret[3] = rowMalFlt(fX,fY,fW,fH);
      red = &ret[1][-*T][-*L];
      grn = &ret[2][-*T][-*L];
      blu = &ret[3][-*T][-*L];

      for (yy = bY+oH; --yy >= bY ; red+=fW, grn+=fW, blu+=fW) {

        getrow(sim,rowBuf,yy,0);
        fltUsh(red,rowBeg,oW);
        getrow(sim,rowBuf,yy,1);	/* Read the whole row...    */
        fltUsh(grn,rowBeg,oW);		/* Convert only part of it. */
        getrow(sim,rowBuf,yy,2);
        fltUsh(blu,rowBeg,oW);
      }
      yy   = fW*oH;
      gryRgbFlt(ret[0],ret[1],ret[2],ret[3],fX,fY,fW,fH);
    } break;
#endif
    default: die("readSgiFltBrd [%s]: NOIMP for SGI image z: %u",__FILE__,sD);
  }
  free(rowBuf);
  iclose(sim);
  if      (sD == 3) return 4;
  else if (sD <= 1) return 1;
  else              return 0;
}


