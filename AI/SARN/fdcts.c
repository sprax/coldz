#include "imgdec.h"
#include <un/unMath.h>

#define PI M_PI

#define odd(n)  (n % 2 == 1)
#define even(n) (n % 2 == 0)

#ifndef _NO_PROTO
static int ipow(int x, int n);
static int Log2(int n);
static int bitrev(int i, int n);
static void shuffle(float x[],int n);
#else
static int ipow();
static int Log2();
static int bitrev();
static void shuffle();
#endif


float C1PI4 = 0.70710678118654746F;
float *costab[20];

static int INITIALIZED = 2;     /* flag indicating dct coeffs initialized */


static int ipow(x,n)
int x,n;
{
   int i,power;
   for (i=0, power=1; i < n; power *= x, i++)
     ;
   return(power);
}


static int Log2(n)
int n;
{
   int i,j=0;

   if (n < 0) 
     fprintf(stderr,"Log2 : logarithm of negative number\n");
   else if (n == 0)
     fprintf(stderr,"Log2 : logarithm of zero\n");
   else {
     for (i=1, j=0; i < n; i <<= 1, j++)
       ;
   }
   return(j);
}


static int bitrev(i,n)
int i,n;
{
   int irev = 0;
   int k;

   for (k=0; k < n; k++, i>>=1 )
     irev = (irev << 1) + (i & 01);
   return(irev);
}


static void shuffle(x,n)
   float x[];
   int n;
{
   int j = 0;
   int i,m;
   float temp;

   for (i=0; i < n; i++) {
     if (j > i) {
       temp = x[j]; x[j] = x[i]; x[i] = temp;
     }
     m = n/2;
     while (m >= 2 && j >= m) {
       j = j - m;   m /= 2;
     }
     j = j + m;
   }
}

dct_init(n)
int n;
{
  float *p;
  double angle,theta;
  int i,j,k,m;

  if (INITIALIZED >= n) 
    return(0);
  for (i=INITIALIZED; i < n; i *= 2) {
    k = Log2(i/2); m = Log2(2*i);
    angle = PI/(4*i);
    p = costab[k] = (float *) malloc(i*sizeof(float));
    for (j=0; j < i; j++) {
      theta = angle*bitrev(i+j,m);
      p[j] = (float) cos(theta);
    }
  }

  INITIALIZED = n;
  return(0);
}


dct_2d(img)
FIMAGE img;
{
   float **fin,*row,*fr;
   register float *p,*r,**x=img.ptr;
   int i;

   for (fin=x+img.h; x < fin; x++)
     dct(*x,img.w);

   row = (float *) malloc(img.h*sizeof(float));
   x = img.ptr; fr = row+img.h;
   for (i=0; i < img.w; i++) {
     for (x=img.ptr, p= *x+i, r=row; r < fr; p= *++x+i, r++)
       *r = *p;
     dct(row,img.h);
     for (x=img.ptr, p= *x+i, r=row; r < fr; p= *++x+i, r++)
       *p = *r;
   }
   free(row);
}

idct_2d(img)
FIMAGE img;
{
   float **fin,*row,*fr;
   register float *p,*r,**x=img.ptr;
   int i;

   for (fin=x+img.h; x < fin; x++)
     idct(*x,img.w);

   row = (float *) malloc(img.h*sizeof(float));
   x = img.ptr; fr = row+img.h;
   for (i=0; i < img.w; i++) {
     for (x=img.ptr, p= *x+i, r=row; r < fr; p= *++x+i, r++)
       *r = *p;
     idct(row,img.h);
     for (x=img.ptr, p= *x+i, r=row; r < fr; p= *++x+i, r++)
       *p = *r;
   }
   free(row);
}


