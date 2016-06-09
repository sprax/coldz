/******************************************************************************
 lksub_fwd.c    S.R.Lines (spraxlo@ai.mit.edu)   93.9.17
 FORWARD Lucas-Kanade optical flow algorithm,
 based on lsqsub.c by Rajesh Hingorani, RCA Sarnoff Labs.
#define DEBUG
 *****************************************************************************/

#define  OLD_ANSI_MATHERR /* SGI only: defines struct exception, matherr(); */
#include "sarnoff/imgdec.h"
#include "sarnoff/imgmacro.h"
#undef   OLD_ANSI_MATHERR /* SGI only: see /usr/include/math.h */

#include "fimgop.h"

#define LIMIT		/* limit residual velocities (only gradient method) */
#define EXTEND
#define TWOPI 6.28318530717958647692528
#define PI 3.14159265358979323846
#define HALF_WINDOW (2)
#define WINDOW (2*HALF_WINDOW+1)
#define NITERATIONS 3
#define MAX_CHANGE 1.0
#define HALF_WINDOW (2)
#define BOTH_GRADIENTS
#define SMALL 5

extern int Image_border;
extern FILTER rf,ef;

static int Qinit=0;
static float **Q;
static float **W;
static float *gama;
static float *lambda;
static float **phi;
static int CSinit=0;
static float *Sin,*Cos;




void
lsq_lsqgrad_fast_fwd(FIMAGE image1,FIMAGE image2,FIMAGE vx,FIMAGE vy,FIMAGE conf)
{
   FIMAGE Gx,Gy,Gt;
   float **dx=vx.ptr,**dy=vy.ptr,**c=conf.ptr;
   float *fxx[WINDOW],*fxy[WINDOW],*fyy[WINDOW],*fxt[WINDOW],*fyt[WINDOW];
   float *oldxx,*oldxy,*oldyy,*oldxt,*oldyt;
   float *newxx,*newxy,*newyy,*newxt,*newyt,*Fxx,*Fxy,*Fyy,*Fxt,*Fyt;
   float alpha_max,alpha_min;
   int row;
   int w=image1.w,h=image1.h,x,y,xx,yy,i,j,k,n,n1,nrot;
   char file[MAX_STR_LEN];

   /* Reflect images */
   reflect(image1,Image_border,3);
   reflect(image2,Image_border,3);
   /* Zero out border on vx and vy */
   fset(vx,0.0);
   fset(vy,0.0);
   fset(conf,0.0);

   /* Allocate needed vectors and matrices */
   gama = alloc_fvector(1,2);
   W = alloc_farray(1,2,1,2);
   phi = alloc_farray(1,2,1,2);
   lambda = alloc_fvector(1,2);
   for (j=0; j < WINDOW; j++) {
     fxx[j] = _alloc_fvector(w);
     fxy[j] = _alloc_fvector(w);
     fyy[j] = _alloc_fvector(w);
     fxt[j] = _alloc_fvector(w);
     fyt[j] = _alloc_fvector(w);
   }
   newxx = _alloc_fvector(w);   Fxx = _alloc_fvector(w);
   newxy = _alloc_fvector(w);   Fxy = _alloc_fvector(w);
   newyy = _alloc_fvector(w);   Fyy = _alloc_fvector(w);
   newxt = _alloc_fvector(w);   Fxt = _alloc_fvector(w);
   newyt = _alloc_fvector(w);   Fyt = _alloc_fvector(w);

   /* Gradient Estimation */
   Gx.w = Gy.w = Gt.w = w;  Gx.h = Gy.h = Gt.h = h;
   alloc_fimage(&Gx);  alloc_fimage(&Gy);  alloc_fimage(&Gt);
   gradient_estimation_fwd(image1,image2,Gx,Gy,Gt);				/* asdf */

   /* Start motion estimation */

   for (i=0; i < w; i++)
     Fxx[i] = Fxy[i] = Fyy[i] = Fxt[i] = Fyt[i] = 0.0;
   for (j=0; j < WINDOW-1; j++) {
     row_lsqgrad_sum_fwd(Gx,Gy,Gt,j,fxx[j],fxy[j],fyy[j],fxt[j],fyt[j]);	/* asdf */
     for (i=HALF_WINDOW; i < w-HALF_WINDOW; i++) {
       Fxx[i] += fxx[j][i];
       Fxy[i] += fxy[j][i];
       Fyy[i] += fyy[j][i];
       Fxt[i] += fxt[j][i];
       Fyt[i] += fyt[j][i];
     }
   }
   row = WINDOW - 1;
   for (i=0; i < w; i++)
     fxx[row][i] = fxy[row][i] = fyy[row][i] = fxt[row][i] = fyt[row][i] = 0.0;

   for (y=HALF_WINDOW; y < h-HALF_WINDOW; y++) {
     row_lsqgrad_sum_fwd(Gx,Gy,Gt,y+HALF_WINDOW,newxx,newxy,newyy,newxt,newyt);
     oldxx = fxx[row];  oldxy = fxy[row];  oldyy = fyy[row];
     oldxt = fxt[row];  oldyt = fyt[row];
     for (x=HALF_WINDOW; x < w-HALF_WINDOW; x++) {
#ifdef DEBUG 
       printf("\n**** Starting computation for Point: %d,%d ****\n",x,y);
#endif

	/* Compute W and gama */
       W[1][1] = Fxx[x] += (newxx[x] - oldxx[x]);
       W[2][1] = Fxy[x] += (newxy[x] - oldxy[x]);
       W[1][2] = W[2][1];
       W[2][2] = Fyy[x] += (newyy[x] - oldyy[x]);

       gama[1] = Fxt[x] += (newxt[x] - oldxt[x]);
       gama[2] = Fyt[x] += (newyt[x] - oldyt[x]);

       compute_velocities_fwd(&dx[y][x],&dy[y][x],&c[y][x],W,gama,lambda,phi);	/* asdf */

#ifdef LIMIT
       if (ABS(dx[y][x]) > HALF_WINDOW) {
         dx[y][x] = SIGN(HALF_WINDOW,dx[y][x]);
       }
       if (ABS(dy[y][x]) > HALF_WINDOW) {
         dy[y][x] = SIGN(HALF_WINDOW,dy[y][x]);
       }
#endif

     }
     fxx[row] = newxx; newxx = oldxx;
     fxy[row] = newxy; newxy = oldxy;
     fyy[row] = newyy; newyy = oldyy;
     fxt[row] = newxt; newxt = oldxt;
     fyt[row] = newyt; newyt = oldyt;
     row++;  row %= WINDOW;
   }

	/* Free up gradient images */
   free_fimage(Gt);  free_fimage(Gy);  free_fimage(Gx);

	/* Free vectors and matrices */
   _free_fvector(Fyt);   _free_fvector(newyt);
   _free_fvector(Fxt);   _free_fvector(newxt);
   _free_fvector(Fyy);   _free_fvector(newyy);
   _free_fvector(Fxy);   _free_fvector(newxy);
   _free_fvector(Fxx);   _free_fvector(newxx);
   for (j=0; j < WINDOW; j++) {
     _free_fvector(fyt[j]);
     _free_fvector(fxt[j]);
     _free_fvector(fyy[j]);
     _free_fvector(fxy[j]);
     _free_fvector(fxx[j]);
   }
   free_fvector(lambda,1,2);
   free_farray(phi,1,2,1,2);
   free_farray(W,1,2,1,2);
   free_fvector(gama,1,2);

#ifdef EXTEND
/* Extend velocity fields */
   extend(vx,HALF_WINDOW);
   extend(vy,HALF_WINDOW);
   extend(conf,HALF_WINDOW);
#endif
   fdivide(conf,(float)(WINDOW*WINDOW));
}


