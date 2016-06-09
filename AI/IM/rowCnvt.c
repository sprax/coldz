/*****************************************************************************
 rowCnvt.c    S.R.Lines    92.1.1-93.10.10
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <un/unTypes.h>
#include <rowCnvt.h>

#define REDF (79)
#define GRNF (156)
#define BLUF (21)


#define ILUM(r,g,b)     ((RINTLUM*(r)+GINTLUM*(g)+BINTLUM*(b))>>8)

void    /* interleave R G B channels */
rowGryGry3F(ppf dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{
pfl     dp,sp;
unt     k;

  for (dX *= 3, dst += dY, src += sY; dH; dH--) {
    dp = *dst++ + dX;
    sp = *src++ + sX;
    k  =  dW;  while (k--) {
      dp[2] = dp[1] = dp[0] = *sp++;
      dp += 3;
    }
  }
}

void    /* interleave R G B channels */
rgbGreyFlt(ppf dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{
pfl     dp,sp;
unt     k;

  for (dX *= 3, dst += dY, src += sY; dH; dH--) {
    dp = *dst++ + dX;
    sp = *src++ + sX;
    k  =  dW;  while (k--) {
      *dp++ = (flt)(REDD*sp[0] + GRND*sp[1] + BLUD*sp[2]);
       sp += 3;
    }
  }
}



void rgbGrayUnc3 ( ppf dRGB, ppf dGry, ppu srcU[3]
                 , int cX, int cY
                 , int gX, int gY
                 , unt dW, unt dH, int sX, int sY)
{
ppu     sR = srcU[0]+sY, sG = srcU[1]+sY, sB = srcU[2]+sY;
pfl     dC, dV;
puc     rp,gp,bp;
unt     k, q = dW/8, r = dW % 8,  cX3 = cX*3;

  for (dRGB += cY, dGry += gY; dH-- ; ) {
    dC = *dRGB++ + cX3;
    dV = *dGry++ + gX;
    rp = *sR++   + sX;
    gp = *sG++   + sX;
    bp = *sB++   + sX;
    k  =  q;  while(k--) {
      dC[ 0] =                   rp[0]; dC[ 1] = gp[0]; dC[ 2] = bp[0];
      dV[ 0] = (flt)(((unt)(REDF*rp[0] +  GRNF * gp[0] +  BLUF * bp[0]))>>8);
      dC[ 3] =                   rp[1]; dC[ 4] = gp[1]; dC[ 5] = bp[1];
      dV[ 1] = (flt)(((unt)(REDF*rp[1] +  GRNF * gp[1] +  BLUF * bp[1]))>>8);
      dC[ 6] =                   rp[2]; dC[ 7] = gp[2]; dC[ 8] = bp[2];
      dV[ 2] = (flt)(((unt)(REDF*rp[2] +  GRNF * gp[2] +  BLUF * bp[2]))>>8);
      dC[ 9] =                   rp[3]; dC[10] = gp[3]; dC[11] = bp[3];
      dV[ 3] = (flt)(((unt)(REDF*rp[3] +  GRNF * gp[3] +  BLUF * bp[3]))>>8);
      dC[12] =                   rp[4]; dC[13] = gp[4]; dC[14] = bp[4];
      dV[ 4] = (flt)(((unt)(REDF*rp[4] +  GRNF * gp[4] +  BLUF * bp[4]))>>8);
      dC[15] =                   rp[5]; dC[16] = gp[5]; dC[17] = bp[5];
      dV[ 5] = (flt)(((unt)(REDF*rp[5] +  GRNF * gp[5] +  BLUF * bp[5]))>>8);
      dC[18] =                   rp[6]; dC[19] = gp[6]; dC[20] = bp[6];
      dV[ 6] = (flt)(((unt)(REDF*rp[6] +  GRNF * gp[6] +  BLUF * bp[6]))>>8);
      dC[21] =                   rp[7]; dC[22] = gp[7]; dC[23] = bp[7];
      dV[ 7] = (flt)(((unt)(REDF*rp[7] +  GRNF * gp[7] +  BLUF * bp[7]))>>8);
      rp +=  8;
      gp +=  8;
      bp +=  8;
      dV +=  8;
      dC += 24;
    }
    k  =  r;  while(k--) {
      *dV++  = (flt)(((unt)(REDF**rp   +  GRNF * *gp   +  BLUF * *bp))>>8);
      *dC++  =                   *rp++ ; *dC++ = *gp++ ; *dC++ = *bp++ ;
    }
  }
}


void
gryRgbUnc (ppu dst, ppu red, ppu grn, ppu blu, int dX, int dY, unt dW, unt dH)
{
puc	dp, rp, gp, bp;  
unt	k;

  red += dY, blu += dY, grn += dY;
  while ( dH--) {
    dp = dX + *dst++;
    rp = dX + *red++;
    gp = dX + *grn++;
    bp = dX + *blu++;
    k = dW;  while (k--) {
      *dp++ = ((unt)(REDF**rp++ + GRNF**gp++ + BLUF**bp++))>>8; 
    }
  }
}

void
gryRgbFlt (ppf dst, ppf red, ppf grn, ppf blu, int dX, int dY, unt dW, unt dH)
{
pfl	dp, rp, gp, bp;  
unt	k;

  for (dst += dY, red += dY, grn += dY, blu += dY; dH-- ; ) {
    dp = dX + *dst++;
    rp = dX + *red++;
    gp = dX + *grn++;
    bp = dX + *blu++;
    k  = dW;  while (k--) {
      *dp++ = (flt)(REDF**rp++ + GRNF**gp++ + BLUF**bp++); 
    }
  }
}

void
gryRgbUsh (ppus dst, ppus red, ppus grn, ppus blu, unt dW, unt dH)
{
pus	dp, rp, gp, bp;  
unt	k;

  while ( dH--) {
    dp = *dst++;
    rp = *red++;
    gp = *grn++;
    bp = *blu++;
    k = dW;  while (k--) {
      *dp++ = ((unt)(REDF**rp++ + GRNF**gp++ + BLUF**bp++))>>8; 
    }
  }
}

/*    rgbGreyFlt3(sRGB[j],&srcF[j][1],sX,sY,sW,sH,sX,sY);   */

void	/* interleave R G B channels */
rgbGreyFlt3(ppf dst, ppf *src, int dX, int dY, unt dW, unt dH, int sX, int sY)
{
ppf	scR = src[0], scG = src[1], scB = src[2]; 
pfl	dp,rp,gp,bp;
unt	k;

  for (dX *= 3, dst += dY, scR += sY, scG += sY, scB += sY; dH; dH--) {
    dp = *dst++ + dX;
    rp = *scR++ + sX;
    gp = *scG++ + sX;
    bp = *scB++ + sX;
    k  =  dW;  while(k--) {
      *dp++ = *rp++ ;
      *dp++ = *gp++ ;
      *dp++ = *bp++ ;
    }
  }
}



void	/* interleave R G B channels */
rgbGrey3UF (ppf dst, ppu src[3], int dX, int dY, unt dW, unt dH, int sX, int sY)
{
ppu	scR = src[0], scG = src[1], scB = src[2]; 
pfl	dp;
puc 	rp,gp,bp;
unt	k;

  for (dX *= 3, dst += dY, scR += sY, scG += sY, scB += sY; dH-- ; ) {
    dp = *dst++ + dX;
    rp = *scR++ + sX;
    gp = *scG++ + sX;
    bp = *scB++ + sX;
    k  =  dW;  while(k--) {
      *dp++ = *rp++ ;
      *dp++ = *gp++ ;
      *dp++ = *bp++ ;
    }
  }
}



void
ushUnc (rgst ush *sptr, rgst unc *cptr, rgst unt n)
{
  while (n--)    *sptr++ = *cptr++;
}


void
uncUsh (rgst unc *cptr, rgst ush *sptr, unt n)
{
  rgst unt r = n/8;
  while (r--) {
    cptr[0] = (unc)sptr[0];
    cptr[1] = (unc)sptr[1];
    cptr[2] = (unc)sptr[2];
    cptr[3] = (unc)sptr[3];
    cptr[4] = (unc)sptr[4];
    cptr[5] = (unc)sptr[5];
    cptr[6] = (unc)sptr[6];
    cptr[7] = (unc)sptr[7];
    sptr   += 8; 
    cptr   += 8;
    n      -= 8;
  }
  r = n % 8;
  while(r--)
    *cptr++ = (unc)(*sptr++);
}


void
fltUsh (rgst flt *fptr, rgst ush *sptr, rgst unt n)
{
  while(n--) {
    if(n>=8) {
      fptr[0] = sptr[0];
      fptr[1] = sptr[1];
      fptr[2] = sptr[2];
      fptr[3] = sptr[3];
      fptr[4] = sptr[4];
      fptr[5] = sptr[5];
      fptr[6] = sptr[6];
      fptr[7] = sptr[7];
      sptr+=8; 
      fptr+=8;
      n -= 7;
    }
    else
      *fptr++ = *sptr++;
  }
}



#if 00
puc IntToUncInvertClip (puc dst, pcint src, unt wide, unt tall)
{
    size_t count = (size_t)wide*tall;
    if (!(dst = (puc)realloc(dst,count*sizeof(unc)))) {
        fprintf(stderr,"IntToUncInvertClip: realloc failed\n");
        return(NULL);
    }
    while (count--)
#ifdef SUN
        *dst++ =       *src++;
#else
        *dst++ = (unc) *src++;
#endif
    return(dst);
}
#endif

#ifndef SUN
puc IntToUnc (puc dst, pcint src, unt wide, unt tall)
{
    size_t count = (size_t)wide*tall;
    register pcint sp = src;
    register puc  dp;
    if (!(dst = (puc)realloc(dst,count*sizeof(unc)))) {
        fprintf(stderr,"IntToUnc: realloc failed\n");
        return(NULL);
    }
    for (dp = dst; count; count--)
        *dp++ = (unc) *sp++;
    return(dst);
}
#endif

                  

puc RGBIfromRGBTI (puc dst, rpuc src, unt wide, unt tall)
{   
    size_t size = (size_t)  wide*tall*3;
    register puc dp, end;
    if ((dst = (puc)realloc(dst,size)))  {
        for (dp = dst, end = dst+size; dp != end; ) {
            *dp++ = *src++;
            *dp++ = *src++;
            *dp++ = *src++;
            src++;
        }
    }
    else fprintf(stderr,"RGBIfromGray: malloc failed on %lu bytes\n", size);
    return(dst);
}


#if 0    /*_______________________________________________________________*/
static   int DELTA_X[8] = {1, 1, 0,-1,-1,-1, 0, 1},
             DELTA_Y[8] = {0, 1, 1, 1, 0,-1,-1,-1};

#define  RGBI2G_5D(gpx,rpx,w1,d,o)                                                  \
             if (o=DELTA_X[d])  {  o = (o-w1)*2 ;  *(gpx + o) = RGBI2G(rpx + o*3);  \
                                   o += w1      ;  *(gpx + o) = RGBI2G(rpx + o*3);  \
                                   o += w1      ;  *(gpx + o) = RGBI2G(rpx + o*3);  \
                                   o += w1      ;  *(gpx + o) = RGBI2G(rpx + o*3);  \
                                   o += w1      ;  *(gpx + o) = RGBI2G(rpx + o*3);  \
                                }

#define  RGBI2G_5A(gpx,rpx,w2,d,o)                                                  \
             if (o=DELTA_Y[d])  {  o = o*w2 - 2 ;  *(gpx + o) = RGBI2G(rpx + o*3);  \
                                   o++          ;  *(gpx + o) = RGBI2G(rpx + o*3);  \
                                   o++          ;  *(gpx + o) = RGBI2G(rpx + o*3);  \
                                   o++          ;  *(gpx + o) = RGBI2G(rpx + o*3);  \
                                   o++          ;  *(gpx + o) = RGBI2G(rpx + o*3);  \
                                }
#endif /* 0 ______________________________________________________________*/


int BanksToInterleaved (puc *src, unt wide, unt tall, int nChan)
{   size_t size = (size_t)wide*tall;
    unsigned char *ret; 
    register puc  temp;
  
  if ( ! (temp = ret = (puc)malloc(sizeof(unc) * size * nChan)))  {
      fprintf(stderr,"BanksToRGBI: malloc failed on %lu bytes\n",size*nChan);
      return(-1);
      }
  switch (nChan)  {
    case 3: {
          register puc red,blu,grn;
          red = *src;
          grn = red + size;
          blu = grn + size;
          for ( ; size ; size--)  {
              *temp++ = *red++ ;
              *temp++ = *grn++ ;
              *temp++ = *blu++ ;
              }
          }
          break;

    case 4: {
          register puc red,blu,grn,alp;
          red = *src;
          grn = red + size;
          blu = grn + size;
          alp = blu + size;
          for ( ; size ; size--)  {
              *temp++ = *red++ ;
              *temp++ = *grn++ ;
              *temp++ = *blu++ ;
              *temp++ = *alp++ ;
              }
          }
          break;

    case 1:  {
          ret = *src;
          fprintf(stderr,"BanksToRGBI: called with nChan == 1\n");
	  }
          break;

    default:  {   /* The "exotic" and error cases... */

#if 1     /* this is probably the better version */
	  int j; long q;
          register puc chan;
          for (j=0; j < nChan; j++)  {
              chan = *src + j*size;
              temp = ret  + j;
              for (q = (long)size ; q ; q--, temp += nChan)  
                   *temp = *chan++ ;
              }
#else
	  int j;
          register puc *chan;
          if ( ! (chan = (puc *)malloc((size_t)nChan*sizeof(puc)))) {
              fprintf(stderr,"BanksToRGBI: failed on %d channels\n",nChan);
	      return(-1);  /* This should cover the case of (nChan < 1) */
              }
          for (j=0; j != nChan; j++)  
               chan[j] = *src + j*size;

          for ( ; size ; size--)
               for (j=0; j != nChan; j++)  
                   *temp++ = *chan[j]++ ;
          free (chan);
#endif
          }
          break;
    }  /* End of switch (nChan) */

  if (nChan > 1)  free(*src);
  *src = ret;
  return(0);
}




#if 1  /* This version, which uses the pointer itself for the loop induction variable,
          is at least slightly faster than the version following, even when compiled
          with optimizing options on. --Tested for Mac, ...@@  */
          
puc RGBIfromGray (puc src8, unt wide, unt tall)
{   size_t size = (size_t)  wide*tall*3;
    puc dst;
    register unc *dp, *end, val;
  if ((dst = (puc)malloc(size)))  {
      for (dp = dst, end = dp+size; dp != end; )   {
          val = *src8++ ;
          *dp++ = val;
          *dp++ = val;
          *dp++ = val;
          }
      }
  else fprintf(stderr,"RGBIfromGray: malloc failed on %lu bytes\n", size);
  return(dst);
}

#else  /* This version, though efficiently using the size as the loop induction var.,
          is at least slightly slower than the one using the pointer itself. */
          
puc RGBIfromGray (puc src8, size_t pixels)
{   puc dst;
    register unc *dp, val;
  if (dst = (puc)malloc(pixels*3))  {
      for (dp = dst ; pixels ; pixels--)   {
          val = *src8++ ;
          *dp++ = val;
          *dp++ = val;
          *dp++ = val;
          }
      }
  else fprintf(stderr,"RGBIfromGray: malloc failed on %lu bytes\n", pixels*3);
  return(dst);
}
#endif  /* Two versions of RGBIfromGray, using different loop schemes. */



puc    RGBItoGray (puc src24, unt wide, unt tall)
{
  register size_t pixels = (size_t)wide*tall;
  puc dst8;
  register puc p; 

  if ((p = dst8 = (puc)malloc(pixels*sizeof(unc))))  {
    for (   ; pixels ; pixels--)  {
      *p++ = (unc)(REDI*(*src24) + GRNI*(*(src24+1)) + BLUI*(*(src24+2))) ;
      src24  += 3;
      }
    }
  else fprintf(stderr,"RGBItoGray: malloc failed\n");
return(dst8);
}


void RGBItoGray5x5 (puc rip, puc gip, int wide, int x, int y)
{   long j,k,q;
    register puc rp;
    
  for (j = (y-2)*wide; j != (y+3)*wide; j += wide)
    for (k = x-2; k != x+3; k++)  {
      q = (j+k) ;
      rp = rip + q*3 ;
      *(gip+q) = RGBI2G( rp );
      }
}



puc RGBItoGray5x5p (puc rip, puc gip, int wide, int x, int y)   /* especially the return value, which should be free */
{   int j,k;                                 /* probably better to redesign this using pixel pointers */
    register unl  q  = (y-2)*wide + x-2;     /* of pointers to beginnings if pixmaps. */
    register puc  gp = gip + q;
    register puc  rp = rip + q*3;
    
  q = wide-5;
  for (j = 5; j ; j--)
    {
      for (k=5; k ; k--)
        {
          *gp++ = RGBI2G(rp);
          rp += 3;
        }
      gp += q;
      rp += q*3;
    }
rp = rip + (y*wide + x)*3;
return(rp);
}





int RGBIintoGray8 (puc src, puc dst, rgst size_t pixels)
{
for (   ; pixels ; pixels--)   {
  *dst++ = (unc)(REDI*(*src) + GRNI*(*(src+1)) + BLUI*(*(src+2)));
  src += 3;
  }
return(0);  
}


puc  GrayFromRGBI (puc src24, unt wide, unt tall)
{
  register size_t pixels = (size_t)wide*tall;
  puc dst8;
  register puc p; 

  if ((p = dst8 = (puc)malloc(pixels*sizeof(unc))))  {
    for (   ; pixels ; pixels--)  {
      *p++ = (unc)(REDI*(*src24) + GRNI*(*(src24+1)) + BLUI*(*(src24+2))) ;
      src24  += 3;
      }
    }
  else fprintf(stderr,"RGBItoGray: malloc failed\n");
return(dst8);
}






/*  Color24toGray8and24 takes a 1-dimensional 24-bit array of
 *  interleaved rgb color ("src") and makes both a 1-d 8-bit and
 *  a 1-d 24-bit array of [interleaved] gray ("dst" and "dest24")
 *  according to:
 *    (r,b,g) --> (gray,gray,gray) where gray = 0.30*r + 0.59*g + 0.11*b,
 *  as described in Starbase manuals pertinent to the HP displays.  
 *  A BadWindows call like: icon=(struct curstruct *)LoadDaveIcon(path),
 *  makes icon->src such a 1-d array of interleaved rgb color. 
 */
int Color24toGray8and24 (puc src, puc dst, puc dest24, size_t pixels)
{
for (   ; pixels ; pixels--)   {
  *dest24 = *(dest24+1) = *(dest24+2) = *dst++ = (unc)
    (REDI*(*src) + GRNI*(*(src+1)) + BLUI*(*(src+2))) ;
  dest24 += 3;
  src += 3;
  }
return(0);  
}





int RGBItoChannels (puc src24, puc gray, puc red, puc grn, puc blu, int cols, int rows)
{   register  k ;
for (           ; rows ; rows--)  {
  for (k = cols ; k    ; k--)  {
    *gray    = (unc)(0.30)*(*red++ = *src24++) ;
    *gray   += (unc)(0.59)*(*grn++ = *src24++) ;
    *gray++ += (unc)(0.11)*(*blu++ = *src24++) ;
    }
  }
return(0);  
}




int GrayToRGBI (puc gip, puc rgbi, int cols, int rows)
{ register unl q = (unl) cols*rows;
  register unc val;
  for (  ; q ; q--)  {
     val  = *gip++;
     *rgbi++ = val;
     *rgbi++ = val;
     *rgbi++ = val;
     }
  return(0);
}


int GrayToBinary (rgst unc *gray8bit, int cols, int rows, rgst unt thresh)
{         register unl q = (unl)cols*rows;
for (  ; q ; q--)
   *gray8bit++ = ((*gray8bit > (unc)thresh) ? '\1' : '\0');
return(0);
}


int RescaleBinIm (unc *bin8bit, int cols, int rows, int posval)
{         register unl q = (unl)cols*rows;
          register unc val = (unc) posval;
for (  ; q ; q--)  {
  if (*bin8bit)  *bin8bit = val;
  bin8bit++ ;
  }
return(0);
}


int RescaleBinTo24 (puc *bin8bit, puc cip24, int cols, int rows, int posval)
{         register unl q = (unl)cols*rows;
          register unc val = (unc) posval;
  memset((void *)cip24,0,(size_t)q);
  for (  ; q ; q--)  {
    if (*bin8bit)
      *cip24 = *(cip24+1) = *(cip24+2) = val;
    cip24 += 3 ;
    bin8bit++ ;
    }
  return(0);
}



int ScaleUncToFlt (puc up, pfl gipf, int cols, int rows, double range)
{             register unl  q = (unl)cols*rows;
              float scale = (float)(range/255.0);
for (   ;  q ;  q--)
  *gipf++  =  scale * (*up++) ;
return(0);  
}




int ScaleFltToUnc (float *gipf, puc up, int cols, int rows, double range)
{           register    unl q = (unl)cols*rows;
    float scale = (flt)(128.0/range);
for (   ;  q ;  q--)
  *up++ = (unc) ((*gipf++ * scale) + 127.5);
return(0);  
}

int CopyFto24 (pfl gipf, puc dest24, int cols, int rows)
{         register   unl q  =   (unl)cols*rows;
for (   ;  q ;  q--)  {
  *(dest24+2) = *(dest24+1) = *(dest24) = (unc) *gipf++ ;
  dest24 += 3 ;
  }
return(0);  
}

int Copy8to24 (puc src8, puc dest24, int cols, int rows)
{   register   unl q   =   (unl)cols*rows;
for (   ;  q ;  q--)  {
  *(dest24+2) = *(dest24+1) = *(dest24) = *src8++ ;
  dest24 += 3 ;
  }
return(0);  
}


int CopyFtoUNC (float *gipf, unc *gip8, int cols, int rows)
{   register unsigned long q = (unl) cols*rows;
while (q)  {  *(gip8++) = (unc) *(gipf++) ;  q-- ;  }
return(0);  
}


int CopyFFTtoImage8 (float *fftf, puc fft8, int fdd[2 ])
{   register unc u ;
    register unl q ;
for (q = (unl) fdd[0]*fdd[1]*2;  q ;  q--)  {
  *(fft8++) = (u = (unc) *(fftf++)) ;
  }
return(0);  
}

int CopyFFTmagTo8 (float *fftf, puc fft8, int fdd[2 ])
{   register unc u ;
    register unl k,q ;
for (q = (unl) fdd[0]*fdd[1]*2, k=0;  k != q ;  k += 2)  {
  u = (unc) sqrt((double)(fftf[k]*fftf[k] + fftf[k+1]*fftf[k+1])) ;
  *(fft8++) = u ;
  }
return(0);  
}

/*____________RealTo24:____________To display result of inverse FFT */
int RealTo24 (pfl fftf, puc xip24, int cols, int rows, int fdd[2 ])
{        register int q  ;
         int pad  = (fdd[1]-cols)*2 ;
for (          ;  rows ;  rows--)  {
  for (q = cols;     q ;  q--)  {
    *xip24 = *(xip24+1) = *(xip24+2) = (unc) *fftf ;
    xip24 += 3 ;
    fftf  += 2 ;       /* Skip imaginary part. */
    }
  fftf += pad ;
  }
return(0);  
}


int RealTo8 (float *fftf, puc gip8, int cols, int rows, int fdd[2 ])
{   int pad  = (fdd[1]-cols)*2 ;
    register int k ;
for (          ; rows ; rows--)  {
  for (k = cols; k    ; k--)  {
    *gip8++ = (unc) (*fftf++) ;
    fftf++ ;       /* Skip imaginary part. */
    }
  fftf += pad ;
  }
return(0);  
}

int CplxMagTo24 (float *fftf, puc xip24, int cols, int rows)
{   unl z = (unl)cols*rows ;
    float real, imag ;
for (   ;  z ;  z--)  {
  real = *fftf++ ;
  imag = *fftf++ ;
  *xip24 = *(xip24+1) = *(xip24+2)
         = (unc) sqrt((double)(real*real + imag*imag));
  xip24 += 3 ;
  }
return(0);  
}

int CplxMagTo8 (float *fftf, puc xip8, int cols, int rows)
{   unl z = (unl)cols*rows ;
    float real, imag ;
for (   ;  z ;  z--)  {
  real = *fftf++ ;
  imag = *fftf++ ;
  *xip8++ = (unc)(sqrt((double)(real*real + imag*imag)));
  }
return(0);  
}



int DataTo8and24 (float *fftf, puc fft8, puc fft24, int cols, int rows, int fdd[2 ])
{          register int q ;
           int pad  = (fdd[1]-cols)*2 ;
for (          ;  rows ;  rows--)  {
  for (q = cols;     q ;  q--)  {
    *fft24 = *(fft24+1) = *(fft24+2) = *fft8++ = (unc) *fftf++ ;
    fft24 += 3 ;
    fftf++ ;
    }
  fftf += pad ;
  }
return(0);  
}




int CopyMagTo8and24 (float *fftf, puc fft8, puc fft24, int cols, int rows)
{   uni z ;   /** No "register" hints.  Let compiler decide.  **/
    float real, imag ;
for (z = rows*cols;  z ;  z--)  {
  real = *fftf++ ;
  imag = *fftf++ ;
  *fft24 = *(fft24+1) = *(fft24+2) = *fft8++
         = (unc) sqrt((double)(real*real + imag*imag));
  fft24 += 3 ;
  }
return(0);  
}

