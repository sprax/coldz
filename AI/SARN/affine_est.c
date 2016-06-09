/* Routines for affine parameter estimation */

#include "imgdec.h"
#include "imgmacro.h"

#define BORDER
#define BORDER_WIDTH 4
#define NOT_ABOUT_CENTER

extern int Image_border;


static float **W;
static float *s;

      
compute_affine_parameters(vx,vy,W,s)
PARTYPE *vx,*vy;
float **W,*s;
{
  int index[7];
  float det;

#ifdef DEBUG
  printws();
#endif

  ludcmp(W,6,index,&det);
  lubksb(W,6,index,s);
  
  vx->a = s[1];  vx->b = s[2];  vx->c = s[3];
  vy->a = s[4];  vy->b = s[5];  vy->c = s[6];

#ifdef DEBUG
  print_affine_parameters(vx,vy);
#endif
}


affine_parameter_estimate(image1,image2,vx,vy,roi)
FIMAGE image1,image2;
PARTYPE *vx,*vy;
BIMAGE roi;
{
   float **img1=image1.ptr,**img2=image2.ptr;
   float fx,fy,ft,f,xf,yf;
   float Fxx=0.0,xFxx=0.0,yFxx=0.0,xyFxx=0.0,xxFxx=0.0,yyFxx=0.0;
   float Fxy=0.0,xFxy=0.0,yFxy=0.0,xyFxy=0.0,xxFxy=0.0,yyFxy=0.0;
   float Fyy=0.0,xFyy=0.0,yFyy=0.0,xyFyy=0.0,xxFyy=0.0,yyFyy=0.0;
   float Fxt=0.0,xFxt=0.0,yFxt=0.0,Fyt=0.0,xFyt=0.0,yFyt=0.0;
   float **alloc_farray(),*alloc_fvector();
   byte **reg=roi.ptr;
   int w=image1.w,h=image1.h,i,j,l=0,r=w,b=0,t=h;
   float xc,yc,x,y;

#ifdef BORDER
   l = MAX(l,BORDER_WIDTH);
   r = MIN(r,image1.w-BORDER_WIDTH);
   b = MAX(b,BORDER_WIDTH);
   t = MIN(t,image1.h-BORDER_WIDTH);
#endif
#ifdef ABOUT_CENTER
   xc=(l+r-1)/2.0; yc=(b+t-1)/2.0;
#else
   xc=0.0; yc=0.0;
#endif

/* Reflect images */
   reflect(image1,Image_border,3);
   reflect(image2,Image_border,3);

/* Allocate needed vectors and matrices */
   s = alloc_fvector(1,6);
   W = alloc_farray(1,6,1,6);

/* Start motion estimation */

   for (j=b; j < t; j++) {
     y = j - yc;
     for (i=l; i < r; i++) {
       if (!reg[j][i])
         continue;
       x = i - xc;
       fx = (img2[j][i+1] - img2[j][i-1])/2.0;
       fy = (img2[j+1][i] - img2[j-1][i])/2.0;
       ft = img1[j][i] - img2[j][i];
       Fxx += f = fx*fx;  xFxx += xf = x*f;  yFxx += yf = y*f;
       xxFxx += x*xf;     xyFxx += y*xf;     yyFxx += y*yf;
       Fxy += f = fx*fy;  xFxy += xf = x*f;  yFxy += yf = y*f;
       xxFxy += x*xf;     xyFxy += y*xf;     yyFxy += y*yf;
       Fyy += f = fy*fy;  xFyy += xf = x*f;  yFyy += yf = y*f;
       xxFyy += x*xf;     xyFyy += y*xf;     yyFyy += y*yf;
       Fxt += f = fx*ft;  xFxt += x*f;       yFxt += y*f;
       Fyt += f = fy*ft;  xFyt += x*f;       yFyt += y*f;
     }
   }

/* Compute W and s */
   W[1][1] = Fxx;
   W[1][2] = W[2][1] = xFxx;
   W[1][3] = W[3][1] = yFxx;
   W[1][4] = W[4][1] = Fxy;
   W[1][5] = W[5][1] = xFxy;
   W[1][6] = W[6][1] = yFxy;
   W[2][2] = xxFxx;
   W[2][3] = W[3][2] = xyFxx;
   W[2][4] = W[4][2] = xFxy;
   W[2][5] = W[5][2] = xxFxy;
   W[2][6] = W[6][2] = xyFxy;
   W[3][3] = yyFxx;
   W[3][4] = W[4][3] = yFxy;
   W[3][5] = W[5][3] = xyFxy;
   W[3][6] = W[6][3] = yyFxy;
   W[4][4] = Fyy;
   W[4][5] = W[5][4] = xFyy;
   W[4][6] = W[6][4] = yFyy;
   W[5][5] = xxFyy;
   W[5][6] = W[6][5] = xyFyy;
   W[6][6] = yyFyy;

   s[1] = Fxt;  s[2] = xFxt;  s[3] = yFxt;
   s[4] = Fyt;  s[5] = xFyt;  s[6] = yFyt;

   compute_affine_parameters(vx,vy,W,s);
#ifdef ABOUT_CENTER
/* vx,vy are defined about center of the image; convert them to origin */
   vx->a = vx->a - vx->b*xc - vx->c*yc;
   vy->a = vy->a - vy->b*xc - vy->c*yc;
#endif

   free_farray(W,1,6,1,6);
   free_fvector(s,1,6);
}
 


static printws()	/* Write W matrix and s to stdout */
{
   int i,j;

   for (j=1; j <= 6; j++) {
     printf("<");
     for (i=1; i <= 6; i++)
       printf(" %10.4f",W[j][i]);
     printf("> v[%d] = %10.4f\n",j,s[j]);
   }
}


static print_affine_parameters(vx,vy)	/* Write vx and vy plane parameters to stdout */
PARTYPE *vx,*vy;
{
   printf("vx.a=%10.4f   vx.b=%10.4f   vx.c=%10.4f\n",vx->a,vx->b,vx->c);
   printf("vy.a=%10.4f   vy.b=%10.4f   vy.c=%10.4f\n",vy->a,vy->b,vy->c);
}

