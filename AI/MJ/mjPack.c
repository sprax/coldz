/*****************************************************************************
 mjPack.c    S.R.Lines    93.10.10
 *****************************************************************************/
#include <un/unTypes.h>
#include <mj.h>
#include <mjPack.h>

void
mjPackUncGray ( rgst unl *l, ppu src, pDmj D)
{
rgst puc u;
rgst int k;
int      x = D->x, q = D->w/8, r = D->w % 8, j = D->y + D->h;

  src += j;
  j = D->h; while (j--) {
    u = *--src + x;
    k = q; while(k--) {
      l[0] = 0xff000000+0x00010101*(ush)u[0];
      l[1] = 0xff000000+0x00010101*(ush)u[1];
      l[2] = 0xff000000+0x00010101*(ush)u[2];
      l[3] = 0xff000000+0x00010101*(ush)u[3];
      l[4] = 0xff000000+0x00010101*(ush)u[4];
      l[5] = 0xff000000+0x00010101*(ush)u[5];
      l[6] = 0xff000000+0x00010101*(ush)u[6];
      l[7] = 0xff000000+0x00010101*(ush)u[7];
      l += 8;
      u += 8;
    }
    k = r; while(k--)
      *l++ = 0xff000000+0x00010101*(ush)*u++;
  }
}