dct2d(img)
FIMAGE img;
{
   float **x=img.ptr,**y,**fin;
   int nx=img.w, ny=img.h;
   register float **rp,**rq,**r;
   register float *p,*q,*fp;
   float t;
   register float *c,*s;
   float scale;
   int i,j,k,l,m,n,nn,nstages,id8;

   for (y=x,fin=x+ny; y < fin; y++)
     dct(*y,nx);

   if (INITIALIZED < ny)
     dct_init(ny);

   for (m=ny; m >= 4; m /= 2)
     for (rp=x, rq=rp+(m-1); rp < rq; rp++, rq--) 
       for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
         t = *p;    *p = t + *q;    *q = t - *q;
     }

   rp = x; rq = rp + 1;
   for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
     t = *p;   *p = C1PI4*(t+*q);   *q = C1PI4*(t-*q);
   }

   for (n=2, y = x+n; n < ny; n *= 2, y = x+n) {
     nn = 2*n; nstages = 2*Log2(nn)-3;
     for (m=nstages; m > 0; m--) {
       if (m == 1) {
         c = costab[Log2(n/2)]; s = c+n-1;
         for (rp=y, rq=y+(n-1); rp < rq; rp++, rq--, c++, s--)
           for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
             t = *p;  *p = t*(*s) + *q*(*c);  *q = -t*(*c) + *q*(*s);
           }
       }
       else if (m == nstages) 
         for (rp=y+(n/4), rq=rp+(n/2-1); rp < rq; rp++, rq--)
           for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
             t = *p;   *p = C1PI4*(*q-t);    *q = C1PI4*(*q+t);
           }
       else if (odd(m)) {
         i = nn/ipow(2,(m-1)/2);  id8 = i/8;  k = n/i;
         c = costab[Log2(id8)]; s = c+i/4-1;
         rp = y; rq = y+(n-1); 
         for (j=0; j < id8; j++, c++, s--) {
           rp += k; rq -= k;
           for (r=rp+k; rp < r; rp++, rq--)
             for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
               t = *p;  *p = -t*(*c) + *q*(*s);  *q = t*(*s) + *q*(*c);
             }
           for (r=rp+k; rp < r; rp++, rq--)
             for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
               t = *p;  *p = -t*(*s) - *q*(*c);  *q = -t*(*c) + *q*(*s);
             }
           rp += k; rq -= k;
         }
       }
       else if (even(m)) {
         k = ipow(2,m/2);  j = n/(2*k);
         for (i=0, r=y; i < j; i++, r += 2*k) {
           for (rp=r, rq=rp+(k-1); rp < rq; rp++, rq--)
             for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
               t = *p;  *p = t + *q;  *q = t - *q;
             }
           for (rp=r+k, rq=rp+(k-1); rp < rq; rp++, rq--)
             for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
               t = *p;  *p = -t + *q;  *q = t + *q;
             }
         }
       }
     }
   }

   for (i=0, j=0; i < ny; i++) {
     if (j > i)
       for (p=x[j], q=x[i], fp=p+nx; p < fp; p++, q++) {
         t = *p; *p = *q; *q = t;
       }
     m = ny/2;
     while (m >= 2 && j >= m) {
       j = j - m;   m /= 2;
     }
     j = j + m;
   }

   scale = 2.0/((float) ny);
   for (rp=x, fin=rp+nx; rp < fin; rp++)
     for (p= *rp, fp=p+nx; p < fp; p++) 
       *p *= scale;
   
}


