/******************************************************************************
rowDiff.c    S.R.Lines (spraxlo@ai.mit.edu)    93.12.7
Pixelwise difference between two images stored as arrays of pointers to rows.
******************************************************************************/
#include	<un/unTypes.h>
#include	"imagmem.h"
#include	"rowZero.h"
#include	"rowDiff.h"
void die(const char *fmt, ...);
 
void
rowAddFlt (ppf dst, ppf src, ppf add, int xi, int yi, unt dW, unt dH)
{
  register flt *dp, *sp, *sb, *dpEnd;

  if (dst != src)
    for (dst += yi, src += yi, add += yi; dH--; )
      for (dp = xi+*dst++, sp = xi+*src++, sb = xi+*add++, dpEnd=dp+dW; dp<dpEnd; )
        *dp++ = *sp++ + *sb++;
  else
    for (dst += yi, add += yi; dH--; )
      for (dp = xi+*dst++, sb = xi+*add++, dpEnd=dp+dW; dp<dpEnd; )
        *dp++ += *sb++;
}


void
rowDiff(void *dst,void *src,void *sub,int xi,int yi,unt dW,unt dH,unt mmss)
{
  unt ctype = mmss & MM_TYPE;
  switch(ctype) {
    case MM_CHR:
    case MM_UNC:
      rowDiffUnc(dst,src,sub,xi,yi,dW,dH);  break;
    case MM_FLT:
      rowDiffFlt(dst,src,sub,xi,yi,dW,dH);  break;
    default: die("rowDiff: NOIMP for ctype %d\n",ctype);
  }
}


void
rowDiffFlt (ppf dst, ppf src, ppf sub, int xi, int yi, unt dW, unt dH)
{
  register flt *dp, *sp, *sb, *dpEnd;
  if (src == sub)  rowZeroF(dst,xi,yi,dW,dH);
  else if (dst != src)
    for (dst += yi, src += yi, sub += yi; dH--; )
      for (dp = xi+*dst++, sp = xi+*src++, sb = xi+*sub++, dpEnd=dp+dW; dp<dpEnd; )
        *dp++ = *sp++ - *sb++;
  else
    for (dst += yi, sub += yi; dH--; )
      for (dp = xi+*dst++, sb = xi+*sub++, dpEnd=dp+dW; dp<dpEnd; )
        *dp++ -= *sb++;
}


void
rowDiffUnc (ppu dst, ppu src, ppu sub, int xi, int yi, unt dW, unt dH)
{
  register puc dp, sp, sb, dpEnd;
  if (src == sub)  rowZeroU(dst,xi,yi,dW,dH);
  else if (dst != src)
    for (dst += yi, src += yi, sub += yi; dH--; )
      for (dp = xi+*dst++, sp = xi+*src++, sb = xi+*sub++, dpEnd=dp+dW; dp<dpEnd; )
        *dp++ = *sp++ - *sb++;
  else
    for (dst += yi, sub += yi; dH--; )
      for (dp = xi+*dst++, sb = xi+*sub++, dpEnd=dp+dW; dp<dpEnd; )
        *dp++ -= *sb++;
}


