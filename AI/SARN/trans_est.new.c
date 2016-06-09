/* Translation parameter estimator */

#include "imgdec.h"
#include "imgmacro.h"

#define BORDER
#define BORDER_WIDTH 4

extern int Image_border;

typedef struct {
  int x,y;		/* xoffset and yoffset		*/
  int w,h;		/* width and height of window	*/
} WINDOW;

static float **W;
static float *gama;

translation_estimate(image1,image2,vx,vy,window)
FIMAGE image1,image2;
float *vx,*vy;
WINDOW *window;
{
   float **img1=image1.ptr,**img2=image2.ptr;
   float Fx,Fy,Ft,Fxx=0.0,Fxy=0.0,Fyy=0.0,Fxt=0.0,Fyt=0.0;
   float **alloc_farray(),*alloc_fvector();
   int l,r,b,t,w=image1.w,h=image1.h,x,y;

   if (window != NULL) {
     l=window->x; r=l+window->w; b=window->y; t=b+window->h;
   } else {
     l=0; r=w; b=0; t=h;
   }
#ifdef BORDER
   l = MAX(l,BORDER_WIDTH);
   r = MIN(r,image1.w-BORDER_WIDTH);
   b = MAX(b,BORDER_WIDTH);
   t = MIN(t,image1.h-BORDER_WIDTH);
#endif

/* Reflect images */
   reflect(image1,Image_border,3);
   reflect(image2,Image_border,3);

/* Allocate needed vectors and matrices */
   gama = alloc_fvector(1,2);
   W = alloc_farray(1,2,1,2);

/* Start motion estimation */

   for (y=b; y < t; y++) {
     for (x=l; x < r; x++) {
       Fx = (img2[y][x+1] - img2[y][x-1])/2.0;
       Fy = (img2[y+1][x] - img2[y-1][x])/2.0;
       Ft = img1[y][x] - img2[y][x];
       Fxx += Fx*Fx;
       Fxy += Fx*Fy;
       Fyy += Fy*Fy;
       Fxt += Fx*Ft;
       Fyt += Fy*Ft;
     }
   }

/* Compute W and gama */
   W[1][1] = Fxx;
   W[2][1] = Fxy;
   W[1][2] = W[2][1];
   W[2][2] = Fyy;

   gama[1] = Fxt;
   gama[2] = Fyt;

   compute_translation(vx,vy,W,gama);

   free_farray(W,1,2,1,2);
   free_fvector(gama,1,2);
}
       
compute_translation(vx,vy,W,gama)
float *vx,*vy;
float **W,*gama;
{
  int index[3];
  float det;

#ifdef DEBUG
  printwgama();
#endif

  ludcmp(W,2,index,&det);
  lubksb(W,2,index,gama);

  *vx = gama[1];  *vy = gama[2];
#ifdef DEBUG
  printf("Velocity vector: %8.4f  %8.4f\n",*vx,*vy);
#endif
}


static printwgama()	/* Write W matrix and gama to stdout */
{
   printf("W matrix: %10.4f   %10.4f\n          %10.4f   %10.4f\n",
            W[1][1],W[1][2],W[2][1],W[2][2]);
   printf("Gamma:    %10.4f   %10.4f\n",gama[1],gama[2]);
}


