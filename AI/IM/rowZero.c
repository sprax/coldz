/*****************************************************************************
rowZero.c    S.R.Lines (spraxlo@ai)    93.6.1-7.11
*****************************************************************************/
#include <string.h>		/* memset */
#include <un/unBug.h>		/* die */
#include <un/unTypes.h>
#include "imagmem.h"
#include "m/rowZero.h"

void rowSetF ( ppf dst, int xo, int yo, unt wide, unt tall, double fv)
{
regflt	ff = (flt)fv;
regint	k;
pfl 	dp;

  for (dst += yo; tall-- ; )
    for (dp = *dst++ + xo, k = wide; k-- ; )
     *dp++ = ff;
}


void rowZeroF ( ppf dst, int xo, int yo, unt wide, unt tall )
{ 
  if (dst[1]-dst[0] == (int)wide) {
    /*wrn("rowZeroF: doing one big memset...................");*/
    memset((pvd)(dst[yo] + xo),0,sizeof(flt)*wide*tall);
  }
  else for (dst += yo; tall-- ; )
    memset((pvd)(*dst++ + xo),0,sizeof(flt)*wide);
}


void rowZeroU ( ppu dst, int xo, int yo, unt wide, unt tall )
{ 
  for (dst += yo; tall-- ; )
     memset((pvd)(*dst++ + xo),0,sizeof(unc)*wide);
}



void rowZeroFlt ( ppf dst, unt wide, unt tall )
{ 
#if 1		/* works on all machines I know. */
  while (tall--)
     memset((pvd)*dst++,0,sizeof(flt)*wide);
#else
  register int k;
  register pfl dp;
  while (tall--)
    for (dp = *dst++, k = wide; k; k--)
      *dp++ = 0.0F;
#endif
}


void rowZero ( void **dst, unt wide, unt tall, unt mmss)
{
  unt ctype = mmss & MM_TYPE;
  switch(ctype) {
    case MM_CHR:
    case MM_UNC:
      rowZeroUnc((unc **)dst,wide,tall);  break;
    case MM_FLT:
      rowZeroFlt((flt **)dst,wide,tall);  break;
    default: die("rowZero: NOIMP for unType %d",ctype);
  }
}


void rowZeroUnc(unc **dst, unt wide, unt tall)
{
  for ( ; tall; tall--)
     memset((void*)*dst++,0,wide);
}