/*___________________END:lsq_lsqgrad_fast_fwd________________________________________*/


gradient_estimation_fwd(FIMAGE image1, FIMAGE image2, FIMAGE Gx, FIMAGE Gy, FIMAGE Gt)
{
   float **img1=image1.ptr,**img2=image2.ptr;
   float **gx=Gx.ptr, **gy=Gy.ptr, **gt=Gt.ptr;
   int w=image1.w, h=image2.h, x,y;
   FIMAGE Scr;
   float **scr;

   Scr.w = w;  Scr.h = h;  alloc_fimage(&Scr);  scr = Scr.ptr; 
/*********************************** (original) s+a
   fsubtract_img(Scr,image1,image2);
*****************************************/
   fsubtract_img(Scr,image1,image2);
   reflect(Scr,3,3);
   for (y=0; y < h; y++)
     for (x=0; x < w; x++) {
#ifdef BOTH_GRADIENTS
       gx[y][x] = (img2[y][x+1] - img2[y][x-1] + 
                   img1[y][x+1] - img1[y][x-1])/4.0;
       gy[y][x] = (img2[y+1][x] - img2[y-1][x] + 
                   img1[y+1][x] - img1[y-1][x])/4.0;
       gt[y][x] = (scr[y][x-1] + scr[y][x+1] + scr[y][x] +
                   scr[y-1][x] + scr[y+1][x])/5.0;
#endif
#ifdef FIVE_GRADIENT
       gx[y][x] = (img2[y][x-2] -8.0*img2[y][x-1] + 8.0*img2[y][x+1]
                   - img2[y][x+2])/12.0;
       gy[y][x] = (img2[y-2][x] -8.0*img2[y-1][x] + 8.0*img2[y+1][x]
                   - img2[y+2][x])/12.0;
       gt[y][x] = (scr[y][x-1] + scr[y][x+1] + scr[y][x] +
                   scr[y-1][x] + scr[y+1][x])/5.0;
#endif

#ifdef BOTH_FIVE_GRADIENT
       gx[y][x] = (img2[y][x-2] -8.0*img2[y][x-1] + 8.0*img2[y][x+1]
                   - img2[y][x+2] +
                   img1[y][x-2] -8.0*img1[y][x-1] + 8.0*img1[y][x+1]
                   - img1[y][x+2])/24.0;
       gy[y][x] = (img2[y-2][x] -8.0*img2[y-1][x] + 8.0*img2[y+1][x]
                   - img2[y+2][x] +
                   img1[y-2][x] -8.0*img1[y-1][x] + 8.0*img1[y+1][x]
                   - img1[y+2][x])/24.0;
       gt[y][x] = (scr[y][x-1] + scr[y][x+1] + scr[y][x] +
                   scr[y-1][x] + scr[y+1][x])/5.0;
#endif

#ifdef THREE_GRADIENT
       gx[y][x] = (img2[y][x+1] - img2[y][x-1])/2.0;
       gy[y][x] = (img2[y+1][x] - img2[y-1][x])/2.0;
       gt[y][x] = scr[y][x];
#endif

     }
   free_fimage(Scr);
return 00;
}
/*___________________END:gradient_estimation_fwd _________________________*/


