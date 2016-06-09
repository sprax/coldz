/******************************************************************************
rowSGI.c    S.R.Lines (spraxlo@ai.mit.edu)    93.10.12
******************************************************************************/
#undef	NDEBUG
#include        <stdlib.h>
#include        <unTypes.h>
#include        <unBug.h>
#include        <unMem.h>
#include        <unMath.h>
#include        <SGI/image.h>
#include        <m/imagmem.h>
#include        <m/rowCnvt.h>
#include        <m/rowMake.h>
#include        <m/rowSGI.h>
#ifdef MAC
#include	<m/rowFile.h>
#endif

unt	  /* don't invert data: y = 0 on bottom row (cartesian screen) */
sgiReadUn ( ppu *ret, unt *dW, unt *dH
          , int oX, int oY, unt mW, unt mH, char *path, unt flags)
{
IMAGE   *sgim;
ush     *rowbuf;
unt     sW,sH,sD;
int     y, yI;

  if ( ! (sgim=iopen(path,"r")))
    die("sgiReadUn: iopen(%s) failed\n",path);

  sW = sgim->xsize;
  sH = sgim->ysize;
  sD = sgim->zsize;

  yI = sizeof(unsigned short)*sW*sD;
  mallocAss(rowbuf,ush,yI);

  sW -= oX;  if (sW > mW)  sW = mW;  *dW = sW;
  sH -= oY;  if (sH > mH)  sH = mH;  *dH = sH;

  switch (sD) {
    case  1: { ppu dst;
      ret[0] = dst = rowMalUnc(0,0,sW,sH);
      /***** ret[1] = NULL; *****/
      for (yI = 0, y = oY; yI < sH ; y++, yI++)  {
        getrow(sgim,rowbuf,y,1);
        uncUsh(dst[yI],rowbuf+oX,sW);
      }
    } break;
    case  3: { ppu red, grn, blu;
      ret[0] = red = rowMalUnc(0,0,sW,sH);
      ret[1] = grn = rowMalUnc(0,0,sW,sH);
      ret[2] = blu = rowMalUnc(0,0,sW,sH);
      /***** ret[4] = NULL; *****/
      for (yI = 0, y = oY; yI < sH ; y++, yI++)  {
        getrow(sgim,   rowbuf, y, 0);
        uncUsh(red[yI],rowbuf+oX,sW);
        getrow(sgim,   rowbuf, y, 1);
        uncUsh(grn[yI],rowbuf+oX,sW);
        getrow(sgim,   rowbuf, y, 2);
        uncUsh(blu[yI],rowbuf+oX,sW);
      }
    } break;
    default: die("sgiReadUn [%s]: NOIMP for SGI image depth %u",__FILE__,sD);
  }
  free(rowbuf);
  iclose(sgim);
  return sD;
}


unt
sgiReadU( ppu *ret, unt *dW, unt *dH
	, int oX, int oY, unt mW, unt mH, char *path, unt flags)
{
IMAGE   *sgim;
ush     *rowbuf;
unt     sW,sH,sD;
int     y, yI;

  if ( ! (sgim=iopen(path,"r")))
    die("sgiReadU: iopen(%s) failed\n",path);

  sW = sgim->xsize;
  sH = sgim->ysize;
  sD = sgim->zsize;

  yI = sizeof(unsigned short)*sW*sD;
  mallocAss(rowbuf,ush,yI);

  sW -= oX;  if (sW > mW)  sW = mW;  *dW = sW;
  sH -= oY;  if (sH > mH)  sH = mH;  *dH = sH;

  switch (sD) {
    case  1: { ppu dst;
      ret[0] = dst = rowMalUnc(0,0,sW,sH);
      /***** ret[1] = NULL; *****/
      for (yI = sH, y = oY; yI ; y++) {
        getrow(sgim,rowbuf,y,1);
        uncUsh(dst[--yI],rowbuf+oX,sW);
      }
    } break;
    case  3: { ppu red, grn, blu;
      ret[0] = red = rowMalUnc(0,0,sW,sH);
      ret[1] = grn = rowMalUnc(0,0,sW,sH);
      ret[2] = blu = rowMalUnc(0,0,sW,sH);
      /***** ret[4] = NULL; *****/
      for (yI = sH, y = oY; yI-- ; y++) {
        getrow(sgim,   rowbuf, y, 0);
        uncUsh(red[yI],rowbuf+oX,sW);
        getrow(sgim,   rowbuf, y, 1);
        uncUsh(grn[yI],rowbuf+oX,sW);
        getrow(sgim,   rowbuf, y, 2);
        uncUsh(blu[yI],rowbuf+oX,sW);
      }
    } break;
    default: die("sgiReadU [%s]: NOIMP for SGI image depth %u",__FILE__,sD);
  }
  free(rowbuf);
  iclose(sgim);
  return sD;
}