idct2d(img)
FIMAGE img;
{
   float **x=img.ptr,**y,**fin;
   int nx=img.w, ny=img.h;
   register float **rp,**rq,**r;
   register float *p,*q,*fp;
   float t;
   register float *c,*s;
   int i,j,k,l,m,n,nn,nstages,id8;

   for (y=x,fin=x+ny; y < fin; y++)
     idct(*y,nx);

   if (INITIALIZED < ny)
     dct_init(ny);

   for (i=0, j=0; i < ny; i++) {
     if (j > i)
       for (p=x[j], q=x[i], fp=p+nx; p < fp; p++, q++) {
         t = *p; *p = *q; *q = t;
       }
     m = ny/2;
     while (m >= 2 && j >= m) {
       j = j - m;   m /= 2;
     }
     j = j + m;
   }

   rp = x; rq = rp + 1;
   for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
     t = *p;   *p = C1PI4*(t+*q);   *q = C1PI4*(t-*q);
   }

   for (n=2, y = x+n; n < ny; n *= 2, y = x+n) {
     nn = 2*n; nstages = 2*Log2(nn)-3;
     for (m=1; m <= nstages; m++) {
       if (m == 1) {
         c = costab[Log2(n/2)]; s = c+n-1;
         for (rp=y, rq=y+(n-1); rp < rq; rp++, rq--, c++, s--)
           for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
             t = *p;  *p = t*(*s) - *q*(*c);  *q = t*(*c) + *q*(*s);
           }
       }
       else if (m == nstages) 
         for (rp=y+(n/4), rq=rp+(n/2-1); rp < rq; rp++, rq--)
           for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
             t = *p;   *p = C1PI4*(*q-t);    *q = C1PI4*(*q+t);
           }
       else if (odd(m)) {
         i = nn/ipow(2,(m-1)/2);  id8 = i/8;  k = n/i;
         c = costab[Log2(id8)]; s = c+i/4-1;
         rp = y; rq = y+(n-1); 
         for (j=0; j < id8; j++, c++, s--) {
           rp += k; rq -= k;
           for (r=rp+k; rp < r; rp++, rq--)
             for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
               t = *p;  *p = -t*(*c) + *q*(*s);  *q = t*(*s) + *q*(*c);
               }
           for (r=rp+k; rp < r; rp++, rq--)
             for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
               t = *p;  *p = -t*(*s) - *q*(*c);  *q = -t*(*c) + *q*(*s);
             }
           rp += k; rq -= k;
         }
       }
       else if (even(m)) {
         k = ipow(2,m/2);  j = n/(2*k);
         for (i=0, r=y; i < j; i++, r += 2*k) {
           for (rp=r, rq=rp+(k-1); rp < rq; rp++, rq--)
             for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
               t = *p;  *p = t + *q;  *q = t - *q;
             }
           for (rp=r+k, rq=rp+(k-1); rp < rq; rp++, rq--)
             for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
               t = *p;  *p = -t + *q;  *q = t + *q;
             }
         }
       }
     }
   }

   for (m=4; m <= ny; m *= 2)
     for (rp=x, rq=rp+(m-1); rp < rq; rp++, rq--) 
       for (p= *rp, q= *rq, fp=p+nx; p < fp; p++, q++) {
         t = *p;    *p = t + *q;    *q = t - *q;
     }
}





dct(x,nx)
float *x;
int nx;
{
   register float *p,*q;
   float *r,*y,t;
   register float *c,*s;
   float scale;
   int i,j,k,l,m,n,nn,nstages,id8;

   if (INITIALIZED < nx)
     dct_init(nx);

   for (m=nx; m >= 4; m /= 2)
     for (p=x,q=p+m-1; p < q; p++,q--) {
       t = *p;    *p = t + *q;    *q = t - *q;
     }

   p = x; q = p + 1;
   t = *p;   *p = C1PI4*(t+*q);   *q = C1PI4*(t-*q);

   for (n=2, y = x+n; n < nx; n *= 2, y = x+n) {
     nn = 2*n; nstages = 2*Log2(nn)-3;
     for (m=nstages; m > 0; m--) {
       if (m == 1) {
         c = costab[Log2(n/2)]; s = c+n-1;
         for (p=y, q=y+n-1; p < q; p++, q--, c++, s--) {
           t = *p;  *p = t*(*s) + *q*(*c);  *q = -t*(*c) + *q*(*s);
         }
       }
       else if (m == nstages) 
         for (p=y+n/4, q=p+n/2-1; p < q; p++, q--) {
           t = *p;   *p = C1PI4*(*q-t);    *q = C1PI4*(*q+t);
         }
       else if (odd(m)) {
         i = nn/ipow(2,(m-1)/2);  id8 = i/8;  k = n/i;
         c = costab[Log2(id8)]; s = c+i/4-1;
         p = y; q = y+n-1; 
         for (j=0; j < id8; j++, c++, s--) {
           p += k; q -= k;
           for (r=p+k; p < r; p++, q--) {
             t = *p;  *p = -t*(*c) + *q*(*s);  *q = t*(*s) + *q*(*c);
           }
           for (r=p+k; p < r; p++, q--) {
             t = *p;  *p = -t*(*s) - *q*(*c);  *q = -t*(*c) + *q*(*s);
           }
           p += k; q -= k;
         }
       }
       else if (even(m)) {
         k = ipow(2,m/2);  j = n/(2*k);
         for (i=0, r=y; i < j; i++, r += 2*k) {
           for (p=r, q=p+k-1; p < q; p++, q--) {
             t = *p;  *p = t + *q;  *q = t - *q;
           }
           for (p=r+k, q=p+k-1; p < q; p++, q--) {
             t = *p;  *p = -t + *q;  *q = t + *q;
           }
         }
       }
     }
   }

   shuffle(x,nx);
   scale = 2.0/((float) nx);
   for (p=x, r=x+nx; p < r; p++)
     *p *= scale;
   
}