row_lsqgrad_sum_fwd(FIMAGE Gx, FIMAGE Gy, FIMAGE Gt, int row, float *fxx, float *fxy, float *fyy, float *fxt, float *fyt)
{
   float **gx=Gx.ptr,**gy=Gy.ptr,**gt=Gt.ptr;
   int x,xx,col;
   int w=Gx.w,h=Gx.h;
   float gxx[WINDOW],gxy[WINDOW],gyy[WINDOW],gxt[WINDOW],gyt[WINDOW];
   float Fx,Fy,Ft;

   fxx += HALF_WINDOW;  fxy += HALF_WINDOW;  fyy += HALF_WINDOW;
   fxt += HALF_WINDOW;  fyt += HALF_WINDOW;
   *fxx = *fxy = *fyy = *fxt = *fyt = 0.0;
   for (x=0; x < WINDOW; x++) {
     Fx = gx[row][x];
     Fy = gy[row][x];
     Ft = gt[row][x];
     *fxx += gxx[x] = Fx*Fx;
     *fxy += gxy[x] = Fx*Fy;
     *fyy += gyy[x] = Fy*Fy;
     *fxt += gxt[x] = Fx*Ft;
     *fyt += gyt[x] = Fy*Ft;
   }
   col = 0;
   fxx++; fxy++; fyy++; fxt++; fyt++;
   for (x=HALF_WINDOW+1,xx=x+HALF_WINDOW; x < w-HALF_WINDOW; x++,xx++) {
     *fxx = *(fxx-1) - gxx[col]; 
     *fxy = *(fxy-1) - gxy[col]; 
     *fyy = *(fyy-1) - gyy[col]; 
     *fxt = *(fxt-1) - gxt[col]; 
     *fyt = *(fyt-1) - gyt[col]; 
     Fx = gx[row][xx];
     Fy = gy[row][xx];
     Ft = gt[row][xx];
     *fxx++ += gxx[col] = Fx*Fx; 
     *fxy++ += gxy[col] = Fx*Fy; 
     *fyy++ += gyy[col] = Fy*Fy; 
     *fxt++ += gxt[col] = Fx*Ft; 
     *fyt++ += gyt[col] = Fy*Ft;
     col++;  col %= WINDOW; 
   }
return 00;
}
/*______________END:row_lsqgrad_sum_fwd ___________________________________________*/

compute_velocities_fwd(float *vx, float *vy, float *conf, float **ww, float *gama, float *lambda, float **phi)
{
  float alpha_max,alpha_min;
  int nrot;

#ifdef DEBUG
  printwgama();
#endif

/* Get eigenvectors and eigenvalues of ww */
  eigens(ww,lambda,phi);
/*  jacobi(ww,2,lambda,phi,&nrot);*/
/*  eigsrt(lambda,phi,2);*/

#ifdef DEBUG
  printeigens();
#endif

/* Compute confidence */
  *conf = lambda[2];
/* Compute velocities */
  if (lambda[1] > 100*lambda[2] && lambda[1] > SMALL) {/* Singular point */
    alpha_max = (phi[1][1]*gama[1] + phi[2][1]*gama[2])/lambda[1];
    *vx = alpha_max*phi[1][1];
    *vy = alpha_max*phi[2][1];
#ifdef DEBUG
    printf("Singular point: alpha_max=%f\n",alpha_max);
    printf("Velocity vector: %8.4f  %8.4f\n",*vx,*vy);
#endif
  } else if (lambda[1] > SMALL) {	/* Non-singular point */
    alpha_max = (phi[1][1]*gama[1] + phi[2][1]*gama[2])/lambda[1];
    alpha_min = (phi[1][2]*gama[1] + phi[2][2]*gama[2])/lambda[2];
    /************************************************* was: (a+s)
    *vx = alpha_max*phi[1][1] + alpha_min*phi[1][2];
    *vy = alpha_max*phi[2][1] + alpha_min*phi[2][2];
    **************************************************/
    *vx = alpha_max*phi[1][1] + alpha_min*phi[1][2];
    *vy = alpha_max*phi[2][1] + alpha_min*phi[2][2];
#ifdef DEBUG
    printf("Non-singular point: alpha_max=%f alpha_min=%f\n",
                   alpha_max,alpha_min);
    printf("Velocity vector: %8.4f  %8.4f\n",*vx,*vy);
#endif
  } else {
    *vx = 0.0;
    *vy = 0.0;
#ifdef DEBUG
    printf("Singular point: ww=0\n");
#endif
  }
return 00;
}
/*______________END:compute_velocities_fwd _________________________________*/