int	/** replaced rowWriteSGIN **/
rowWritSGImU(ppu *srcU, int dX, int dY, unt dW, unt dH, unt chan, char *path)
{
ush     sbuf[1536];
ppu	srcp;
int     q,j,yI;
IMAGE   *sgim = iopen(path,"w",RLE(1),3,dW,dH,chan); 
  if (!sgim) {
    warn("rowWritSGImU: iopen failed");
    return -1;
  }
  for (q = 0; q < chan; q++) {
    for (srcp = &srcU[q][dY], j = dH, yI = dY; j-- ; ) {
      ushUnc(sbuf,*srcp++ + dX,dW);
      putrow(sgim,sbuf,j,q);
    }
  }
  return iclose(sgim);
}


int	/* DOES NOT FLIP IMAGE AROUND X-AXIS -- ORIGIN AT BOTTOM LEFT */
rowWritSgiU(ppu *srcU, int dX, int dY, unt dW, unt dH, unt chan, char *path)
{
ush     sbuf[1024];
ppu	srcp;
int     j,q;
IMAGE   *sgim = iopen(path,"w",RLE(1),3,dW,dH,chan);

  if (!sgim) {
    warn("rowWritSgiU: iopen failed");
    return -1;
  }
  for (q = 0; q < chan; q++) {
    for (srcp = &srcU[q][dY], j = 0; j < dH; j++) {
      ushUnc(sbuf,*srcp++ + dX,dW);
      putrow(sgim, sbuf,j,q);
    }
  }
  return iclose(sgim);
  /* 0:success, -1:stdlib close err (check errno), EOF:sgi err */
}


unt
readSgiUsh
( ppus ret[4], unt *dW, unt *dH, int oX, int oY, char *path, unt flags )
{
IMAGE   *sgim;
unt     sW,sH,sD;
size_t  rsize;
int     y, yI;

  if ( ! (sgim=iopen(path,"r")))
    die("readSgiUsh: iopen(%s) failed\n",path);
  sW = sgim->xsize;
  sH = sgim->ysize;
  sD = sgim->zsize;

  rsize = sizeof(unsigned short)*sW*sD;

  sH -= oY;  *dH = sH;
  sW -= oX;  *dW = sW;

  switch (sD) {
    case  1: { ppus dst;
      ret[0] = dst = rowMalUsh(0,0,sW,sH);
      ret[1] = NULL;
      for (yI = sH, y = oY; yI ; y++)
        getrow(sgim,dst[--yI],y,1);
    } break;
    case  3: { ppus gry, red, grn, blu;
      ret[0] = gry = rowMalUsh(0,0,sW,sH);
      ret[1] = red = rowMalUsh(0,0,sW,sH);
      ret[2] = grn = rowMalUsh(0,0,sW,sH);
      ret[3] = blu = rowMalUsh(0,0,sW,sH);
      for (yI = sH, y = oY; yI-- ; y++) {
        getrow(sgim,  red[yI], y, 0);
        getrow(sgim,  grn[yI], y, 1);
        getrow(sgim,  blu[yI], y, 2);
      }
      gryRgbUsh(gry,red,grn,blu,sW,sH);
    } break;
    default: die("readSgiUsh [%s]: NOIMP for SGI image depth %u",__FILE__,sD);
  }
  iclose(sgim);
  if      (sD == 3) return 4;
  else if (sD <= 1) return 1;
  else              return 0;
}