void
mjPackFltRGBI ( rgst unl *l, ppf src, pDmj D)
{ /* Truncating, not rounding.  For quick screen display only. */
rgst pfl f;
int	x3 = D->x*3;
unt 	j, k;
#if 0	/* USED TO INVERT DATA -- NO MORE! */
ppf 	rgb = src + D->y + D->h - 1;
  j = D->h; while (j--) {
    f = *rgb-- + x3;
#else
  src += D->y;
  j = D->h; while (j--) {
    f = *src++ + x3;
#endif
    for (k = D->w; k >= 8; ) {
      l[0] = (ush)f[ 0] | ((ush)f[ 1]<<8) | ((ush)f[ 2]<<16) | ((unl)0xff<<24);
      l[1] = (ush)f[ 3] | ((ush)f[ 4]<<8) | ((ush)f[ 5]<<16) | ((unl)0xff<<24);
      l[2] = (ush)f[ 6] | ((ush)f[ 7]<<8) | ((ush)f[ 8]<<16) | ((unl)0xff<<24);
      l[3] = (ush)f[ 9] | ((ush)f[10]<<8) | ((ush)f[11]<<16) | ((unl)0xff<<24);
      l[4] = (ush)f[12] | ((ush)f[13]<<8) | ((ush)f[14]<<16) | ((unl)0xff<<24);
      l[5] = (ush)f[15] | ((ush)f[16]<<8) | ((ush)f[17]<<16) | ((unl)0xff<<24);
      l[6] = (ush)f[18] | ((ush)f[19]<<8) | ((ush)f[20]<<16) | ((unl)0xff<<24);
      l[7] = (ush)f[21] | ((ush)f[22]<<8) | ((ush)f[23]<<16) | ((unl)0xff<<24);
      f += 24;
      l +=  8;
      k -=  8;
    }
    while(k--) {
      *l++ = ((ush)f[0] | ((ush)f[1]<<8) | ((ush)f[2]<<16) | ((unl)0xff<<24));
      f += 3;
    }
  }
}


#if 1
void
mjPackUncRGBP(rgst unl *l, ppu rgbp[], pDmj D)
{
unt k, j = D->y;
ppu red = rgbp[0]+j, grn = rgbp[1]+j, blu = rgbp[2]+j;
puc r,g,b;

  j = D->h; while (j--) {
    k = D->x;
    r = *red++ + k;
    g = *grn++ + k;
    b = *blu++ + k;
    for (k = D->w; k >= 8; ) {
      l[0] = r[0] | (g[0]<<8) | (b[0]<<16) | ((unl)0xff<<24);
      l[1] = r[1] | (g[1]<<8) | (b[1]<<16) | ((unl)0xff<<24);
      l[2] = r[2] | (g[2]<<8) | (b[2]<<16) | ((unl)0xff<<24);
      l[3] = r[3] | (g[3]<<8) | (b[3]<<16) | ((unl)0xff<<24);
      l[4] = r[4] | (g[4]<<8) | (b[4]<<16) | ((unl)0xff<<24);
      l[5] = r[5] | (g[5]<<8) | (b[5]<<16) | ((unl)0xff<<24);
      l[6] = r[6] | (g[6]<<8) | (b[6]<<16) | ((unl)0xff<<24);
      l[7] = r[7] | (g[7]<<8) | (b[7]<<16) | ((unl)0xff<<24);
      l += 8;
      r += 8;
      g += 8;
      b += 8;
      k -= 8;
    }
    while(k--)
      *l++ = *r++ | ((*g++)<<8) | ((*b++)<<16) | ((unl)0xff<<24);
  }
}
#else
void
mjPackUncRGBP(rgst unl *l, ppu rgbp[], pDmj D)
{
unt k, j = D->y + D->h - 1;
ppu red = rgbp[0]+j, grn = rgbp[1]+j, blu = rgbp[2]+j;
puc r,g,b;

  j = D->h; while (j--) {
    k = D->x;
    r = *red-- + k;
    g = *grn-- + k;
    b = *blu-- + k;
    for (k = D->w; k >= 8; ) {
      l[0] = r[0] | (g[0]<<8) | (b[0]<<16) | ((unl)0xff<<24);
      l[1] = r[1] | (g[1]<<8) | (b[1]<<16) | ((unl)0xff<<24);
      l[2] = r[2] | (g[2]<<8) | (b[2]<<16) | ((unl)0xff<<24);
      l[3] = r[3] | (g[3]<<8) | (b[3]<<16) | ((unl)0xff<<24);
      l[4] = r[4] | (g[4]<<8) | (b[4]<<16) | ((unl)0xff<<24);
      l[5] = r[5] | (g[5]<<8) | (b[5]<<16) | ((unl)0xff<<24);
      l[6] = r[6] | (g[6]<<8) | (b[6]<<16) | ((unl)0xff<<24);
      l[7] = r[7] | (g[7]<<8) | (b[7]<<16) | ((unl)0xff<<24);
      l += 8;
      r += 8;
      g += 8;
      b += 8;
      k -= 8;
    }
    while(k--)
      *l++ = *r++ | ((*g++)<<8) | ((*b++)<<16) | ((unl)0xff<<24);
  }
}
#endif



#if 1   /* Yes, this really does test faster, using prof -pixie, time, etc. */
void
mjPackFltGray(register punl l, ppf f, pDmj D)
{
unt k,j;
pfl g;
  f += D->y;
  j  = D->h; while (j--) {
    for (g = *f++ + D->x, k = D->w; k >= 8; ) {
      l[0] = 0xff000000+0x00010101*(ush)g[0];
      l[1] = 0xff000000+0x00010101*(ush)g[1];
      l[2] = 0xff000000+0x00010101*(ush)g[2];
      l[3] = 0xff000000+0x00010101*(ush)g[3];
      l[4] = 0xff000000+0x00010101*(ush)g[4];
      l[5] = 0xff000000+0x00010101*(ush)g[5];
      l[6] = 0xff000000+0x00010101*(ush)g[6];
      l[7] = 0xff000000+0x00010101*(ush)g[7];
      l += 8;
      g += 8;
      k -= 8;
    }
    while(k--)
      *l++ = 0xff000000+0x00010101*(ush)*g++;
  }
}
#else
void
mjPackFltGray(register unsigned long *l, register flt *f, register int n)
{
  register unsigned short u;
  while(n>=8) {
    u = f[0], l[0] = 0xff000000+0x00010101*u;
    u = f[1], l[1] = 0xff000000+0x00010101*u;
    u = f[2], l[2] = 0xff000000+0x00010101*u;
    u = f[3], l[3] = 0xff000000+0x00010101*u;
    u = f[4], l[4] = 0xff000000+0x00010101*u;
    u = f[5], l[5] = 0xff000000+0x00010101*u;
    u = f[6], l[6] = 0xff000000+0x00010101*u;
    u = f[7], l[7] = 0xff000000+0x00010101*u;
    l += 8;
    f += 8;
    n -= 8;
  }
  while(n--)
    u = *f++, *l++ = 0xff000000+0x00010101*u;
}
#endif



void	/* Truncating, not rounding.  For quick screen display only. */
mjPackUncRGBS(rgst unl *l, rgst puc r, rgst puc g, rgst puc b, rgst unt n)
{
  while(n>=8) {
    l[0] = r[0] | (g[0]<<8) | (b[0]<<16) | ((unl)0xff<<24);
    l[1] = r[1] | (g[1]<<8) | (b[1]<<16) | ((unl)0xff<<24);
    l[2] = r[2] | (g[2]<<8) | (b[2]<<16) | ((unl)0xff<<24);
    l[3] = r[3] | (g[3]<<8) | (b[3]<<16) | ((unl)0xff<<24);
    l[4] = r[4] | (g[4]<<8) | (b[4]<<16) | ((unl)0xff<<24);
    l[5] = r[5] | (g[5]<<8) | (b[5]<<16) | ((unl)0xff<<24);
    l[6] = r[6] | (g[6]<<8) | (b[6]<<16) | ((unl)0xff<<24);
    l[7] = r[7] | (g[7]<<8) | (b[7]<<16) | ((unl)0xff<<24);
    l += 8;
    r += 8;
    g += 8;
    b += 8;
    n -= 8;
  }
  while(n--)
    *l++ = *r++ | ((*g++)<<8) | ((*b++)<<16) | ((unl)0xff<<24);
}


void mjPackUshGray (unsigned long *l, ush *b, unt n)
{
  unt r = n/8;
  while (r--) {
    l[0] = 0xff000000+0x00010101*b[0];
    l[1] = 0xff000000+0x00010101*b[1];
    l[2] = 0xff000000+0x00010101*b[2];
    l[3] = 0xff000000+0x00010101*b[3];
    l[4] = 0xff000000+0x00010101*b[4];
    l[5] = 0xff000000+0x00010101*b[5];
    l[6] = 0xff000000+0x00010101*b[6];
    l[7] = 0xff000000+0x00010101*b[7];
    l += 8;
    b += 8;
  }
  r = n % 8;
  while(r--)
    *l++ = 0xff000000+0x00010101*(*b++);
}


void mjPackShrGray (unsigned long *l, shr *b, unt n)
{
  unt r = n/8;
  while (r--) {
    l[0] = 0xff000000+0x00010101*(ush)b[0];	/* The cast is necessary! */
    l[1] = 0xff000000+0x00010101*(ush)b[1];
    l[2] = 0xff000000+0x00010101*(ush)b[2];
    l[3] = 0xff000000+0x00010101*(ush)b[3];
    l[4] = 0xff000000+0x00010101*(ush)b[4];
    l[5] = 0xff000000+0x00010101*(ush)b[5];
    l[6] = 0xff000000+0x00010101*(ush)b[6];
    l[7] = 0xff000000+0x00010101*(ush)b[7];
    l += 8;
    b += 8;
  }
  r = n % 8;
  while(r--)
    *l++ = 0xff000000+0x00010101*(ush)(*b++);
}



