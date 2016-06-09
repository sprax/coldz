/*****************************************************************************
 mjRead.c    S.R.Lines    94.4.20
#include	"mj.h"
 *****************************************************************************/
#include	<malloc.h>
#include	<un/unMem.h>
#include	<un/unBug.h>
#include	<SGI/image.h>
#include	<un/unTypes.h>
#include	<rowCnvt.h>
#include	<rowMake.h>
#include	<mj.h>
#include	<mjBord.h>
#include	<mjRead.h>

unt mjReadBordUnc ( ppu mcU[], pDmj D, char *path, unt flags)
{
  unt   z, w = D->w, h = D->h;
  int   l = D->l, r = D->r, t = D->t, b = D->b, x = D->x, y = D->y;

  /* @@ replace long arg lists below with struct ptrs */

  z = mjReadBordUncSgi(mcU,&w,&h,&l,&r,&t,&b,D->x,D->y,path,flags);
  D->w =  D->l + D->r + w;
  D->h =  D->t + D->b + h;
  /****
  wrn("FROM mjReadBordUnc:\
  mjBordUnc(mcU,%d,%d,%u,%u,%d,%d,%d,%d,%d,%d,%u,%u,%d,%d);"
        ,D->l,D->t,D->w,D->h,(x?x:l),(y?y:t),w+r,h+b,D->l,D->t,D->w,D->h,1,z);
  ****/

  mjBordUnc(mcU,D->l,D->t,D->w,D->h,(x?-x:l),(y?-y:t),w+r,h+b,D->l,D->t,D->w,D->h,1,z);
  D->x = -D->l;
  D->y = -D->t;
  return z;
}

unt
mjReadBordUncSgi ( ppu ret[]
, unt *dW, unt *dH, int *L, int *R, int *T, int *B
, int  oX, int  oY, char *path, unt flags)
{
IMAGE   *sim;
ush     *rowBuf, *rowBeg;
unt     sW,sH,sD,fW,fH,  oW,oH;
unt     yy, bY,bX, fX = -*L, fY = -*T;

  if ( ! (sim=iopen(path,"r")))
    die("mjReadBordUncSgi: iopen(%s) failed\n",path);

  sD = sim->zsize;
  sW = sim->xsize;
  sH = sim->ysize;
  if ((int)sW <= oX || (int)sH <= oY )
    die("mjReadBordUncSgi: start point %d %d not in %dx%d image %s",oX,oY,sW,sH,path);
  mallocAss(rowBuf,ush,sW);	/* need exactly this space */
  sW -= oX;
  sH -= oY;

  if ( *dW < 1 || *dW > sW ) *dW = sW;
  else if (*dW < sW)
	{ sW  = *dW; /*wrn("set sW:%d with oX:%d & *L:%d",sW,oX,*L);*/ }

  if ( *dH < 1 || *dH > sH )  *dH = sH;
  else if (*dH < sH)  
	{ sH  = *dH; /* wrn("set sH:%d with oY:%d & *T:%d",sH,oY,*T);*/ }

  fW = *dW + *L + *R;
  fH = *dH + *T + *B;

  yy = sW  -  *dW;		/* Already constrained >= 0 */
  if ((unt)*R > yy)  *R = yy;
  if (*L > oX)  *L = oX;
  oW = *L + *R + *dW;
  bX = oX - *L;  rowBeg = &rowBuf[bX];

  yy = sH  -  *dH;
  if ((unt)*B > yy)  *B = yy;
  if (*T > oY)  *T = oY;
  oH = *T + *B + *dH;
  bY = oY - *T;

 /***
  wrn("rd: *LRTBdWHoXY= %3d %3d %3d %3d %3u %3u %3d %3d",*L,*R,*T,*B,*dW,*dH,oX,oY);
  wrn("rd:  bXY osWosH= %3d %3d %3u %3u %3u %3u" ,bX,bY,oW,sW+oX,oH,sH+oY);
  ***/

  assert(*L >= 0 && *R >= 0 && *T >= 0  && *B >= 0 );
  assert(bX >= 0 && bY >= 0);
  assert(oW <= (unt)sim->xsize-bX && oH <= (unt)sim->ysize-bY);

  switch (sD) {
    case  1: { puc gry;
      ret[0] = rowMalUnc(fX,fY,fW,fH); /* ret[1] = NULL; */
#if 1
      for (fH = bY+oH, gry = &ret[0][-*T][-*L], yy = bY; yy < fH ; yy++, gry += fW) {
#else
      for (gry = &ret[0][-*T][-*L], yy = bY+oH; --yy >= bY ; gry += fW) {
#endif
        getrow(sim,rowBuf,yy,1);	    /* Read the whole row...    */
        uncUsh(gry,rowBeg,oW);              /* Convert only part of it. */
      }
    } break;

#if 0
      for (dp = &ret[0][-*T][-*L], yy = bY+oH; --yy >= bY ; dp += fW) {
        getrow(sim,rowBuf,yy,1);        /* Read the whole row...    */
#endif


    case  3: { puc red, grn, blu;
      ret[0] = rowMalUnc(fX,fY,fW,fH);
      ret[1] = rowMalUnc(fX,fY,fW,fH);
      ret[2] = rowMalUnc(fX,fY,fW,fH);
      red = &ret[0][-*T][-*L];
      grn = &ret[1][-*T][-*L];
      blu = &ret[2][-*T][-*L];
      for (fH = bY+oH, yy = bY; yy < fH; red+=fW, grn+=fW, blu+=fW, yy++) {
        getrow(sim,rowBuf,yy,0);
        uncUsh(red,rowBeg,oW);
        getrow(sim,rowBuf,yy,1);	/* Read the whole row...    */
        uncUsh(grn,rowBeg,oW);		/* Convert only part of it. */
        getrow(sim,rowBuf,yy,2);
        uncUsh(blu,rowBeg,oW);
      }
    } break;
    default: die("mjReadBordUncSgi [%s]: NOIMP for SGI image z: %u",__FILE__,sD);
  }
  free(rowBuf);
  iclose(sim);
  return sD;
}

