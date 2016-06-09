/*****************************************************************************
rowMake.c    S.R.Lines (spraxlo@ai.mit.edu)    93.7.20
*****************************************************************************/
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>
#include	<un/unTypes.h>
#include	<un/unMem.h>
#include	<un/unBug.h>
#include	<imagmem.h>
#include	<m/rowCopy.h>
#include	<m/rowMake.h>

/*__________________________________________________________________________**
  rowMem:
  
  Given a contiguous memory block mem, make an array of pointers to each row.
  If row is NULL, malloc enought space for it.
  The pathological case of overlap between row and mem is not guarded,
  although if row already points to an array of pointers to rows in some
  block mem of length wide*tall, then rowMem(row,*row,wide,tall) should be
  harmless, just reassining the row pointers to the same locations.
**__________________________________________________________________________*/

void *rowMem (void **row, void *mem, unt wide, unt tall, unt ctype)
{				/* Conversions only for homegeneous types */
  switch (ctype) {
    case MM_UNC:  return rowMemUnc((unc**)row,(unc*)mem,wide,tall);
    case MM_FLT:  return rowMemFlt((flt**)row,(flt*)mem,wide,tall);
    default:      die(  "rowMem: NOIMP for type %d\n",ctype);
  }
  return NULL;
}

unc **rowMemUnc (unc **row, unc *mem, unt wide, unt tall)
{
  unc **tmp;
  if ( ! row)  mallocAss(row,unc*,tall);
  for (tmp = row; tall-- ; )
    *tmp++ = mem, mem += wide;
  return row;
}

flt **rowMemFlt (flt **row, flt *mem, unt wide, unt tall)
{
  flt **tmp;
  if ( ! row)  mallocAss(row,flt*,tall);
  for (tmp = row; tall-- ; )
    *tmp++ = mem, mem += wide;
  return row;
}

ppu rowPntUnc (ppu src, int dX, int dY, int sX, int sY, unt sW, unt sH)
{
ppu   mem, row;
int   off = sX - dX;
  mallocAss(mem,puc,sH);
  row = mem;
  for (src += sY; sH-- ; )
    *row++ = *src++ + off;
  return mem - dY;
}



ppf rowPntFlt (ppf src, int dX, int dY, int sX, int sY, unt sW, unt sH)
{
ppf   mem, row;
int   off = sX - dX;
  mallocAss(mem,pfl,sH);
  row = mem;
  for (off = sX - dX, src += sY; sH-- ; )
    *row++ = *src++ + off;
  return mem - dY;
}


void **rowMalVoid (int xi, int yi, unt wide, unt tall, size_t eltSize)
{
  void **nro, **row;
  char *mem;
  if (  ! (nro = (void **) malloc(sizeof(void*)*tall))
     || ! (mem = (char * ) malloc(eltSize* wide*tall)))
    die("rowMalVoid: malloc %d x %d sz %d",wide,tall,eltSize);
  for (mem -= xi, row = nro; tall; tall--)
    *row++ = mem, mem = mem + wide*eltSize;
  return nro - yi;
}


void *rowCal (int wide, int tall, int ctype)
{
  switch (ctype) {
    case MM_UNC:  return rowCalUnc(0,0,wide,tall);
    case MM_FLT:  return rowCalFlt(0,0,wide,tall);
    default:      die(  "rowCal: NOIMP for type %d\n",ctype);
  }
  return NULL;
}

void *rowMal (int wide, int tall, int ctype)
{
  switch (ctype) {
    case MM_UNC:  return rowMalUnc(0,0,wide,tall);
    case MM_FLT:  return rowMalFlt(0,0,wide,tall);
    default:      die(  "rowMal: NOIMP for type %d\n",ctype);
  }
  return NULL;
}


flt **rowCalFlt (int xi, int yi, unt wide, unt tall)
{
  flt **nro, **row, *mem;
  
  if (  ! (nro = (flt **) malloc(sizeof(flt *)*tall))
     || ! (mem = (flt * ) calloc((size_t)tall*wide,sizeof(flt))))
    die("rowCalFlt: calloc %d",tall*wide);
  for (mem -= xi, row = nro; tall; tall--)
    *row++ = mem, mem += wide;
  return nro - yi;
}

dbl **rowCalDbl (int xi, int yi, unt wide, unt tall)
{
  dbl **nro, **row, *mem;
  
  if (  ! (nro = (dbl **) malloc(sizeof(dbl *)*tall))
     || ! (mem = (dbl * ) calloc((size_t)tall*wide,sizeof(dbl))))
    die("rowCalDbl: calloc %d",tall*wide);
  for (mem -= xi, row = nro; tall; tall--)
    *row++ = mem, mem += wide;
  return nro - yi;
}