#if 0	/*0000000000000000000000000000000000000000000000000000*/

void
lsq_grad_fwd(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy)
{
   float **img1=image1.ptr,**img2=image2.ptr,**dx=vx.ptr,**dy=vy.ptr;
   float Fx,Fy,Ft,Fxl,Fyl,Ftl,Fxr,Fyr,Ftr,Fxx,Fxy,Fyy,Fxt,Fyt;
   float alpha_max,alpha_min;
   float **alloc_farray(),*alloc_fvector();
   int left,right;
   int w=image1.w,h=image1.h,x,y,xx,yy,i,j,k,n,n1,nrot;
   char file[MAX_STR_LEN];

/* Reflect images */
   reflect(image1,Image_border,3);
   reflect(image2,Image_border,3);
/* Zero out border on vx and vy */
   fset(vx,0.0);
   fset(vy,0.0);

/* Allocate needed vectors and matrices */
   gama = alloc_fvector(1,2);
   W = alloc_farray(1,2,1,2);
   phi = alloc_farray(1,2,1,2);
   lambda = alloc_fvector(1,2);

/* Start motion estimation */

   for (y=HALF_WINDOW; y < h-HALF_WINDOW; y++) {
     for (x=HALF_WINDOW; x < w-HALF_WINDOW; x++) {
#ifdef DEBUG 
       printf("\n**** Starting computation for Point: %d,%d ****\n",x,y);
#endif

/* Get sum over window of derivatives */
       if (x==HALF_WINDOW) { 
         Fxx = Fxy = Fyy = Fxt = Fyt = 0.0;
         for (j = -HALF_WINDOW,yy=y+j; j <= HALF_WINDOW; j++,yy++)
           for (i = -HALF_WINDOW,xx=x+i; i <= HALF_WINDOW; i++,xx++) {
             Fx = (img2[yy][xx+1] - img2[yy][xx-1])/2.0;
             Fy = (img2[yy+1][xx] - img2[yy-1][xx])/2.0;
             Ft = img1[yy][xx] - img2[yy][xx];
             Fxx += Fx*Fx;
             Fxy += Fx*Fy;
             Fyy += Fy*Fy;
             Fxt += Fx*Ft;
             Fyt += Fy*Ft; 
           }
         left=0; right=left+WINDOW;
       } else {
         for (j= -HALF_WINDOW,yy=y+j; j <= HALF_WINDOW; j++,yy++) {
           Fxl = (img2[yy][left+1] - img2[yy][left-1])/2.0;
           Fyl = (img2[yy+1][left] - img2[yy-1][left])/2.0;
           Ftl = img1[yy][left] - img2[yy][left];
           Fxr = (img2[yy][right+1] - img2[yy][right-1])/2.0;
           Fyr = (img2[yy+1][right] - img2[yy-1][right])/2.0;
           Ftr = img1[yy][right] - img2[yy][right];
           Fxx += (Fxr*Fxr - Fxl*Fxl);
           Fxy += (Fxr*Fyr - Fxl*Fyl);
           Fyy += (Fyr*Fyr - Fyl*Fyl);
           Fxt += (Fxr*Ftr - Fxl*Ftl);
           Fyt += (Fyr*Ftr - Fyl*Ftl);
         }
         left++; right++;
       }

/* Compute W and gama */
       W[1][1] = Fxx;
       W[2][1] = Fxy;
       W[1][2] = W[2][1];
       W[2][2] = Fyy;

       gama[1] = Fxt;
       gama[2] = Fyt;

       compute_velocities_fwd(&dx[y][x],&dy[y][x],W,gama,lambda,phi);

#ifdef LIMIT
       if (ABS(dx[y][x]) > HALF_WINDOW) {
         dx[y][x] = SIGN(HALF_WINDOW,dx[y][x]);
       }
       if (ABS(dy[y][x]) > HALF_WINDOW) {
         dy[y][x] = SIGN(HALF_WINDOW,dy[y][x]);
       }
#endif

     }
   }

/* Free vectors and matrices */
   free_fvector(lambda,1,2);
   free_farray(phi,1,2,1,2);
   free_farray(W,1,2,1,2);
   free_fvector(gama,1,2);

#ifdef EXTEND
/* Extend velocity fields */
   extend(vx,HALF_WINDOW);
   extend(vy,HALF_WINDOW);
#endif
}
       

