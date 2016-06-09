/*****************************************************************************
rowCopy.c    S.R.Lines (spraxlo@ai)    93.6.12-8.17
*****************************************************************************/
#include <string.h>	/* void *memcpy(void *, const void *, size_t); */
#include <un/unBug.h>
#include <un/unTypes.h>
#include "imagmem.h"
#include "m/rowCopy.h"

#undef	IRINTF		/* Return int nearest to float argument: want fast! */
#define IRINTF(f)  	((int)((f) < 0.0 ? ((f)-0.5F) : ((f)+0.5F)))

void
rgbCopyFU(ppu dst[4], ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{
rgst int k;
rgst pfl sp;
puc     *vr = dst[0], *vg = dst[1], *vb = dst[2], dr, dg, db;

  vr += dY; vg += dY; vb += dY; src += sY;

  for ( ; dH ; dH--) {
    dr = *vr++  + dX;
    dg = *vg++  + dX;
    db = *vb++  + dX;
    sp = *src++ + sX*3;
    for (k = dW; k; k--)  {
      *dr++ = (unc) *sp++;
      *dg++ = (unc) *sp++;
      *db++ = (unc) *sp++;
    }
  }
}



void rowCopy(void **dst,void **src,unt dW,unt dH,unt dmmss,unt smmss)
{   /* Eventually either simplify mmss args or else use them in func. */
  unt dtype = dmmss & MM_TYPE;
  unt stype = smmss & MM_TYPE;

  switch(dtype) {
    case MM_CHR:
    case MM_UNC:
      rowCopyToUnc((unc**)dst,src,dW,dH,0,0,stype);
      break;
    case MM_FLT:
      rowCopyToFlt((flt**)dst,src,dW,dH,0,0,stype);
      break;
    default: die("rowCopy: NOIMP for dst type %d",dtype);
  }
}


void rowCopyToUnc(unc **dst,void **src,unt dW,unt dH,int sX,int sY,unt srctype)
{
  register unc *dp;
  register unt k;

  src += sY;
  switch(srctype) {
    case MM_CHR:  			/* Treat char same as unchar */
    case MM_UNC: { ppu spp = (ppu)src;	/* Don't optimize this case to single memcpy */
      while (dH--)
        memcpy((void*)*dst++,(void*)(*spp++ + sX),dW);
    } break;
    case MM_SHT:  			/* Treat int16 same as uint16 */
    case MM_USH: { ush **spp = (ush**)src; register ush *sp;
      while (dH--)
        for (dp = *dst++, sp = *spp++ + sX, k = dW; k; k--)
          *dp++ = (unc) *sp++;
    } break;
    case MM_INT:  			/* Treat uint32 same as int32 */
    case MM_UNT: { int **spp = (int **)src; register int *sp;
      for (; dH; dH--)
        for (dp = *dst++, sp = *spp++ + sX, k = dW; k; k--)
          *dp++ = (unc) *sp++;
    } break;
    case MM_FLT: { ppf spp = (ppf)src; register flt *sp; register flt fv;
      for (; dH; dH--)
        for (dp = *dst++, sp = *spp++ + sX, k = dW; k; k--) {
          fv    = *sp++,
          *dp++ = IRINTF(fv);
        }
    } break;
    case MM_DBL: { dbl **spp = (dbl **)src; register dbl *sp; register flt fv;
      for (; dH; dH--)
        for (dp = *dst++, sp = *spp++ + sX, k = dW; k; k--) {
          fv    = (flt) *sp++,
          *dp++ = IRINTF(fv);
        }
    } break;
    default: die("rowCopyToUnc: NOIMP for src type %d",srctype);
  }
}

void rowCopyUnc(unc **dst,unc **src,unt dW, unt dH)
{ /* not much more overhead than the macro ROWCOPYFLT */
  for ( ; dH; dH--)
    memcpy((void*)*dst++,(void*)*src++,sizeof(unc)*dW);
}

void rowCopyUU(ppu dst, ppu src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{
rgst int k;
rgst puc sp;
rgst puc dp;
  for (dst += dY, src += sY; dH; dH--)
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k; k--)
      *dp++ = *sp++;
}

void rowCopyFU(ppu dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{
rgst int k;
rgst pfl sp;
rgst puc dp;
rgst flt fv;
  for (dst += dY, src += sY; dH; dH--)
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k; k--)
      fv    = *sp++,
      *dp++ = (unc) IRINTF(fv);
}

void rowCopyFltToUnc (unc **dst, flt **src, unt dW, unt dH)
{
  register unt  k;
  register unc *dp;
  register flt *sp;
  register flt  fv;
  for (; dH; dH--)
    for (dp = *dst++, sp = *src++, k = dW; k; k--)
      fv    = *sp++,
      *dp++ = IRINTF(fv);
}


void rowCopyToFlt(flt **dst,void **src,unt dW,unt dH,int sX,int sY,unt srctype)
{
  register flt *dp;
  register unt k;
    
  src += sY;
  switch(srctype) {
    case MM_FLT: { ppf spp = (ppf)src;	/* Don't optimize this case to single memcpy */
      for ( ; dH; dH--)
        memcpy((void*)*dst++,(void*)(*spp++ + sX),sizeof(flt)*dW);
    } break;
    case MM_CHR:  			/* Treat char same as unchar */
    case MM_UNC: { ppu spp = (ppu)src; register unc *sp;
      for (; dH; dH--)
        for (dp = *dst++, sp = *spp++ + sX, k = dW; k; k--)
          *dp++ = (flt) *sp++;
    } break;
    case MM_SHT:  			/* Treat int16 same as uint16 */
    case MM_USH: { ush **spp = (ush **)src; register ush *sp;
      for (; dH; dH--)
        for (dp = *dst++, sp = *spp++ + sX, k = dW; k; k--)
          *dp++ = (flt) *sp++;
    } break;
    case MM_INT:  			/* Treat uint32 same as int32 */
    case MM_UNT: { int **spp = (int **)src; register int *sp;
      for (; dH; dH--)
        for (dp = *dst++, sp = *spp++ + sX, k = dW; k; k--)
          *dp++ = (flt) *sp++;
    } break;
    case MM_DBL: { dbl **spp = (dbl **)src; register dbl *sp;
      for (; dH; dH--)
        for (dp = *dst++, sp = *spp++ + sX, k = dW; k; k--)
          *dp++ = (flt) *sp++;
    } break;
    default: die("rowCopyToFlt: NOIMP for src type %d",srctype);
  }
}

void rowCopyFlt(flt **dst,flt **src,unt dW, unt dH)
{ /* not much more overhead than the macro ROWCOPYFLT */
  for ( ; dH; dH--)
    memcpy((void*)*dst++,(void*)*src++,sizeof(flt)*dW);
}

void rowCopyFF(ppf dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{
rgst unt k;
rgst pfl sp;
rgst pfl dp;
  for (dst += dY, src += sY; dH; dH--)
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k; k--)
      *dp++ = *sp++;
}

void rowCopyFFIn(ppf dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{
rgst unt k;
rgst pfl sp;
rgst pfl dp;
  for (dst += (dY+dH-1), src += sY; dH--; )
    for (dp = *dst-- + dX, sp = *src++ + sX, k = dW; k; k--)
      *dp++ = *sp++;
}

void rowCopyUF(ppf dst, ppu src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{
rgst int k;
rgst puc sp;
rgst pfl dp;
  for (dst += dY, src += sY; dH; dH--)
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k; k--)
      *dp++ = (flt) *sp++;
}


void rowCopyUncToFlt (flt **dst, unc **src, unt dW, unt dH)
{
  register unt  k;
  register flt *dp;
  register unc *sp;
  while (dH--) {
    for (dp = *dst++, sp = *src++, k = dW; k--; )
      *dp++ = (flt) *sp++;
  }
}







