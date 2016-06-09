/* Image and Pyr I/O routines */

#include "imgdec.h"
#include "imgmacro.h"


static FIMAGE h,v;
static int hsize = 0;
static int vsize = 0;

/*****************************************************************************/

fgaussian(pyr,botlev,toplev,rk,ntaps)
FPYR pyr;
float *rk;
int botlev,toplev,ntaps;
{
   int lev;
   for (lev=botlev; lev < toplev; lev++)
     freduce(pyr[lev],pyr[lev+1],rk,ntaps);
}

flaplacian(pyr,botlev,toplev,ek,ntaps)
FPYR pyr;
float *ek;
int botlev,toplev,ntaps;
{
   int lev;
   for (lev=botlev; lev < toplev; lev++)
     fexpand(pyr[lev+1],pyr[lev],-1,ek,ntaps);
}

fexpandpyr(pyr,botlev,toplev,ek,ntaps)
FPYR pyr;
float *ek;
int botlev,toplev,ntaps;
{
   int lev;
   for (lev=toplev; lev > botlev; lev--)
     fexpand(pyr[lev],pyr[lev-1],1,ek,ntaps);
}

freduce(in,out,rk,ntaps)
FIMAGE in,out;
float *rk;
int ntaps;
{
   _freduce(in.ptr,in.w,in.h,out.ptr,out.w,out.h,rk,ntaps);
}

fexpand(in,out,mode,ek,ntaps)
FIMAGE in,out;
float *ek;
int mode,ntaps;
{
   _fexpand(in.ptr,in.w,in.h,out.ptr,out.w,out.h,mode,ek,ntaps);
}

/*****************************************************************************/
_freduce(in,iw,ih,out,ow,oh,rk,ntaps)
register float *in,*out,*rk;
int iw,ih,ow,oh,ntaps;
{
   float sum;
   int border,ntapsd2,rtype;

   border = ntapsd2 = ntaps/2;

   /* allocate scratch images */

   h.w = iw+2*border; h.h = ih;	
   if (hsize < h.w*h.h) {
     if (h.ptr == NULL)
       h.ptr = (float *) malloc(h.w*h.h*sizeof(h.ptr));
     else
       h.ptr = (float *) realloc(h.ptr,h.w*h.h*sizeof(h.ptr));
     hsize = h.w*h.h;
   }
   v.w = ow; v.h = ih+2*border;
   if (vsize < v.w*v.h) {
     if (v.ptr == NULL)
       v.ptr = (float *) malloc(v.w*v.h*sizeof(v.ptr));
     else
       v.ptr = (float *) realloc(v.ptr,v.w*v.h*sizeof(v.ptr));
     vsize = v.w*v.h;
   }

   /* copy input image into h scratch image for horizontal filtering */

   {
     register float *p,*q,*fq,*fp;
     for (q=h.ptr+border,fq=h.ptr+h.w*h.h; q < fq; q += h.w)
       for (p=q,fp=p+iw; p < fp; *p++ = *in++)
         ;
   }
   hor_reflect(h,border,rtype);		/* horizontal reflection */

   /* horizontal filtering */
   {
     register float *s,*l,*r,*f,*ff,*fin,*p,*q,*fq;
     s = v.ptr + border*v.w;
     for (q=h.ptr+border, fq=h.ptr+h.w*h.h; q < fq; q += h.w)
       for (p=q, fin=s+v.w; s < fin; p+=2) {
         sum = (*p)*(*rk);
         for (l=p-1,r=p+1,f=rk+1,ff=rk+ntapsd2; f <= ff; l--, r++)
           sum += (*l + *r)*(*f++);
         *s++ = sum;
       }
   }

   ver_reflect(v,border,rtype);		/* vertical reflection */

   /* vertical filtering */
   {
     register float *s,*b,*t,*f,*ff,*fin,*p,*q,*fq;
     for (q=v.ptr+border*v.w, fq=q+ih*v.w; q < fq; q += 2*v.w)
       for (p=q, fin=out+ow; out < fin; p++) {
         sum = (*p)*(*rk);
         for (b=p-v.w,t=p+v.w,f=rk+1,ff=rk+ntapsd2; f <= ff; b -= v.w, t += v.w)
          sum += (*b + *t)*(*f++);
         *out++ = sum;
      }
   }
}
    