void
lsq_grad_fast_fwd(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy)
{
   float **dx=vx.ptr,**dy=vy.ptr;
   float *fxx[WINDOW],*fxy[WINDOW],*fyy[WINDOW],*fxt[WINDOW],*fyt[WINDOW];
   float *oldxx,*oldxy,*oldyy,*oldxt,*oldyt;
   float *newxx,*newxy,*newyy,*newxt,*newyt,*Fxx,*Fxy,*Fyy,*Fxt,*Fyt;
   float alpha_max,alpha_min;
   float **alloc_farray(),*alloc_fvector(),*_alloc_fvector();
   int row;
   int w=image1.w,h=image1.h,x,y,xx,yy,i,j,k,n,n1,nrot;
   char file[MAX_STR_LEN];

/* Reflect images */
   reflect(image1,Image_border,3);
   reflect(image2,Image_border,3);
/* Zero out border on vx and vy */
   fset(vx,0.0);
   fset(vy,0.0);

/* Allocate needed vectors and matrices */
   gama = alloc_fvector(1,2);
   W = alloc_farray(1,2,1,2);
   phi = alloc_farray(1,2,1,2);
   lambda = alloc_fvector(1,2);
   for (j=0; j < WINDOW; j++) {
     fxx[j] = _alloc_fvector(w);
     fxy[j] = _alloc_fvector(w);
     fyy[j] = _alloc_fvector(w);
     fxt[j] = _alloc_fvector(w);
     fyt[j] = _alloc_fvector(w);
   }
   newxx = _alloc_fvector(w);   Fxx = _alloc_fvector(w);
   newxy = _alloc_fvector(w);   Fxy = _alloc_fvector(w);
   newyy = _alloc_fvector(w);   Fyy = _alloc_fvector(w);
   newxt = _alloc_fvector(w);   Fxt = _alloc_fvector(w);
   newyt = _alloc_fvector(w);   Fyt = _alloc_fvector(w);

/* Start motion estimation */

   for (i=0; i < w; i++)
     Fxx[i] = Fxy[i] = Fyy[i] = Fxt[i] = Fyt[i] = 0.0;
   for (j=0; j < WINDOW-1; j++) {
     row_grad_sum(image1,image2,j,fxx[j],fxy[j],fyy[j],fxt[j],fyt[j]);
     for (i=HALF_WINDOW; i < w-HALF_WINDOW; i++) {
       Fxx[i] += fxx[j][i];
       Fxy[i] += fxy[j][i];
       Fyy[i] += fyy[j][i];
       Fxt[i] += fxt[j][i];
       Fyt[i] += fyt[j][i];
     }
   }
   row = WINDOW - 1;
   for (i=0; i < w; i++)
     fxx[row][i] = fxy[row][i] = fyy[row][i] = fxt[row][i] = fyt[row][i] = 0.0;

   for (y=HALF_WINDOW; y < h-HALF_WINDOW; y++) {
     row_grad_sum(image1,image2,y+HALF_WINDOW,newxx,newxy,newyy,newxt,newyt);
     oldxx = fxx[row];  oldxy = fxy[row];  oldyy = fyy[row];
     oldxt = fxt[row];  oldyt = fyt[row];
     for (x=HALF_WINDOW; x < w-HALF_WINDOW; x++) {
#ifdef DEBUG 
       printf("\n**** Starting computation for Point: %d,%d ****\n",x,y);
#endif

/* Compute W and gama */
       W[1][1] = Fxx[x] += (newxx[x] - oldxx[x]);
       W[2][1] = Fxy[x] += (newxy[x] - oldxy[x]);
       W[1][2] = W[2][1];
       W[2][2] = Fyy[x] += (newyy[x] - oldyy[x]);

       gama[1] = Fxt[x] += (newxt[x] - oldxt[x]);
       gama[2] = Fyt[x] += (newyt[x] - oldyt[x]);

       compute_velocities_fwd(&dx[y][x],&dy[y][x],W,gama,lambda,phi);

#ifdef LIMIT
       if (ABS(dx[y][x]) > HALF_WINDOW) {
         dx[y][x] = SIGN(HALF_WINDOW,dx[y][x]);
       }
       if (ABS(dy[y][x]) > HALF_WINDOW) {
         dy[y][x] = SIGN(HALF_WINDOW,dy[y][x]);
       }
#endif

     }
     fxx[row] = newxx; newxx = oldxx;
     fxy[row] = newxy; newxy = oldxy;
     fyy[row] = newyy; newyy = oldyy;
     fxt[row] = newxt; newxt = oldxt;
     fyt[row] = newyt; newyt = oldyt;
     row++;  row %= WINDOW;
   }

/* Free vectors and matrices */
   _free_fvector(Fyt);   _free_fvector(newyt);
   _free_fvector(Fxt);   _free_fvector(newxt);
   _free_fvector(Fyy);   _free_fvector(newyy);
   _free_fvector(Fxy);   _free_fvector(newxy);
   _free_fvector(Fxx);   _free_fvector(newxx);
   for (j=0; j < WINDOW; j++) {
     _free_fvector(fyt[j]);
     _free_fvector(fxt[j]);
     _free_fvector(fyy[j]);
     _free_fvector(fxy[j]);
     _free_fvector(fxx[j]);
   }
   free_fvector(lambda,1,2);
   free_farray(phi,1,2,1,2);
   free_farray(W,1,2,1,2);
   free_fvector(gama,1,2);

#ifdef EXTEND
/* Extend velocity fields */
   extend(vx,HALF_WINDOW);
   extend(vy,HALF_WINDOW);
#endif
}