unt
sgiRead4U (ppu ret[4], unt *dW, unt *dH, int oX, int oY, char *path, unt flags)
{
IMAGE   *sgim;
ush 	*rowbuf;
unt     sW,sH,sD;
size_t	rsize;
int     y, yI;

  if ( ! (sgim=iopen(path,"r")))
    die("sgiRead4U: iopen(%s) failed\n",path);
  sW = sgim->xsize;
  sH = sgim->ysize;
  sD = sgim->zsize;

  rsize = sizeof(unsigned short)*sW*sD;
  if (! (rowbuf  = (unsigned short *)malloc(rsize)))
    die("sgiRead4U(%s): malloc(%u)",path,rsize);

  sH -= oY;  *dH = sH;
  sW -= oX;  *dW = sW;

  switch (sD) {
    case  1: { ppu dst;
      ret[0] = dst = rowMalUnc(0,0,sW,sH);
      ret[1] = NULL;
      for (yI = sH, y = oY; yI ; y++) {
        getrow(sgim,rowbuf,y,1);
        uncUsh(dst[--yI],rowbuf+oX,sW);
      }
    } break;
    case  3: { ppu gry, red, grn, blu;
      ret[0] = gry = rowMalUnc(0,0,sW,sH);
      ret[1] = red = rowMalUnc(0,0,sW,sH);
      ret[2] = grn = rowMalUnc(0,0,sW,sH);
      ret[3] = blu = rowMalUnc(0,0,sW,sH);
      /***** ret[4] = NULL; *****/
      for (yI = sH, y = oY; yI-- ; y++) {
        getrow(sgim,   rowbuf, y, 0);
        uncUsh(red[yI],rowbuf+oX,sW);
        getrow(sgim,   rowbuf, y, 1);
        uncUsh(grn[yI],rowbuf+oX,sW);
        getrow(sgim,   rowbuf, y, 2);
        uncUsh(blu[yI],rowbuf+oX,sW);
      }
      gryRgbUnc(gry,red,grn,blu,0,0,sW,sH);
    } break;
    default: die("sgiRead4U [%s]: NOIMP for SGI image depth %u",__FILE__,sD);
  }
  free(rowbuf);
  iclose(sgim);
  if      (sD == 3) return 4;
  else if (sD <= 1) return 1;
  else              return 0;
}

/************************************************************************/
/************************************************************************/
/************************************************************************/
/************************************************************************/
/************************************************************************/

void *
rowReadSGI(char *path, unt *sW, unt *sH, unt *deep, unt flags)
{
IMAGE   *sgim;
ush 	*rowbuf;
void	*ret=NULL;
unt     xsize, ysize, zsize, y, yI, type = flags & MM_TYPE;
size_t	rsize;

  if ( ! (sgim=iopen(path,"r")))
    die("rowReadSGI: iopen(%s) failed\n",path);
  *sW = xsize = sgim->xsize;
  *sH = ysize = sgim->ysize;
  *deep = zsize = sgim->zsize;
  rsize = sizeof(unsigned short)*xsize*zsize;
  if (! (rowbuf  = (unsigned short *)malloc(rsize)))
    die("rowReadSGI(%s): malloc(%u)",path,rsize);
  switch (type) {
    case MM_UNC: {
      unc **dst = rowMalUnc(0,0,xsize,ysize);
      ret = dst;
      for (yI = ysize, y = 0; yI ; y++) {
        getrow(sgim,rowbuf,y,1);
        uncUsh(dst[--yI],rowbuf,xsize);
      }
    } break;
    case MM_FLT: {
      flt **dst = rowMalFlt(0,0,xsize,ysize);
      ret = dst;
      for (yI = ysize, y = 0; yI ; y++) {
        getrow(sgim,rowbuf,y,1);
        fltUsh(dst[--yI],rowbuf,xsize);
      }
    } break;
    default: die("rowReadSGI(%s): NOIMP for flags:%d",path,flags);
  }
  free(rowbuf);
  iclose(sgim);
  return ret;
}