/*****************************************************************************/
_fexpand(in,iw,ih,out,ow,oh,mode,ek,ntaps)
register float *in,*out,*ek;
int iw,ih,ow,oh,mode,ntaps;
{
   float sum;
   int border,ntapsd2,rtype;

   mode = 4*mode;
   border = ntapsd2 = ntaps/2;

   /* allocate scratch images */

   v.w = iw; v.h = ih+2*border;	
   if (vsize < v.w*v.h) {
     if (v.ptr == NULL)
       v.ptr = (float *) malloc(v.w*v.h*sizeof(v.ptr));
     else
       v.ptr = (float *) realloc(v.ptr,v.w*v.h*sizeof(v.ptr));
     vsize = v.w*v.h;
   }
   h.w = iw+2*border; h.h = oh;
   if (hsize < h.w*h.h) {
     if (h.ptr == NULL)
       h.ptr = (float *) malloc(h.w*h.h*sizeof(h.ptr));
     else
       h.ptr = (float *) realloc(h.ptr,h.w*h.h*sizeof(h.ptr));
     hsize = h.w*h.h;
   }

   /* copy input image into v scratch image for vertical filtering */

   {
     register float *p,*fin;
     for (p=v.ptr+border*v.w, fin=in+iw*ih; in < fin; *p++ = *in++)
         ;
   }
   ver_reflect(h,border,rtype);		/* vertical reflection */

   /* vertical filtering */
   {
     register float *s,*b,*t,*f,*ff,*p,*q,*fp,*fq;
     register int off=2*v.w;
     s = v.ptr + border*v.w;
     for (q=h.ptr+border, fq=h.ptr+h.w*MIN(h.h,2*ih); q < fq; q += 2*h.w)
       for (p=q, fp=p+v.w; p < fp; p++, s++) {
         sum = (*s)*(*ek);
         for (b=s-off,t=s+off,f=ek+2,ff=ek+ntapsd2; f <= ff; b-=off,t+=off,f+=2)
           sum += (*b + *t)*(*f);
         *p = sum;
         sum = 0.0;
         for (b=s-v.w,t=s+v.w,f=ek+1,ff=ek+ntapsd2; f <= ff; b-=off,t+=off,f+=2)
           sum += (*b + *t)*(*f);
         *(p+h.w) = sum;
       }
     if (h.h % 2 != 1)			/* last row for odd case */
       for (p=h.ptr+border+(h.h-1)*h.w, fp=p+v.w; p < fp; p++, s++) {
         sum = (*s)*(*ek);
         for (b=s-off,t=s+off,f=ek+2,ff=ek+ntapsd2; f <= ff; b-=off,t+=off,f+=2)
           sum += (*b + *t)*(*f);
         *p = sum;
       }
   }

   hor_reflect(h,border,rtype);		/* horizontal reflection */

   /* horizontal filtering */
   {
     register float *s,*l,*r,*f,*ff,*p,*q,*fp,*fq;
     for (q=h.ptr+border, fq=h.ptr+h.h*h.w; q < fq; q += h.w) {
       for (p=q, fp=p+MIN(ow,2*iw); p < fp; p+=2) {
         sum = (*p)*(*ek);
         for (l=p-2,r=p+2,f=ek+2,ff=ek+ntapsd2; f <= ff; l-=2, r+=2, f+=2)
           sum += (*l + *r)*(*f);
         *out++ += mode*sum;
         sum = 0.0;
         for (l=p-1,r=p+1,f=ek+1,ff=ek+ntapsd2; f <= ff; l-=2, r+=2, f+=2)
           sum += (*l + *r)*(*f);
         *out++ += mode*sum;
       }
       if (ow %2 != 1)	{		/* last pixel for odd case */
         sum = (*p)*(*ek);
         for (l=p-2,r=p+2,f=ek+2,ff=ek+ntapsd2; f <= ff; l-=2, r+=2, f+=2)
           sum += (*l + *r)*(*f);
         *out++ += mode*sum;
       }
     }
   }
}
    
/*****************************************************************************/