void
row_grad_sum_fwd(FIMAGE image1, FIMAGE image2, int row, float *fxx, float *fxy, float *fyy, float *fxt, float *fyt)
{
   float **img1=image1.ptr,**img2=image2.ptr;
   int x,xx,col;
   int w=image2.w,h=image2.h;
   float gxx[WINDOW],gxy[WINDOW],gyy[WINDOW],gxt[WINDOW],gyt[WINDOW];
   float Fx,Fy,Ft;

   fxx += HALF_WINDOW;  fxy += HALF_WINDOW;  fyy += HALF_WINDOW;
   fxt += HALF_WINDOW;  fyt += HALF_WINDOW;
   *fxx = *fxy = *fyy = *fxt = *fyt = 0.0;
   for (x=0; x < WINDOW; x++) {
     Fx = (img2[row][x+1] - img2[row][x-1])/2.0;
     Fy = (img2[row+1][x] - img2[row-1][x])/2.0;
     Ft = img1[row][x] - img2[row][x];
     *fxx += gxx[x] = Fx*Fx;
     *fxy += gxy[x] = Fx*Fy;
     *fyy += gyy[x] = Fy*Fy;
     *fxt += gxt[x] = Fx*Ft;
     *fyt += gyt[x] = Fy*Ft;
   }
   col = 0;
   fxx++; fxy++; fyy++; fxt++; fyt++;
   for (x=HALF_WINDOW+1,xx=x+HALF_WINDOW; x < w-HALF_WINDOW; x++,xx++) {
     *fxx = *(fxx-1) - gxx[col]; 
     *fxy = *(fxy-1) - gxy[col]; 
     *fyy = *(fyy-1) - gyy[col]; 
     *fxt = *(fxt-1) - gxt[col]; 
     *fyt = *(fyt-1) - gyt[col]; 
     Fx = (img2[row][xx+1] - img2[row][xx-1])/2.0;
     Fy = (img2[row+1][xx] - img2[row-1][xx])/2.0;
     Ft = img1[row][xx] - img2[row][xx];
     *fxx++ += gxx[col] = Fx*Fx; 
     *fxy++ += gxy[col] = Fx*Fy; 
     *fyy++ += gyy[col] = Fy*Fy; 
     *fxt++ += gxt[col] = Fx*Ft; 
     *fyt++ += gyt[col] = Fy*Ft;
     col++;  col %= WINDOW; 
   }
}



