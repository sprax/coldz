/*****************************************************************************
mjMake.c    S.R.Lines (spraxlo@ai.mit.edu)    93.7.20
*****************************************************************************/
#include	<sys/types.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<malloc.h>
#include	<string.h>
#include	<assert.h>
#include	<un/unTypes.h>
#include	<un/unMem.h>
#include	<un/unBug.h>
#include	<mj.h>
#include	<mjCopy.h>
#include	<mjMake.h>

void mjPrint(pDmj d, char *tag)
{
  fprintf(stderr,"mjPrint %s: %u %u %d %d %d %d %d %d %u\n",
	  tag,d->w,d->h,d->x,d->y,d->l,d->r,d->t,d->b,d->d);
}


ppu mjSubUnc (ppu dst, ppu src, int dX, int dY, int sX, int sY, unt sW, unt sH)
{
ppu   row;
int   off = sX - dX;
  dst = realloc(dst,sH*sizeof(puc));
  if (!dst) die("mjSubUnc: bad realloc(%lu) = %lu",sH*sizeof(puc),dst);
  row = dst;
  for (src += sY; sH-- ; )
    *row++ = *src++ + off;
  return (dst -= dY);
}

#if 0000
/*__________________________________________________________________________**
  mjMem:
  
  Given a contiguous memory block mem, make an array of pointers to each mj.
  If mj is NULL, malloc enought space for it.
  The pathological case of overlap between mj and mem is not guarded,
  although if mj already points to an array of pointers to mjs in some
  block mem of length wide*tall, then mjMem(mj,*mj,wide,tall) should be
  harmless, just reassining the mj pointers to the same locations.
**__________________________________________________________________________*/

void *mjMem (void **mj, void *mem, unt wide, unt tall, unt ctype)
{				/* Conversions only for homegeneous types */
  switch (ctype) {
    case MM_UNC:  return mjMemUnc((unc**)mj,mem,wide,tall);
    case MM_FLT:  return mjMemFlt((flt**)mj,mem,wide,tall);
    default:      die(  "mjMem: NOIMP for type %d\n",ctype);
  }
  return NULL;
}

unc **mjMemUnc (unc **mj, unc *mem, unt wide, unt tall)
{
  unc **tmp;
  if ( ! mj)  mallocAss(mj,unc*,tall);
  for (tmp = mj; tall-- ; )
    *tmp++ = mem, mem += wide;
  return mj;
}

flt **mjMemFlt (flt **mj, flt *mem, unt wide, unt tall)
{
  flt **tmp;
  if ( ! mj)  mallocAss(mj,flt*,tall);
  for (tmp = mj; tall-- ; )
    *tmp++ = mem, mem += wide;
  return mj;
}



void *mjCal (int wide, int tall, int ctype)
{
  switch (ctype) {
    case MM_UNC:  return mjCalUnc(0,0,wide,tall);
    case MM_FLT:  return mjCalFlt(0,0,wide,tall);
    default:      die(  "mjCal: NOIMP for type %d\n",ctype);
  }
  return NULL;
}

void *mjMal (int wide, int tall, int ctype)
{
  switch (ctype) {
    case MM_UNC:  return mjMalUnc(0,0,wide,tall);
    case MM_FLT:  return mjMalFlt(0,0,wide,tall);
    default:      die(  "mjMal: NOIMP for type %d\n",ctype);
  }
  return NULL;
}


flt **mjCalFlt (int xi, int yi, unt wide, unt tall)
{
  flt **new, **mj, *mem;
  
  if (  ! (new = (flt **) malloc(sizeof(flt *)*tall))
     || ! (mem = (flt * ) calloc((size_t)tall*wide,sizeof(flt))))
    die("mjCalFlt: calloc %d",tall*wide);
  for (mem -= xi, mj = new; tall; tall--)
    *mj++ = mem, mem += wide;
  return new - yi;
}

dbl **mjCalDbl (int xi, int yi, unt wide, unt tall)
{
  dbl **new, **mj, *mem;
  
  if (  ! (new = (dbl **) malloc(sizeof(dbl *)*tall))
     || ! (mem = (dbl * ) calloc((size_t)tall*wide,sizeof(dbl))))
    die("mjCalDbl: calloc %d",tall*wide);
  for (mem -= xi, mj = new; tall; tall--)
    *mj++ = mem, mem += wide;
  return new - yi;
}

flt **mjMalFlt (int xi, int yi, unt wide, unt tall)
{
  flt **new, **mj, *mem;
  if (  ! (new = (flt **) malloc(sizeof(flt *)*tall))
     || ! (mem = (flt * ) malloc(sizeof(flt  )*tall*wide)))
    die("mjMalFlt: malloc %d x %d",wide,tall);
  for (mem -= xi, mj = new; tall; tall--)
    *mj++ = mem, mem += wide;
  return new - yi;
}