idct(x,nx)
float *x;
int nx;
{
   register float *p,*q;
   float *r,*y,t;
   register float *c,*s;
   int i,j,k,l,m,n,nn,nstages,id8;

   if (INITIALIZED < nx)
     dct_init(nx);

   shuffle(x,nx);
   
   p = x; q = p + 1;
   t = *p;   *p = C1PI4*(t+*q);   *q = C1PI4*(t-*q);

   for (n=2, y = x+n; n < nx; n *= 2, y = x+n) {
     nn = 2*n; nstages = 2*Log2(nn)-3;
     for (m=1; m <= nstages; m++) {
       if (m == 1) {
         c = costab[Log2(n/2)]; s = c+n-1;
         for (p=y, q=y+n-1; p < q; p++, q--, c++, s--) {
           t = *p;  *p = t*(*s) - *q*(*c);  *q = t*(*c) + *q*(*s);
         }
       }
       else if (m == nstages) 
         for (p=y+n/4, q=p+n/2-1; p < q; p++, q--) {
           t = *p;   *p = C1PI4*(*q-t);    *q = C1PI4*(*q+t);
         }
       else if (odd(m)) {
         i = nn/ipow(2,(m-1)/2);  id8 = i/8;  k = n/i;
         c = costab[Log2(id8)]; s = c+i/4-1;
         p = y; q = y+n-1; 
         for (j=0; j < id8; j++, c++, s--) {
           p += k; q -= k;
           for (r=p+k; p < r; p++, q--) {
             t = *p;  *p = -t*(*c) + *q*(*s);  *q = t*(*s) + *q*(*c);
           }
           for (r=p+k; p < r; p++, q--) {
             t = *p;  *p = -t*(*s) - *q*(*c);  *q = -t*(*c) + *q*(*s);
           }
           p += k; q -= k;
         }
       }
       else if (even(m)) {
         k = ipow(2,m/2);  j = n/(2*k);
         for (i=0, r=y; i < j; i++, r += 2*k) {
           for (p=r, q=p+k-1; p < q; p++, q--) {
             t = *p;  *p = t + *q;  *q = t - *q;
           }
           for (p=r+k, q=p+k-1; p < q; p++, q--) {
             t = *p;  *p = -t + *q;  *q = t + *q;
           }
         }
       }
     }
   }

   for (m=4; m <= nx; m *= 2)
     for (p=x,q=p+m-1; p < q; p++,q--) {
       t = *p;    *p = t + *q;    *q = t - *q;
     }

}