flt **rowMalFlt (int xi, int yi, unt wide, unt tall)
{
  flt **nro, **row, *mem;
  if (  ! (nro = (flt **) malloc(sizeof(flt *)*tall))
     || ! (mem = (flt * ) malloc(sizeof(flt  )*tall*wide)))
    die("rowMalFlt: malloc %d x %d",wide,tall);
  for (mem -= xi, row = nro; tall; tall--)
    *row++ = mem, mem += wide;
  return nro - yi;
}



dbl **rowMalDbl (int xi, int yi, unt wide, unt tall)
{
  dbl **nro, **row, *mem;
  if (  ! (nro = (dbl **) malloc(sizeof(dbl *)*tall))
     || ! (mem = (dbl * ) malloc(sizeof(dbl  )*tall*wide)))
    die("rowMalDbl: malloc %d x %d",wide,tall);
  for (mem -= xi, row = nro; tall; tall--)
    *row++ = mem, mem += wide;
  return nro - yi;
}


flt **rowRalFlt (ppf old, int xi, int yi, unt wide, unt tall)
{
  flt **nro, **row, *mem;
  if ( ! old )
    return rowMalFlt(xi,yi,wide,tall);
  else {
    if (  ! (nro = (flt **) realloc(old+yi,    sizeof(flt *)*tall))
       || ! (mem = (flt * ) realloc(old[yi]+xi,sizeof(flt  )*tall*wide)))
      die("rowRalFlt: malloc %d x %d",wide,tall);
    for (mem -= xi, row = nro; tall; tall--)
      *row++ = mem, mem += wide;
    return nro - yi;
  }
}


unc **rowCalUnc (int xi, int yi, unt wide, unt tall)
{
  unc **nro, **row, *mem;
  
  if (  ! (nro = (unc **) malloc(sizeof(unc *)*tall))
     || ! (mem = (unc * ) calloc((size_t)tall*wide,sizeof(unc))))
    die("rowCalUnc: calloc %d",tall*wide);
  for (mem -= xi, row = nro; tall; tall--)
    *row++ = mem, mem += wide;
  return nro - yi;
}

unc **rowMalUnc (int xi, int yi, unt wide, unt tall)
{
  unc **nro, **row, *mem;
  
  if (  ! (nro = (unc **) malloc(sizeof(unc *)*tall))
     || ! (mem = (unc * ) malloc(sizeof(unc  )*tall*wide))) {
    die("rowMalUnc: malloc %d",tall*wide);
    nro = (unc **)1;
    *nro = (unc *)1;
    **nro = (unc )1;
  }

  for (mem -= xi, row = nro; tall; tall--)
    *row++ = mem, mem += wide;
  return nro - yi;
}

shr **rowMalShr (int xi, int yi, unt wide, unt tall)
{
shr **nro, **row, *mem;
  if (  ! (nro = (shr **) malloc(sizeof(ush *)*tall))
     || ! (mem = (shr * ) malloc(sizeof(ush  )*tall*wide)))
    die("rowMalShr: malloc %d",tall*wide);
  for (mem -= xi, row = nro; tall; tall--)
    *row++ = mem, mem += wide;
  return nro - yi;
}

ush **rowMalUsh (int xi, int yi, unt wide, unt tall)
{
ush **nro, **row, *mem;
  if (  ! (nro = (ush **) malloc(sizeof(ush *)*tall))
     || ! (mem = (ush * ) malloc(sizeof(ush  )*tall*wide)))
    die("rowMalUsh: malloc %d",tall*wide);
  for (mem -= xi, row = nro; tall; tall--)
    *row++ = mem, mem += wide;
  return nro - yi;
}


ppf rowDupFlt(ppf src, int xi, int yi, unt wide, unt tall)
{
#if 0
  flt      **dst;
  const flt  *sp;

  if (  ! ( dst = (flt **) malloc(sizeof(flt *)*tall))
     || ! (*mem = (flt * ) malloc(sizeof(flt  )*tall*wide)))
    die("rowDupFlt: malloc %d x %d",wide,tall);
  for (src += yi; tall; dst++, tall--) {
    dst = mem;
    sp = *src++ + xi;
    memcpy(*dst,sp,rsize);
    *dst -= xi;
  }
  return dst - tall - yi;
#else
  ppf dst = rowMalFlt(xi,yi,wide,tall);
  rowCopyFF(dst, src, xi,yi,wide,tall,xi,yi);
  return dst;
#endif
}

void rowKillFlt (ppf rows, int xi, int yi)
{
  if (rows) {
    if (rows[yi])  free(rows[yi]+xi);
    free(rows+yi);
  }
  rows = NULL;
}

void rowKillUnc (ppu rows, int xi, int yi)
{
  if (rows) {
    if (rows[yi])  free(rows[yi]+xi);
    free(rows+yi);
  }
  rows = NULL;
}


void rowFree(void **rows)
{
  if (rows) {
    if  (*rows)  free(*rows);
    free( rows);
  }
  rows = NULL;
}


