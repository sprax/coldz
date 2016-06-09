/* Filter routines */
/* No kernel symmetries are used. */

#include "imgdec.h"
#include "imgmacro.h"

/*****************************************************************************/

hor_filter(in,out,f)
FIMAGE in,out;
FILT1D f;
{
  register float **p=in.ptr,**q=out.ptr,*k=f.k;
  float val;
  int i,j,n,lb=f.l, ub=f.r;

  for (j=0; j < in.h; j++)
    for (i=0; i < in.w; i++) {
      for (n=lb,val=0.0; n <= ub; n++)
        val += p[j][i-n]*k[n];
      q[j][i] = val;
    }
}
 
/*****************************************************************************/

ver_filter(in,out,f)
FIMAGE in,out;
FILT1D f;
{
  register float **p=in.ptr,**q=out.ptr,*k=f.k;
  float val;
  int i,j,n,lb = f.l, ub = f.r;

  for (j=0; j < in.h; j++) 
    for (i=0; i < in.w; i++) {
      for (n=lb,val=0.0; n <= ub; n++)
        val += p[j-n][i]*k[n];
      q[j][i] = val;
    }
}
 
/*****************************************************************************/
/* Valid for odd-sized symmetric filter kernels only */

hor_sym_filter(in,out,f)
FIMAGE in,out;
FILTER f;
{
  register float **p=in.ptr,**q=out.ptr,*k=f.k;
  float val;
  int i,j,n,nlim = f.taps/2;

  for (j=0; j < in.h; j++)
    for (i=0; i < in.w; i++) {
      for (n=1,val=p[j][i]*k[0]; n <= nlim; n++)
        val += (p[j][i-n]+p[j][i+n])*k[n];
      q[j][i] = val;
    }
}
 
/*****************************************************************************/
/* Valid for odd-sized symmetric filter kernels only */
    
ver_sym_filter(in,out,f)
FIMAGE in,out;
FILTER f;
{
  register float **p=in.ptr,**q=out.ptr,*k=f.k;
  float val;
  int i,j,n,nlim = f.taps/2;

  for (j=0; j < in.h; j++) 
    for (i=0; i < in.w; i++) {
      for (n=1,val=p[j][i]*k[0]; n <= nlim; n++)
        val += (p[j-n][i]+p[j+n][i])*k[n];
      q[j][i] = val;
    }
}
 
/*****************************************************************************/
    
filter_2d(in,out,f)
FIMAGE in,out;
FILT2D f;
{
  register float **p=in.ptr,**q=out.ptr,*k,**kp;
  float val;
  int i,j,y,ny,nx,l=f.l,r=f.r,b=f.b,t=f.t;

  for (j=0; j < in.h; j++) 
    for (i=0; i < in.w; i++) {
      for (kp=f.k+b,ny=b,val=0.0; ny <= t; ny++,kp++)
        for (k=l+*kp,y=j-ny,nx=l; nx <= r; nx++,k++) 
          val += p[y][i-nx]*(*k);
      q[j][i] = val;
    }
}
 
/*****************************************************************************/

minimum_filter(img,w,h)
FIMAGE img;
int w,h;
{
  FIMAGE scr;
  float **s,**p,min;
  int half_w=w/2,half_h=h/2,i,j,x,y;

  scr.w = img.w;  scr.h = img.h;  alloc_fimage(&scr);
  s = scr.ptr;  p=img.ptr;
  hor_reflect(img,half_w,3);
  for (j=0; j < img.h; j++)
    for (i=0; i < img.w; i++) {
      min=p[j][i];
      for (x=i-half_w; x <= i+half_w; x++) {
        if (min > p[j][x])
          min = p[j][x];
      }
      s[j][i] = min;
    }
  ver_reflect(scr,half_h,3);
  for (j=0; j < img.h; j++)
    for (i=0; i < img.w; i++) {
      min=s[j][i];
      for (y=j-half_h; y <= j+half_h; y++) {
        if (min > s[y][i])
          min = s[y][i];
      }
      p[j][i] = min;
    }
  free_fimage(scr);
}