dct1d(x,nx)
float *x;
int nx;
{
   float *p,*q,*r,*y,t;
   double theta,ang;
   float c,s,scale;
   int i,j,k,l,m,n,nn,nstages,nbit,bitrev();
   int id2,id4,id8;


   for (m=nx; m >= 4; m /= 2)
     for (p=x,q=p+m-1; p < q; p++,q--) {
       t = *p;    *p = t + *q;    *q = t - *q;
     }

   p = x; q = p + 1;
   t = *p;   *p = C1PI4*(t+*q);   *q = C1PI4*(t-*q);

   for (n=2, y = x+n; n < nx; n *= 2, y = x+n) {
     nn = 2*n; nstages = 2*Log2(nn)-3;
     for (m=nstages; m > 0; m--) {
       if (m == 1) {
         ang = PI/(2*nn); nbit = Log2(nn);
         for (p=y, q=y+n-1; p < q; p++, q--) {
           theta = ang*bitrev(p-x,nbit);   c = cos(theta);  s = sin(theta);
           t = *p;  *p = t*s + *q*c;  *q = -t*c + *q*s;
         }
       }
       else if (m == nstages) 
         for (p=y+n/4, q=p+n/2-1; p < q; p++, q--) {
           t = *p;   *p = C1PI4*(*q-t);    *q = C1PI4*(*q+t);
         }
       else if (odd(m)) {
         i = nn/ipow(2,(m-1)/2);  id2 = i/2;  id4 = i/4;  id8 = i/8;
         nbit = Log2(id2);  k = n/i;   ang = PI/i;
         p = y; q = y+n-1; 
         for (j=0; j < id8; j++) {
           theta = ang*bitrev(id4+j,nbit);  c = cos(theta);  s = sin(theta);
           p += k; q -= k;
           for (r=p+k; p < r; p++, q--) {
             t = *p;  *p = -t*c + *q*s;  *q = t*s + *q*c;
           }
           for (r=p+k; p < r; p++, q--) {
             t = *p;  *p = -t*s - *q*c;  *q = -t*c + *q*s;
           }
           p += k; q -= k;
         }
       }
       else if (even(m)) {
         k = ipow(2,m/2);  j = n/(2*k);
         for (i=0, r=y; i < j; i++, r += 2*k) {
           for (p=r, q=p+k-1; p < q; p++, q--) {
             t = *p;  *p = t + *q;  *q = t - *q;
           }
           for (p=r+k, q=p+k-1; p < q; p++, q--) {
             t = *p;  *p = -t + *q;  *q = t + *q;
           }
         }
       }
     }
   }

   shuffle(x,nx);
   scale = 2.0/((float) nx);
   for (p=x, r=x+nx; p < r; p++)
     *p *= scale;
   
}



idct1d(x,nx)
float *x;
int nx;
{
   float *p,*q,*r,*y,t;
   double theta,ang;
   float c,s;
   int i,j,k,l,m,n,nn,nstages,nbit,bitrev();
   int id2,id4,id8;

   shuffle(x,nx);
   
   p = x; q = p + 1;
   t = *p;   *p = C1PI4*(t+*q);   *q = C1PI4*(t-*q);

   for (n=2, y = x+n; n < nx; n *= 2, y = x+n) {
     nn = 2*n; nstages = 2*Log2(nn)-3;
     for (m=1; m <= nstages; m++) {
       if (m == 1) {
         ang = PI/(2*nn); nbit = Log2(nn);
         for (p=y, q=y+n-1; p < q; p++, q--) {
           theta = ang*bitrev(p-x,nbit);   c = cos(theta);  s = sin(theta);
           t = *p;  *p = t*s - *q*c;  *q = t*c + *q*s;
         }
       }
       else if (m == nstages) 
         for (p=y+n/4, q=p+n/2-1; p < q; p++, q--) {
           t = *p;   *p = C1PI4*(*q-t);    *q = C1PI4*(*q+t);
         }
       else if (odd(m)) {
         i = nn/ipow(2,(m-1)/2);  id2 = i/2;  id4 = i/4;  id8 = i/8;
         nbit = Log2(id2);  k = n/i;   ang = PI/i;
         p = y; q = y+n-1; 
         for (j=0; j < id8; j++) {
           theta = ang*bitrev(id4+j,nbit);  c = cos(theta);  s = sin(theta);
           p += k; q -= k;
           for (r=p+k; p < r; p++, q--) {
             t = *p;  *p = -t*c + *q*s;  *q = t*s + *q*c;
           }
           for (r=p+k; p < r; p++, q--) {
             t = *p;  *p = -t*s - *q*c;  *q = -t*c + *q*s;
           }
           p += k; q -= k;
         }
       }
       else if (even(m)) {
         k = ipow(2,m/2);  j = n/(2*k);
         for (i=0, r=y; i < j; i++, r += 2*k) {
           for (p=r, q=p+k-1; p < q; p++, q--) {
             t = *p;  *p = t + *q;  *q = t - *q;
           }
           for (p=r+k, q=p+k-1; p < q; p++, q--) {
             t = *p;  *p = -t + *q;  *q = t + *q;
           }
         }
       }
     }
   }

   for (m=4; m <= nx; m *= 2)
     for (p=x,q=p+m-1; p < q; p++,q--) {
       t = *p;    *p = t + *q;    *q = t - *q;
     }

}