void
lsq_lsqgrad_fwd(FIMAGE image1, FIMAGE image2, FIMAGE vx, FIMAGE vy)
{
   FIMAGE Gx,Gy,Gt;
   float **gx,**gy,**gt;
   float **img1=image1.ptr,**img2=image2.ptr,**dx=vx.ptr,**dy=vy.ptr;
   float Fx,Fy,Ft,Fxl,Fyl,Ftl,Fxr,Fyr,Ftr,Fxx,Fxy,Fyy,Fxt,Fyt;
   float alpha_max,alpha_min;
   float **alloc_farray(),*alloc_fvector();
   int left,right;
   int w=image1.w,h=image1.h,x,y,xx,yy,i,j,k,n,n1,nrot;
   char file[MAX_STR_LEN];

/* Reflect images */
   reflect(image1,Image_border,3);
   reflect(image2,Image_border,3);
/* Zero out border on vx and vy */
   fset(vx,0.0);
   fset(vy,0.0);

/* Allocate needed vectors and matrices */
   gama = alloc_fvector(1,2);
   W = alloc_farray(1,2,1,2);
   phi = alloc_farray(1,2,1,2);
   lambda = alloc_fvector(1,2);

/* Gradient Estimation */
   Gx.w = Gy.w = Gt.w = w;  Gx.h = Gy.h = Gt.h = h;
   alloc_fimage(&Gx);  alloc_fimage(&Gy);  alloc_fimage(&Gt);
   gradient_estimation_fwd(image1,image2,Gx,Gy,Gt);
   gx = Gx.ptr;  gy = Gy.ptr;  gt = Gt.ptr;

/* Start motion estimation */

   for (y=HALF_WINDOW; y < h-HALF_WINDOW; y++) {
     for (x=HALF_WINDOW; x < w-HALF_WINDOW; x++) {
#ifdef DEBUG 
       printf("\n**** Starting computation for Point: %d,%d ****\n",x,y);
#endif

/* Get sum over window of derivatives */
       if (x==HALF_WINDOW) { 
         Fxx = Fxy = Fyy = Fxt = Fyt = 0.0;
         for (j = -HALF_WINDOW,yy=y+j; j <= HALF_WINDOW; j++,yy++)
           for (i = -HALF_WINDOW,xx=x+i; i <= HALF_WINDOW; i++,xx++) {
             Fx = gx[yy][xx];
             Fy = gy[yy][xx];
             Ft = gt[yy][xx];
             Fxx += Fx*Fx;
             Fxy += Fx*Fy;
             Fyy += Fy*Fy;
             Fxt += Fx*Ft;
             Fyt += Fy*Ft; 
           }
         left=0; right=left+WINDOW;
       } else {
         for (j= -HALF_WINDOW,yy=y+j; j <= HALF_WINDOW; j++,yy++) {
           Fxl = gx[yy][left];
           Fyl = gy[yy][left];
           Ftl = gt[yy][left];
           Fxr = gx[yy][right];
           Fyr = gy[yy][right];
           Ftr = gt[yy][right];
           Fxx += (Fxr*Fxr - Fxl*Fxl);
           Fxy += (Fxr*Fyr - Fxl*Fyl);
           Fyy += (Fyr*Fyr - Fyl*Fyl);
           Fxt += (Fxr*Ftr - Fxl*Ftl);
           Fyt += (Fyr*Ftr - Fyl*Ftl);
         }
         left++; right++;
       }

/* Compute W and gama */
       W[1][1] = Fxx;
       W[2][1] = Fxy;
       W[1][2] = W[2][1];
       W[2][2] = Fyy;

       gama[1] = Fxt;
       gama[2] = Fyt;

       compute_velocities_fwd(&dx[y][x],&dy[y][x],W,gama,lambda,phi);

#ifdef LIMIT
       if (ABS(dx[y][x]) > HALF_WINDOW) {
         dx[y][x] = SIGN(HALF_WINDOW,dx[y][x]);
       }
       if (ABS(dy[y][x]) > HALF_WINDOW) {
         dy[y][x] = SIGN(HALF_WINDOW,dy[y][x]);
       }
#endif

     }
   }

/* Free up gradient images */
   free_fimage(Gt);  free_fimage(Gy);  free_fimage(Gx);

/* Free vectors and matrices */
   free_fvector(lambda,1,2);
   free_farray(phi,1,2,1,2);
   free_farray(W,1,2,1,2);
   free_fvector(gama,1,2);

#ifdef EXTEND
/* Extend velocity fields */
   extend(vx,HALF_WINDOW);
   extend(vy,HALF_WINDOW);
#endif
}