maximum_filter(img,w,h)
FIMAGE img;
int w,h;
{
  FIMAGE scr;
  float **s,**p,max;
  int half_w=w/2,half_h=h/2,i,j,x,y;

  scr.w = img.w;  scr.h = img.h;  alloc_fimage(&scr);
  s = scr.ptr;  p=img.ptr;
  hor_reflect(img,half_w,3);
  for (j=0; j < img.h; j++)
    for (i=0; i < img.w; i++) {
      max=p[j][i];
      for (x=i-half_w; x <= i+half_w; x++) {
        if (max < p[j][x])
          max = p[j][x];
      }
      s[j][i] = max;
    }
  ver_reflect(scr,half_h,3);
  for (j=0; j < img.h; j++)
    for (i=0; i < img.w; i++) {
      max=s[j][i];
      for (y=j-half_h; y <= j+half_h; y++) {
        if (max < s[y][i])
          max = s[y][i];
      }
      p[j][i] = max;
    }
  free_fimage(scr);
}

/*****************************************************************************/
/* Valid for symmetric filter kernels only */

hor_decimate2_sym(in,out,f)
FIMAGE in,out;
FILTER f;
{
  if (f.taps%2)
    hor_decimate2_odd(in,out,f);
  else
    hor_decimate2_even(in,out,f);
}


/* Valid for odd-sized symmetric filter kernels only */

hor_decimate2_odd(in,out,f)
FIMAGE in,out;
FILTER f;
{
  register float **p=in.ptr,**q=out.ptr,*k=f.k;
  float val;
  int i,id2,j,n,nlim = f.taps/2;

  for (j=0; j < in.h; j++)
    for (i=id2=0; i < in.w; i+=2,id2++) {
      for (n=1,val=p[j][i]*k[0]; n <= nlim; n++)
        val += (p[j][i-n]+p[j][i+n])*k[n];
      q[j][id2] = val;
    }
}


/* Valid for even-sized symmetric filter kernels only */

hor_decimate2_even(in,out,f)
FIMAGE in,out;
FILTER f;
{
  register float **p=in.ptr,**q=out.ptr,*k=f.k;
  float val;
  int i,id2,j,n,nlim = f.taps/2;

  for (j=0; j < in.h; j++)
    for (i=id2=0; i < in.w; i+=2,id2++) {
      for (n=1,val=0.0; n <= nlim; n++)
        val += (p[j][i-n+1]+p[j][i+n])*k[n];
      q[j][id2] = val;
    }
}

/*****************************************************************************/
/* Valid for symmetric filter kernels only */

ver_decimate2_sym(in,out,f)
FIMAGE in,out;
FILTER f;
{
  if (f.taps%2)
    ver_decimate2_odd(in,out,f);
  else
    ver_decimate2_even(in,out,f);
}


/* Valid for odd-sized symmetric filter kernels only */

ver_decimate2_odd(in,out,f)
FIMAGE in,out;
FILTER f;
{
  register float **p=in.ptr,**q=out.ptr,*k=f.k;
  float val;
  int i,j,jd2,n,nlim = f.taps/2;

  for (j=jd2=0; j < in.h; j+=2,jd2++) 
    for (i=0; i < in.w; i++) {
      for (n=1,val=p[j][i]*k[0]; n <= nlim; n++)
        val += (p[j-n][i]+p[j+n][i])*k[n];
      q[jd2][i] = val;
    }
}
 

/* Valid for even-sized symmetric filter kernels only */

ver_decimate2_even(in,out,f)
FIMAGE in,out;
FILTER f;
{
  register float **p=in.ptr,**q=out.ptr,*k=f.k;
  float val;
  int i,j,jd2,n,nlim = f.taps/2;

  for (j=jd2=0; j < in.h; j++,jd2++)
    for (i=0; i < in.w; i++) {
      for (n=1,val=0.0; n <= nlim; n++)
        val += (p[j-n+1][i]+p[j+n][i])*k[n];
      q[jd2][i] = val;
    }
}