void
sgiWritFlt (ppf src, int sX, int sY, unt sW, unt sH, char *path)
{
  IMAGE *image;
  int k;
  unsigned short *rowp, *row;
  pfl srcp;

  if (! (image = iopen(path,"w",RLE(1),2,sW,sH)))
    die("sgiWritFlt: iopen(%s)", path);
  mallocAss(row,unsigned short,sW);
  for (src += sY; sH; ) {
    for (rowp = row, srcp = *src++ + sX, k = sW; k-- ; )
      *rowp++ = 0.5F + *srcp++;			/* Not IRINTF  */
    putrow(image,row,--sH,0);
  }
  iclose(image);
  free(row);
}



void
rowGraySGIn (ppu src, int sX, int sY, unt sW, unt sH, char *path)
{
  IMAGE *image;
  int j, k;
  unsigned short *rowp, *row;
  puc ucp;

  mallocAss(row,unsigned short,sW);

  if (! (image = iopen(path,"w",RLE(1),2,sW,sH)))
    die("rowGraySGI: iopen(%s)", path);

  for (src += sY, j = 0; j < sH; j++) {
    for (rowp = row, ucp = *src++ + sX, k = 0; k < sW; k++)
      *rowp++ = *ucp++;
    putrow(image,row,j,0);
  }
  iclose(image);
  free(row);
}


void
rowGraySGI (ppu src, int sX, int sY, unt sW, unt sH, char *path)
{
  IMAGE *image;
  int k;
  unsigned short *rowp, *row;
  puc ucp;

  mallocAss(row,unsigned short,sW);
  
  if (! (image = iopen(path,"w",RLE(1),2,sW,sH)))
    die("rowGraySGI: iopen(%s)", path);

  for (src += sY; sH; ) {
    for (rowp = row, ucp = *src++ + sX, k = 0; k < sW; k++)
      *rowp++ = *ucp++;
    putrow(image,row,--sH,0);
  }
  iclose(image);
  free(row);
}


void
rowWriteSGI(ppf src, int sX, int sY, unt sW, unt sH, char *path)
{
IMAGE   *sgim;
int     k,y,yI;
flt	*fp;
ush     *up, *urow;

  mallocAss(urow,unsigned short,sW);
  sgim = iopen(path,"w",RLE(1),2,sW,sH,1);
  for (y = 0, yI = sH+sY; yI > sY ; y++) {
    fp = &src[--yI][sX];
    for (up = urow, k = sW; k-- ; ) {    
      *up++ = 0.5F + *fp++;
    }
    putrow(sgim,urow,y,0);
  }
  iclose(sgim);
}

void
rgbWriteSGIn(ppf rgb, int dX, int dY, unt dW, unt dH, char *path)
{
IMAGE   *sgim;
int     k,y;
flt     *sp;
ush     *rp, *gp, *bp, rr[1280], gg[1280], bb[1280];

  assert(dW <= 1280);
  sgim = iopen(path,"w",RLE(1),3,dW,dH,3);  /* WHAT'S the 1st 3? */
  for (rgb += dY, dX *= 3, y = 0; y < dH; y++) {
    sp = *rgb++ + dX;
    for (rp = rr, gp = gg, bp = bb, k = dW; k; k--) {
      *rp++ = 0.5F + *sp++;
      *gp++ = 0.5F + *sp++;
      *bp++ = 0.5F + *sp++;
    }
    putrow(sgim,rr,y,0);
    putrow(sgim,gg,y,1);
    putrow(sgim,bb,y,2);
  }
  iclose(sgim);
}