void
lsq_lsqgrad_fast_mask_fwd(FIMAGE image1, FIMAGE image2, FIMAGE mask, FIMAGE vx, FIMAGE vy, FIMAGE conf)
{
   FIMAGE Gx,Gy,Gt;
   float **msk=mask.ptr,**dx=vx.ptr,**dy=vy.ptr,**c=conf.ptr;
   float *fxx[WINDOW],*fxy[WINDOW],*fyy[WINDOW],*fxt[WINDOW],*fyt[WINDOW];
   float *oldxx,*oldxy,*oldyy,*oldxt,*oldyt;
   float *newxx,*newxy,*newyy,*newxt,*newyt,*Fxx,*Fxy,*Fyy,*Fxt,*Fyt;
   float alpha_max,alpha_min;
   float **alloc_farray(),*alloc_fvector(),*_alloc_fvector();
   int row;
   int w=image1.w,h=image1.h,x,y,xx,yy,i,j,k,n,n1,nrot;
   char file[MAX_STR_LEN];

/* Reflect images */
   reflect(image1,Image_border,3);
   reflect(image2,Image_border,3);
/* Zero out border on vx and vy */
   fset(vx,0.0);
   fset(vy,0.0);
   fset(conf,0.0);

/* Allocate needed vectors and matrices */
   gama = alloc_fvector(1,2);
   W = alloc_farray(1,2,1,2);
   phi = alloc_farray(1,2,1,2);
   lambda = alloc_fvector(1,2);
   for (j=0; j < WINDOW; j++) {
     fxx[j] = _alloc_fvector(w);
     fxy[j] = _alloc_fvector(w);
     fyy[j] = _alloc_fvector(w);
     fxt[j] = _alloc_fvector(w);
     fyt[j] = _alloc_fvector(w);
   }
   newxx = _alloc_fvector(w);   Fxx = _alloc_fvector(w);
   newxy = _alloc_fvector(w);   Fxy = _alloc_fvector(w);
   newyy = _alloc_fvector(w);   Fyy = _alloc_fvector(w);
   newxt = _alloc_fvector(w);   Fxt = _alloc_fvector(w);
   newyt = _alloc_fvector(w);   Fyt = _alloc_fvector(w);

/* Gradient Estimation */
   Gx.w = Gy.w = Gt.w = w;  Gx.h = Gy.h = Gt.h = h;
   alloc_fimage(&Gx);  alloc_fimage(&Gy);  alloc_fimage(&Gt);
   gradient_estimation_fwd(image1,image2,Gx,Gy,Gt);

/* Start motion estimation */

   for (i=0; i < w; i++)
     Fxx[i] = Fxy[i] = Fyy[i] = Fxt[i] = Fyt[i] = 0.0;
   for (j=0; j < WINDOW-1; j++) {
     row_lsqgrad_sum_fwd(Gx,Gy,Gt,j,fxx[j],fxy[j],fyy[j],fxt[j],fyt[j]);
     for (i=HALF_WINDOW; i < w-HALF_WINDOW; i++) {
       Fxx[i] += fxx[j][i];
       Fxy[i] += fxy[j][i];
       Fyy[i] += fyy[j][i];
       Fxt[i] += fxt[j][i];
       Fyt[i] += fyt[j][i];
     }
   }
   row = WINDOW - 1;
   for (i=0; i < w; i++)
     fxx[row][i] = fxy[row][i] = fyy[row][i] = fxt[row][i] = fyt[row][i] = 0.0;

   for (y=HALF_WINDOW; y < h-HALF_WINDOW; y++) {
     row_lsqgrad_sum_fwd(Gx,Gy,Gt,y+HALF_WINDOW,newxx,newxy,newyy,newxt,newyt);
     oldxx = fxx[row];  oldxy = fxy[row];  oldyy = fyy[row];
     oldxt = fxt[row];  oldyt = fyt[row];
     for (x=HALF_WINDOW; x < w-HALF_WINDOW; x++) {
#ifdef DEBUG 
       printf("\n**** Starting computation for Point: %d,%d ****\n",x,y);
#endif

/* Compute W and gama */
       W[1][1] = Fxx[x] += (newxx[x] - oldxx[x]);
       W[2][1] = Fxy[x] += (newxy[x] - oldxy[x]);
       W[1][2] = W[2][1];
       W[2][2] = Fyy[x] += (newyy[x] - oldyy[x]);

       gama[1] = Fxt[x] += (newxt[x] - oldxt[x]);
       gama[2] = Fyt[x] += (newyt[x] - oldyt[x]);

       if (msk[y][x] > 0.5) {
	 compute_velocities_fwd(&dx[y][x],&dy[y][x],&c[y][x],W,gama,lambda,phi);
       } else {
	 dx[y][x] = 0.0;
	 dy[y][x] = 0.0;
	 c[y][x] = 0.0;
       }

#ifdef LIMIT
       if (ABS(dx[y][x]) > HALF_WINDOW) {
         dx[y][x] = SIGN(HALF_WINDOW,dx[y][x]);
       }
       if (ABS(dy[y][x]) > HALF_WINDOW) {
         dy[y][x] = SIGN(HALF_WINDOW,dy[y][x]);
       }
#endif

     }
     fxx[row] = newxx; newxx = oldxx;
     fxy[row] = newxy; newxy = oldxy;
     fyy[row] = newyy; newyy = oldyy;
     fxt[row] = newxt; newxt = oldxt;
     fyt[row] = newyt; newyt = oldyt;
     row++;  row %= WINDOW;
   }

/* Free up gradient images */
   free_fimage(Gt);  free_fimage(Gy);  free_fimage(Gx);

/* Free vectors and matrices */
   _free_fvector(Fyt);   _free_fvector(newyt);
   _free_fvector(Fxt);   _free_fvector(newxt);
   _free_fvector(Fyy);   _free_fvector(newyy);
   _free_fvector(Fxy);   _free_fvector(newxy);
   _free_fvector(Fxx);   _free_fvector(newxx);
   for (j=0; j < WINDOW; j++) {
     _free_fvector(fyt[j]);
     _free_fvector(fxt[j]);
     _free_fvector(fyy[j]);
     _free_fvector(fxy[j]);
     _free_fvector(fxx[j]);
   }
   free_fvector(lambda,1,2);
   free_farray(phi,1,2,1,2);
   free_farray(W,1,2,1,2);
   free_fvector(gama,1,2);

#ifdef EXTEND
/* Extend velocity fields */
   extend(vx,HALF_WINDOW);
   extend(vy,HALF_WINDOW);
   extend(conf,HALF_WINDOW);
#endif
   fdivide(conf,(float) (WINDOW*WINDOW));
}



#endif	/*00000000000000000000000000000000000000000000000000000*/





/**************************************************************
 **************************************************************
 **************************************************************/

#if 0

static int
matherr(struct exception *exc)
{
   fprintf(stderr,"%s: ",exc->name);
   switch (exc->type) {
     case DOMAIN:
       fprintf(stderr,"argument domain exception\n");
       break;
     case SING:
       fprintf(stderr,"argument singularity\n");
       break;
     case OVERFLOW:
       fprintf(stderr,"Overflow range exception\n");
       break;
     case UNDERFLOW:
       fprintf(stderr,"Underflow range exception\n");
       break;
     default:
       fprintf(stderr,"Unknown Exception\n");
   }
   fprintf(stderr,"\tinput arguments: %f\t%f\n",exc->arg1,exc->arg2);
   fprintf(stderr,"\treturn Value: %f\n",exc->retval);
   return(0);
}
#endif



