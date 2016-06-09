/*****************************************************************************
mjCopy.c    S.R.Lines (spraxlo@ai)    93.6.12-8.17
*****************************************************************************/
#include <string.h>	/* void *memcpy(void *, const void *, size_t); */
#include <un/unBug.h>
#include <un/unTypes.h>
#include "imagmem.h"
#include "mjCopy.h"
#undef	IRINTF		/* Return int nearest to float argument: want fast! */
#define IRINTF(f)  	((int)((f) < 0.0 ? ((f)-0.5F) : ((f)+0.5F)))

void mjCopyUF(ppf dst, ppu src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{
rgst int k;
rgst puc sp;
rgst pfl dp;
  for (dst += dY, src += sY; dH-- ; ) {
#if 0
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k >= 8; k -= 8) {
      *dp++ = (flt) *sp++;
      *dp++ = (flt) *sp++;
      *dp++ = (flt) *sp++;
      *dp++ = (flt) *sp++;
      *dp++ = (flt) *sp++;
      *dp++ = (flt) *sp++;
      *dp++ = (flt) *sp++;
      *dp++ = (flt) *sp++;
    }
#else
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k >= 8; k -= 8) {
      dp[0] = (flt) sp[0];
      dp[1] = (flt) sp[1];
      dp[2] = (flt) sp[2];
      dp[3] = (flt) sp[3];
      dp[4] = (flt) sp[4];
      dp[5] = (flt) sp[5];
      dp[6] = (flt) sp[6];
      dp[7] = (flt) sp[7];
      dp   += 8;
      sp   += 8;
    }
#endif
    while (k--)
      *dp++ = (flt) *sp++;
  }
}


void mjCopyFU(ppu dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{/* This version times faster on SGI/cc but not on SUN/gcc (which blows away acc) */
rgst int k;
rgst pfl sp;
rgst puc dp;
int      q = dW/8, r = dW % 8;

  for (dst += dY, src += sY; dH-- ; ) {
    for (dp = *dst++ + dX, sp = *src++ + sX, k = q; k--; ) {
      dp[0] = (unc)(sp[0] + 0.5F);
      dp[1] = (unc)(sp[1] + 0.5F);
      dp[2] = (unc)(sp[2] + 0.5F);
      dp[3] = (unc)(sp[3] + 0.5F);
      dp[4] = (unc)(sp[4] + 0.5F);
      dp[5] = (unc)(sp[5] + 0.5F);
      dp[6] = (unc)(sp[6] + 0.5F);
      dp[7] = (unc)(sp[7] + 0.5F);
      dp   += 8;
      sp   += 8;
    }
    k = r; while (k--)
      *dp++ = (unc)(*sp++ + 0.5F);
  }
}


void mjCopyFUold(ppu dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{/* This version times slower on SGI/cc but faster on  SUN/gcc) */
rgst int k;
rgst pfl sp;
rgst puc dp;
  for (dst += dY, src += sY; dH-- ; ) {
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k >= 8; k -= 8) {
      dp[0] = (unc)(sp[0] + 0.5F);
      dp[1] = (unc)(sp[1] + 0.5F);
      dp[2] = (unc)(sp[2] + 0.5F);
      dp[3] = (unc)(sp[3] + 0.5F);
      dp[4] = (unc)(sp[4] + 0.5F);
      dp[5] = (unc)(sp[5] + 0.5F);
      dp[6] = (unc)(sp[6] + 0.5F);
      dp[7] = (unc)(sp[7] + 0.5F);
      dp   += 8;
      sp   += 8;
    }
    while (k--)
      *dp++ = (unc)(*sp++ + 0.5F);
  }
}

void mjCopyFltToUnc (unc **dst, flt **src, unt dW, unt dH)
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



void mjCopyUncToFlt (flt **dst, unc **src, unt dW, unt dH)
{
  register unt  k;
  register flt *dp;
  register unc *sp;
  while (dH--) {
    for (dp = *dst++, sp = *src++, k = dW; k--; )
      *dp++ = (flt) *sp++;
  }
}




void mjCopyToUnc(unc **dst,void **src,unt dW,unt dH,int sX,int sY,unt srctype)
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
    default: die("mjCopyToUnc: NOIMP for src type %d",srctype);
  }
}

void mjCopyUnc(unc **dst,unc **src,unt dW, unt dH)
{ /* not much more overhead than the macro ROWCOPYFLT */
  for ( ; dH; dH--)
    memcpy((void*)*dst++,(void*)*src++,sizeof(unc)*dW);
}

void mjCopyUU(ppu dst, ppu src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{
rgst int k;
rgst puc sp;
rgst puc dp;
  for (dst += dY, src += sY; dH; dH--)
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k; k--)
      *dp++ = *sp++;
}


void mjCopyToFlt(flt **dst,void **src,unt dW,unt dH,int sX,int sY,unt srctype)
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
    default: die("mjCopyToFlt: NOIMP for src type %d",srctype);
  }
}

void mjCopyFlt(flt **dst,flt **src,unt dW, unt dH)
{ /* not much more overhead than the macro ROWCOPYFLT */
  for ( ; dH; dH--)
    memcpy((void*)*dst++,(void*)*src++,sizeof(flt)*dW);
}

void mjCopyFF(ppf dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{
rgst unt k;
rgst pfl sp;
rgst pfl dp;
  for (dst += dY, src += sY; dH; dH--)
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k; k--)
      *dp++ = *sp++;
}


void mjCopy(void **dst,void **src,unt dW,unt dH,unt dmmss,unt smmss)
{   /* Eventually either simplify mmss args or else use them in func. */
  unt dtype = dmmss & MM_TYPE;
  unt stype = smmss & MM_TYPE;

  switch(dtype) {
    case MM_CHR:
    case MM_UNC:
      mjCopyToUnc((unc**)dst,src,dW,dH,0,0,stype);
      break;
    case MM_FLT:
      mjCopyToFlt((flt**)dst,src,dW,dH,0,0,stype);
      break;
    default: die("mjCopy: NOIMP for dst type %d",dtype);
  }
}

