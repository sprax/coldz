/*****************************************************************************
rowMult.c    S.R.Lines (spraxlo@ai.mit.edu)    93.7.27
Pixelwise multiplication in place of an image stored as an array of
pointers to rows.
Trc truncates instead of rounds (unsigned char only).
*****************************************************************************/
#include	<un/unTypes.h>
#include	"imagmem.h"
#include	"rowMult.h"
void die(const char *fmt, ...);

#undef	IRINTF		/* Return int nearest to float argument: want fast! */
#define IRINTF(f)  	((int)((f) < 0.0 ? ((f)-0.5F) : ((f)+0.5F)))

     
     /* For positively valued src: */
void
rowMultFltToUnc (ppu dst, ppf src, int xi, int yi, unt dW, unt dH, double factor)
{ /* Rounding. */
  register unc *dp, *dpEnd;
  register flt *sp, val, fac = (flt)factor;

  for (src += yi, dst += yi; dH; dH--)
    for (sp = *src++ + xi, dp = *dst++ + xi, dpEnd = dp + dW; dp < dpEnd; )
      val = fac * *sp++, *dp++ = (unc) (val + 0.4995);
}

void rowMult (void *dst, void *src, int xi, int yi, unt dW, unt dH, unt mmss, double factor)
{
  unt ctype = mmss & MM_TYPE;
  switch(ctype) {
    case MM_CHR:
    case MM_UNC:
      rowMultUnc((unc **)dst,src,xi,yi,dW,dH,factor);  break;
    case MM_FLT:
      rowMultFlt((flt **)dst,src,xi,yi,dW,dH,factor);  break;
    default: die("rowMult: NOIMP for ctype %d\n",ctype);
  }
}


void rowMultFlt (flt **dst, flt **src, int xi, int yi, unt dW, unt dH, double factor)
{
  register flt *dp, *sp, *dpEnd, fac = (flt)factor;

  if (dst != src)
    for (src += yi, dst += yi; dH; dH--)
      for (sp = *src++ + xi, dp = *dst++ + xi, dpEnd = dp + dW;  dp < dpEnd; )
        *dp++ = fac * *sp++;
  else
    for (dst += yi; dH; dH--)
      for (dp = *dst++ + xi, dpEnd = dp + dW;  dp < dpEnd; dp++)
        *dp *= fac;
}


void rowMultUnc (unc **dst, unc **src, int xi, int yi, unt dW, unt dH, double factor)
{ /* Rounding. */
  register unc *dp, *sp, *dpEnd;
  register flt val, fac = (flt)factor;

  if (dst != src)
    for (src += yi, dst += yi; dH; dH--)
      for (sp = *src++ + xi, dp = *dst++ + xi, dpEnd = dp + dW;  dp < dpEnd; )
        val = *sp++ * fac, *dp++ = IRINTF(val);
  else
    for (dst += yi; dH; dH--)
      for (dp = *dst++ + xi, dpEnd = dp + dW;  dp < dpEnd; dp++)
        val = *dp * fac, *dp = IRINTF(val);
}


void rowMultTrc (unc **dst, unc **src, int xi, int yi, unt dW, unt dH, double factor)
{ /* No Rounding. */
  register unc *dp, *sp, *dpEnd;
  register flt fac = (flt)factor;

  if (dst != src)
    for (src += yi, dst += yi; dH; dH--)
      for (sp = *src++ + xi, dp = *dst++ + xi, dpEnd = dp + dW;  dp < dpEnd; )
        *dp++ = (unc)(fac * *sp++);
  else
    for (dst += yi; dH; dH--)
      for (dp = *dst++ + xi, dpEnd = dp + dW;  dp < dpEnd; dp++)
        *dp = (unc)(*dp * fac);
}