ppf mjPntFlt (ppf src, int dX, int dY, int sX, int sY, unt sW, unt sH)
{
  ppf	new, mj;
  int	off = sX - dX;

#if 1
  mallocAss(new,pfl,sH);
  mj = new;
#else
  if ( ! (mj = new = (ppf)malloc(sizeof(pfl)*sH)))
    die("mjPntFlt: malloc %u",sH);
#endif

  for (off = sX - dX, src += sY; sH-- ; )
    *mj++ = *src++ + off;
  return new - dY;
}


dbl **mjMalDbl (int xi, int yi, unt wide, unt tall)
{
  dbl **new, **mj, *mem;
  if (  ! (new = (dbl **) malloc(sizeof(dbl *)*tall))
     || ! (mem = (dbl * ) malloc(sizeof(dbl  )*tall*wide)))
    die("mjMalDbl: malloc %d x %d",wide,tall);
  for (mem -= xi, mj = new; tall; tall--)
    *mj++ = mem, mem += wide;
  return new - yi;
}


flt **mjRalFlt (ppf old, int xi, int yi, unt wide, unt tall)
{
  flt **new, **mj, *mem;
  if ( ! old )
    return mjMalFlt(xi,yi,wide,tall);
  else {
    if (  ! (new = (flt **) realloc(old+yi,    sizeof(flt *)*tall))
       || ! (mem = (flt * ) realloc(old[yi]+xi,sizeof(flt  )*tall*wide)))
      die("mjRalFlt: malloc %d x %d",wide,tall);
    for (mem -= xi, mj = new; tall; tall--)
      *mj++ = mem, mem += wide;
    return new - yi;
  }
}


unc **mjCalUnc (int xi, int yi, unt wide, unt tall)
{
  unc **new, **mj, *mem;
  
  if (  ! (new = (unc **) malloc(sizeof(unc *)*tall))
     || ! (mem = (unc * ) calloc((size_t)tall*wide,sizeof(unc))))
    die("mjCalUnc: calloc %d",tall*wide);
  for (mem -= xi, mj = new; tall; tall--)
    *mj++ = mem, mem += wide;
  return new - yi;
}

unc **mjMalUnc (int xi, int yi, unt wide, unt tall)
{
  unc **new, **mj, *mem;
  
  if (  ! (new = (unc **) malloc(sizeof(unc *)*tall))
     || ! (mem = (unc * ) malloc(sizeof(unc  )*tall*wide))) {
    die("mjMalUnc: malloc %d",tall*wide);
    new = (unc **)1;
    *new = (unc *)1;
    **new = (unc )1;
  }

  for (mem -= xi, mj = new; tall; tall--)
    *mj++ = mem, mem += wide;
  return new - yi;
}

shr **mjMalShr (int xi, int yi, unt wide, unt tall)
{
shr **new, **mj, *mem;
  if (  ! (new = (shr **) malloc(sizeof(ush *)*tall))
     || ! (mem = (shr * ) malloc(sizeof(ush  )*tall*wide)))
    die("mjMalShr: malloc %d",tall*wide);
  for (mem -= xi, mj = new; tall; tall--)
    *mj++ = mem, mem += wide;
  return new - yi;
}

ush **mjMalUsh (int xi, int yi, unt wide, unt tall)
{
ush **new, **mj, *mem;
  if (  ! (new = (ush **) malloc(sizeof(ush *)*tall))
     || ! (mem = (ush * ) malloc(sizeof(ush  )*tall*wide)))
    die("mjMalUsh: malloc %d",tall*wide);
  for (mem -= xi, mj = new; tall; tall--)
    *mj++ = mem, mem += wide;
  return new - yi;
}


ppf mjDupFlt(ppf src, int xi, int yi, unt wide, unt tall)
{
#if 0
  flt      **dst;
  const flt  *sp;

  if (  ! ( dst = (flt **) malloc(sizeof(flt *)*tall))
     || ! (*mem = (flt * ) malloc(sizeof(flt  )*tall*wide)))
    die("mjDupFlt: malloc %d x %d",wide,tall);
  for (src += yi; tall; dst++, tall--) {
    dst = mem;
    sp = *src++ + xi;
    memcpy(*dst,sp,rsize);
    *dst -= xi;
  }
  return dst - tall - yi;
#else
  ppf dst = mjMalFlt(xi,yi,wide,tall);
  mjCopyFF(dst, src, xi,yi,wide,tall,xi,yi);
  return dst;
#endif
}

void mjKillFlt (ppf mjs, int xi, int yi)
{
  if (mjs) {
    if (mjs[yi])  free(mjs[yi]+xi);
    free(mjs+yi);
  }
  mjs = NULL;
}

void mjKillUnc (ppu mjs, int xi, int yi)
{
  if (mjs) {
    if (mjs[yi])  free(mjs[yi]+xi);
    free(mjs+yi);
  }
  mjs = NULL;
}


void mjFree(void **mjs)
{
  if (mjs) {
    if  (*mjs)  free(*mjs);
    free( mjs);
  }
  mjs = NULL;
}


#endif /* 0000 */
