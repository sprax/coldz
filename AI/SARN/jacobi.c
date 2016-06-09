/* Routines from Numerical Recipes in C */

#include <stdio.h>
#include <math.h>

#define ABS(x) (((x) > 0) ? (x) : -(x))
#define MAXROT 50
#define ROTATE(a,i,j,k,l) g=a[i][j]; h=a[k][l]; a[i][j]=g-s*(h+g*tau);\
                          a[k][l]=h+s*(g-h*tau)

/* Computes all eigenvalues and eigenvectors of a real symmetric matrix
   a[1..n][1..n].  On output, elements of above the diagonal are destroyed
   d[1..n] returns the eigenvalues of a. v[1..n][1..n] is a matrix whose 
   columns contain, on output, the normalized eigenvectors of a. nrot 
   returns the number of Jacobi rotations that were required.
*/

jacobi(a,n,d,v,nrot)
float **a,d[],**v;
int n,*nrot;
{
   int j,iq,ip,i;
   float thresh,theta,tau,t,sm,s,h,g,c;
   float *b,*z,*alloc_fvector();

   b = alloc_fvector(1,n);
   z = alloc_fvector(1,n);
   for (ip=1; ip <= n; ip++) {	/* initialize to the identity matrix */
     for (iq=1; iq <= n; iq++)
       v[ip][iq] = 0.0;
     v[ip][ip] = 1.0;
   }
   for (ip=1; ip <= n; ip++) {
     b[ip]=d[ip]=a[ip][ip];	/* initialize b and d to the diagonal of a */
     z[ip] = 0.0;
   }
   *nrot = 0;
   for (i=1; i <= MAXROT; i++) {
     for (sm=0.0,ip=1; ip <= n-1; ip++) {	/* Sum off-diagonal elements */
       for (iq=ip+1; iq <= n; iq++)
         sm += ABS(a[ip][iq]);
     }
     if (sm == 0.0) {		/* The normal return, which relies on */
       free_fvector(z,1,n);	/* quadratic convergence to machine underflow */
       free_fvector(b,1,n);
       return;
     }
     if (i < 4)
       thresh = 0.2*sm/(n*n);	/* On the first three sweeps ...*/
     else
       thresh = 0.0;		/* thereafter ... */
     for (ip = 1; ip <= n-1; ip++) {
       for (iq = ip+1; iq <= n; iq++) {
         g = 100.0*ABS(a[ip][iq]);
/* After 4 sweeps, skip rotation if the off-diagonal element is small */
         if (i > 4 && ABS(d[ip])+g == ABS(d[ip])
                   && ABS(d[iq])+g == ABS(d[iq]))
            a[ip][iq] = 0.0;
         else if (ABS(a[ip][iq]) > thresh) {
            h=d[iq]-d[ip];
            if (ABS(h)+g == ABS(h))
              t = (a[ip][iq])/h;	/* t = 1/(2*theta) */
            else {
              theta = 0.5*h/(a[ip][iq]);
              t = 1.0/(ABS(theta)+sqrt(1.0+theta*theta));
              if (theta < 0.0) t = -t;
            }
            c = 1.0/sqrt(1+t*t);
            s = t*c;
            tau = s/(1.0+c);
            h = t*a[ip][iq];
            z[ip] -= h;   z[iq] += h;   d[ip] -= h;   d[iq] += h;
            a[ip][iq] = 0.0;
            for (j=1; j <= ip-1; j++) {	/* case of rotations 1 <= j < p */
               ROTATE(a,j,ip,j,iq);
            }
            for (j=ip+1; j <= iq-1; j++) {/* case of rotations p < j < q */
               ROTATE(a,ip,j,j,iq);
            }
            for (j=iq+1; j <= n; j++) {	/* case of rotations q < j <= n */
               ROTATE(a,ip,j,iq,j);
            }
            for (j=1; j <= n; j++) {
               ROTATE(v,j,ip,j,iq);
            }
            ++(*nrot);
         }
       }
     }
     for (ip=1; ip <= n; ip++) {
       b[ip] += z[ip];
       d[ip] = b[ip];
       z[ip] = 0.0;
     }
   }
   fprintf(stderr,"jacobi: too many iterations\n");
   exit(-1);
}