void
rgbWriteSGI(ppf rgb, int dX, int dY, unt dW, unt dH, char *path)
{
IMAGE   *sgim;
int     k,y,yI;
flt	*sp;
ush     *rp, *gp, *bp, rr[1024], gg[1024], bb[1024];

  assert(dW <= 1024);
  sgim = iopen(path,"w",RLE(1),3,dW,dH,3);  /* WHAT'S the 1st 3? */
  for (dX *= 3, y = 0, yI = dH+dY; yI > dY ; y++) {
    sp = &rgb[--yI][dX];
    for (rp = rr, gp = gg, bp = bb, k = dW; k; k--) {    
      *rp++ = 0.5F + *sp++;
      *gp++ = 0.5F + *sp++;
      *bp++ = 0.5F + *sp++;
    }
    putrow(sgim,rr,y,0);
    putrow(sgim,gg,y,1);
    putrow(sgim,bb,y,2);
  }
  iclose(sgim);
}


void
sgiWritFB(ppf rgb[4], int dX, int dY, unt dW, unt dH, unt dD, char *path)
{
IMAGE   *sgim;
int     k,y,yI;
flt	*sr, *sg, *sb ;
pus     dr, dg, db, rr, gg, bb;

  mallocAss(rr,ush,dW);
  mallocAss(gg,ush,dW);
  mallocAss(bb,ush,dW);

  wrn("sgiWritFB dD: %u  path: %s", dD,path);

  if (dD < 3 || rgb[1] == NULL) {
    sgiWritFlt(rgb[0],dX,dY,dW,dH,path);
    return;
  }
  sgim = iopen(path,"w",RLE(1),3,dW,dH,3);  /* WHAT'S the 1st 3? */
  for (y = 0, yI = dH+dY; yI > dY ; y++) {
    sr = &rgb[1][--yI][dX];
    sg = &rgb[2][  yI][dX];
    sb = &rgb[3][  yI][dX];
    for (dr = rr, dg = gg, db = bb, k = dW; k; k--) {    
      *dr++ = 0.5F + *sr++;
      *dg++ = 0.5F + *sg++;
      *db++ = 0.5F + *sb++;
    }
    putrow(sgim,rr,y,0);
    putrow(sgim,gg,y,1);
    putrow(sgim,bb,y,2);
  }
  iclose(sgim); free(rr); free(gg); free(bb);
}

void
sgiWritUB(ppu rgb[4], unt dW, unt dH, char *path)
{
IMAGE   *sgim;
int     k,y,yI;
unc	*sr, *sg, *sb ;
ush     *dr, *dg, *db, rr[512], gg[512], bb[512];

  assert(dW <= 512);
  sgim = iopen(path,"w",RLE(1),3,dW,dH,3);  /* WHAT'S the 1st 3? */
  for (y = 0, yI = dH; yI ; y++) {
    sr = rgb[1][--yI];
    sg = rgb[2][  yI];
    sb = rgb[3][  yI];
    for (dr = rr, dg = gg, db = bb, k = dW; k; k--) {    
      *dr++ = *sr++;
      *dg++ = *sg++;
      *db++ = *sb++;
    }
    putrow(sgim,rr,y,0);
    putrow(sgim,gg,y,1);
    putrow(sgim,bb,y,2);
  }
  iclose(sgim);
}





void
writSgiUsh(ppus rgb[4], unt dW, unt dH, char *path)
{
IMAGE   *sgim;
unt     y,yI;

  sgim = iopen(path,"w",RLE(1),3,dW,dH,3);  /* WHAT'S the 1st 3? */
  for (y = 0, yI = dH; yI-- ; y++) {
    putrow(sgim,rgb[1][yI],y,0);
    putrow(sgim,rgb[2][yI],y,1);
    putrow(sgim,rgb[3][yI],y,2);
  }
  iclose(sgim);
}


