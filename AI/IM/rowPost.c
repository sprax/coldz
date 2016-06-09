/*****************************************************************************
rowPost.c    S.R.Lines (spraxlo@ai.mit.edu)    93.7.27
Posterize an image stored as an array of pointers to rows.
Assumptions (not followed here?):
    comparison ((unc)'c' < 'C') promotes both sides to int (not unc,short,etc)
    comparison ((flt)'c' < 'C') promotes both sides to float (not double)
    comparison ((dbl)'c' < 'C') promotes both sides to double.
*****************************************************************************/
#include <string.h>	/* void *memcpy(void *, const void *, size_t); */
#include <un/unMath.h>
#include <un/unTypes.h>
#include "imagmem.h"
#include "rowPost.h"

#undef	IRINTF		/* Return int nearest to float argument: want fast! */
#define IRINTF(f)  	((int)((f) < 0.0 ? ((f)-0.5F) : ((f)+0.5F)))
#undef	IRINT		/* want fast! */
#define IRINT(d)  	((int)((d) < 0.0 ? ((d)-0.50) : ((d)+0.50)))

void
rowPostNormFltToUnc8 ( unc **dst,flt **src
                     , int dX, int dY, unt dW, unt dH
                     , int sX, int sY, unt sW, unt sH
                     , dbl mean, dbl sdevs)
{
  flt frac = (flt)(sdevs * 0.25);
  register unc *dp;
  register unt k;
  register flt *sp; register flt sv;
  flt lev1 = (flt)(-3.5 * frac);
  flt lev2 = (flt)(-2.5 * frac);
  flt lev3 = (flt)(-1.5 * frac);
  flt lev4 = (flt)(-0.5 * frac);
  flt lev5 = (flt)( 0.5 * frac);
  flt lev6 = (flt)( 1.5 * frac);
  flt lev7 = (flt)( 2.5 * frac);
  flt lev8 = (flt)( 3.5 * frac);
  unc dv0  =   0;
  unc dv1  =  32;
  unc dv2  =  64;
  unc dv3  =  96;
  unc dv4  = 128; /* middle val */
  unc dv5  = 160;
  unc dv6  = 192;
  unc dv7  = 224;
  unc dv8  = 255;
 
  for (dst += dY, src += sY; dH; dH--) {
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k; k--) {
      sv = (flt)(*sp++ - mean);
      if (sv < lev4) {
        if (sv < lev2) {
          if (sv < lev1)      *dp++ = dv0;
          else                *dp++ = dv1;
        }
        else {
          if (sv < lev3)      *dp++ = dv2;
          else                *dp++ = dv3;
        }
      }
      else {
        if (sv < lev6) {
          if (sv < lev5)      *dp++ = dv4;
          else                *dp++ = dv5;
        }
        else {
          if (sv < lev7)      *dp++ = dv6;
          else if (sv < lev8) *dp++ = dv7;
          else                *dp++ = dv8;
        }
      }
    }
  }
}


		/* For positive values (only): */
void
rowPostFltToUnc8 (unc **dst,flt **src
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, dbl frac)
{
  register unc *dp;
  register unt k;
  register flt *sp; register flt sv;
  flt lev1 = (flt)frac  ;
  flt lev2 = (flt)frac*2;
  flt lev3 = (flt)frac*3;
  flt lev4 = (flt)frac*4;
  flt lev5 = (flt)frac*5;
  flt lev6 = (flt)frac*6;
  flt lev7 = (flt)frac*7;
  flt lev8 = (flt)frac*8;
  unc dv0  =   0;
  unc dv1  =  32;
  unc dv2  =  64;
  unc dv3  =  96;
  unc dv4  = 128;
  unc dv5  = 160;
  unc dv6  = 192;
  unc dv7  = 224;
  unc dv8  = 255;
 
  for (dst += dY, src += sY; dH; dH--) {
    for (dp = *dst++ + dX, sp = *src++ + sX, k = dW; k; k--) {
      sv = *sp++;
      if (sv < lev4) {
        if (sv < lev2) {
          if (sv < lev1)   *dp++ = dv0;
          else             *dp++ = dv1;
        }
        else {
          if (sv < lev3)   *dp++ = dv2;
          else             *dp++ = dv3;
        }
      }
      else {
        if (sv < lev6) {
          if (sv < lev5)   *dp++ = dv4;
          else             *dp++ = dv5;
        }
        else {
          if (sv < lev7)   *dp++ = dv6;
          else
            if (sv < lev8) *dp++ = dv7;
          else             *dp++ = dv8;
        }
      }
    }
  }
}











#if 0000
void rowClip(void **dst,void **src,unt wide,unt tall,unt dmmss,unt smmss,double lo,double hi)
{   /* Eventually either simplify mmss args or else use them in func. */
  unt dtype = dmmss & MM_TYPE;
  unt stype = smmss & MM_TYPE;
  
  switch(dtype) {
    case MM_CHR:
    case MM_UNC:
      rowClipToUnc((unc**)dst,src,wide,tall,stype,lo,hi);
      break;
    case MM_FLT:
      rowClipToFlt((flt**)dst,src,wide,tall,stype,lo,hi);
      break;
    default: die("rowClip: NOIMP for dst type %d",dtype);
  }
}

void rowPostToUnc(unc **dst,void **src,unt wide,unt tall,unt srctype,int lo, int hi)
{
  register unc *dp;
  register unt k;
  register int oo = irint(lo), ii = irint(hi);

  switch(srctype) {
    case MM_CHR:  			/* Treat char same as unchar */
    case MM_UNC: { register ush *sp; register int sv;
      for (; tall; tall--)
        for (dp = *dst++, sp = *src++, k = wide; k; k--) {
          sv = (int) *sp++;
          if (sv > ii)  sv = ii;  else
          if (sv < oo)  sv = oo;
          *dp++ = (unc) sv;
        }
    } break;
    case MM_SHT:  			/* Treat int32 same as uint32 */
    case MM_USH: { register ush *sp; register int sv;
      for (; tall; tall--)
        for (dp = *dst++, sp = *src++, k = wide; k; k--) {
          sv = (int) *sp++;
          if (sv > ii)  sv = ii;  else
          if (sv < oo)  sv = oo;
          *dp++ = (unc) sv;
        }
    } break;
    case MM_INT:  			/* Treat int32 same as uint32  @? */
    case MM_UNT: { register int *sp; register int sv;
      for (; tall; tall--)
        for (dp = *dst++, sp = *src++, k = wide; k; k--) {
          sv = (int) *sp++;
          if (sv > ii)  sv = ii;  else
          if (sv < oo)  sv = oo;
          *dp++ = (unc) sv;
        }
    } break;
    case MM_FLT: { register flt *sp; register flt sv;
      for (; tall; tall--)
        for (dp = *dst++, sp = *src++, k = wide; k; k--) {
          sv = *sp++;
          sv = IRINTF(sv);
          if (sv > ii)  sv = ii;  else
          if (sv < oo)  sv = oo;
          *dp++ = (unc) sv;
        }
    } break;
    case MM_DBL: { register dbl *sp; register dbl sv;
      for (; tall; tall--)
        for (dp = *dst++, sp = *src++, k = wide; k; k--) {
          sv = *sp++;
          sv = IRINT(sv);
          if (sv > ii)  sv = ii;  else
          if (sv < oo)  sv = oo;
          *dp++ = (unc) sv;
        }
    } break;
    default: die("rowClipToUnc: NOIMP for src type %d",srctype);
  }
}


void rowClipToFlt (flt **dst, void **src, unt wide, unt tall, unt srctype, double lo, double hi)
{
  register flt *dp;
  register unt k;
    
  switch(srctype) {
    case MM_FLT:	/* Don't optimize this case to single memcpy */
      for ( ; tall; tall--)
        memcpy(*dst++,*src++,sizeof(flt)*wide);
      break;
    case MM_CHR:  			/* Treat char same as unchar */
    case MM_UNC: { register unc *sp;
      for (; tall; tall--)
        for (dp = *dst++, sp = *src++, k = wide; k; k--)
          *dp++ = (flt) *sp++;
    } break;
    case MM_SHT:  			/* Treat int32 same as uint32 */
    case MM_USH: { register ush *sp;
      for (; tall; tall--)
        for (dp = *dst++, sp = *src++, k = wide; k; k--)
          *dp++ = (flt) *sp++;
    } break;
    case MM_INT:  			/* Treat int32 same as uint32 */
    case MM_UNT: { register int *sp;
      for (; tall; tall--)
        for (dp = *dst++, sp = *src++, k = wide; k; k--)
          *dp++ = (flt) *sp++;
    } break;
    case MM_DBL: { register dbl *sp;
      for (; tall; tall--)
        for (dp = *dst++, sp = *src++, k = wide; k; k--)
          *dp++ = (flt) *sp++;
    } break;
    default: die("rowClipToFlt: NOIMP for src type %d",srctype);
  }
}


void rowClipFlt (flt **dst, flt **src, unt wide, unt tall, double lo, double hi)
{ /* not much more overhead than the macro ROWCOPYFLT */
  for ( ; tall; tall--)
    memcpy((void*)*dst++,(void*)*src++,sizeof(flt)*wide);
}

#endif	/* 0000 */

