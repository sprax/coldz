/***************************************************************************
 *
 * library interface linking Sarnoff code with my vision library
 *
 * David Beymer
 * February, 1993
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "image.h"		/* my image definition */
#include "imgdec.h"		/* their image definition */
#include "imgmacro.h"
#include "interface.h"
#include "beymer/image.h"
#include "beymer/interface.h"
#include "beymer/alloc.h"
#include "beymer/gradient.h"
#include "beymer/strel.h"
#include "beymer/macros.h"
#include "beymer/geo.h"
#include "beymer/image_binary.h"
#include "beymer/image_grey.h"

#ifndef SGN     /* which one? */
#define SGN(A)  ((A) > 0 ? 1 : (A) == 0 ? 0 : (-1))
#define SGN(A)  (((A) > 0) ? (1) : (-(1)))
#endif


#if 1
	int Image_border = 10;
	FILTER rf,ef;
#else
	extern	int Image_border;
	extern	FILTER rf,ef;
#endif

/* good defaults: toplev=4, botlev=0
 */

void
mot_lsqconf ( image_t *dx_arg, image_t *dy_arg, image_t *conf_arg
            , image_t *img1_arg, image_t *img2_arg, int toplev, int botlev)
{
   char filtfile[MAX_STR_LEN];
   FPYR pyr1,pyr2,dx,dy,conf;
   FIMAGE img1,img2;
   int finlev=0;

   strcpy_s(filtfile,MAX_STR_LEN,DEFAULT_PYR_FILTER);	/* default filter */
   getpyrfilters(filtfile,&rf,&ef);
   img1.w = img1_arg->width;
   img1.h = img1_arg->height;
   alloc_fimage (&img1);
   image2sarnoff_f (img1, img1_arg);
   img2.w = img2_arg->width;
   img2.h = img2_arg->height;
   alloc_fimage (&img2);
   image2sarnoff_f (img2, img2_arg);
   if (img1.w != img2.w || img1.h != img2.h) {
     fprintf(stderr,"mot_lsqconf: image size mismatch\n");
     exit(-1);
   }
/* Set up pyramids for coarse to fine estimation */
   def_fpyr(pyr1,botlev,img1.w,img1.h);
   pyr1[botlev].ptr = img1.ptr;
   def_fpyr(pyr2,botlev,img2.w,img2.h);
   pyr2[botlev].ptr = img2.ptr;
#ifdef LAPLACIAN
   alloc_fpyr(pyr1,botlev+1,toplev+1);
   alloc_fpyr(pyr2,botlev+1,toplev+1);
#else
   alloc_fpyr(pyr1,botlev+1,toplev);
   alloc_fpyr(pyr2,botlev+1,toplev);
#endif
   def_fpyr(dx,botlev,img1.w,img1.h);
   alloc_fpyr(dx,finlev,toplev);
   def_fpyr(dy,botlev,img1.w,img1.h);
   alloc_fpyr(dy,finlev,toplev);
   def_fpyr(conf,botlev,img1.w,img1.h);
   alloc_fpyr(conf,finlev,toplev);
   
/* coarse to fine motion estimation */
   coarse_fine(pyr1,pyr2,dx,dy,conf,botlev,finlev,toplev);

   sarnoff_f2image (dx_arg, dx[finlev]);
   sarnoff_f2image (dy_arg, dy[finlev]);
   sarnoff_f2image (conf_arg, conf[finlev]);
   free_fimage (img1);
   free_fimage (img2);
#ifdef LAPLACIAN
   free_fpyr(pyr1,botlev+1,toplev+1);
   free_fpyr(pyr2,botlev+1,toplev+1);
#else
   free_fpyr(pyr1,botlev+1,toplev);
   free_fpyr(pyr2,botlev+1,toplev);
#endif
   free_fpyr(dx,finlev,toplev);
   free_fpyr(dy,finlev,toplev);
   free_fpyr(conf,finlev,toplev);
}

void
mot_3ptflow ( image_t *dx_arg, image_t *dy_arg, image_t *img1_arg
            , image_t *img2_arg, float *img1_x, float *img1_y, float *img2_x
            , float *img2_y, int toplev, int botlev)
{
   FIMAGE img1,img2,aff_warp,dx,dy;
   FVEC xi,yi,nxi,nyi;
   float *s, **A, dx_tmp, dy_tmp;
   int i,j;
   int finlev=0;

   /* convert 3 privelged points */
   to_fvect (&xi, img1_x, 3);
   to_fvect (&yi, img1_y, 3);
   to_fvect (&nxi, img2_x, 3);
   to_fvect (&nyi, img2_y, 3);
   /* convert images */
   img1.w = img1_arg->width;
   img1.h = img1_arg->height;
   alloc_fimage (&img1);
   image2sarnoff_f (img1, img1_arg);
   img2.w = img2_arg->width;
   img2.h = img2_arg->height;
   alloc_fimage (&img2);
   image2sarnoff_f (img2, img2_arg);
   if (img1.w != img2.w || img1.h != img2.h) {
     fprintf(stderr,"mot_3ptflow: image size mismatch\n");
     exit(-1);
   }
   aff_warp.w = dx.w = dy.w =  img1.w;
   aff_warp.h = dx.h = dy.h =  img1.h;
   alloc_fimage(&aff_warp);   /* dx,dy are not allocated here */

/* A,s are the six parameters of the 2D affine transformation */

   s=alloc_fvector(1,2);
   A=alloc_farray(1,2,1,2);
   aff_2d(xi,yi,nxi,nyi,A,s);

/* aff_warp is the intermediate image (first image warped with the 2D affine
   transformation).
*/

   bicubic_warp_aff(img1,aff_warp,A,s); 

#ifdef DEBUG
  write_fimage("aff_warp.pix",aff_warp); 
#endif

/* Apply Lucas-Kanade motion estimation */
   lkflow(aff_warp,img2,&dx,&dy,botlev,finlev,toplev);

#ifdef DEBUG
  warp(aff_warp,img2,dx,dy);  /* write onto img2 */
  write_fimage("warped.pix",img2); 
#endif

/* 
   add displacement fields, note that dx,dy are from img2 to aff_warp
   with their sign flipped.
*/

   for (i=0; i<dx.h; i++)
     for (j=0; j<dx.w; j++) {

       dx_tmp = A[1][1]*(j -dx.ptr[i][j])  + A[1][2]*(i - dy.ptr[i][j])
                + s[1] - j;
       dy_tmp = A[2][1]*(j -dx.ptr[i][j]) + A[2][2]*(i - dy.ptr[i][j])
                + s[2] - i;

       dx.ptr[i][j] = - dx_tmp;
       dy.ptr[i][j] = - dy_tmp;
     }

/* new dx,dy are from img2 to img1 with sign flip. */

   sarnoff_f2image (dx_arg, dx);
   sarnoff_f2image (dy_arg, dy);
   free_fimage (img1);
   free_fimage (img2);
   free_fimage (aff_warp);
   free_fimage (dx);
   free_fimage (dy);
}

#define DET_TH 0.5

void
aff_2d (FVEC xi, FVEC yi, FVEC nxi, FVEC nyi, float **A, float *s)
/* 
given three point correspondences compute the 2D affine transformation.
xi,yi coord of first img, nxi,nyi of second image.
For purposing of warping, we compute the inverse transform, i.e., from img2 
to img1.
the arrays dx,dy will contain the affine flow with sign fliped (later the 
affine flow will be added to the flow from img2 to the affine-warped img1).
*/
{

   float det12,x1,x2,y1,y2,nx1,nx2,ny1,ny2;
   float xo=nxi.ptr[0],yo=nyi.ptr[0],nxo=xi.ptr[0],nyo=yi.ptr[0];

   x1 = nxi.ptr[1] - xo; x2 = nxi.ptr[2] - xo;
   y1 = nyi.ptr[1] - yo; y2 = nyi.ptr[2] - yo;
   nx1 = xi.ptr[1] - nxo; nx2 = xi.ptr[2] - nxo;
   ny1 = yi.ptr[1] - nyo; ny2 = yi.ptr[2] - nyo;
   det12 = x1*y2 - y1*x2;
   if (ABS(det12) < DET_TH) {
      printf("det %f of p1,p2,p3 is too small --- 3 pts are nearly colinear\n",det12);
      exit(-1);
   }

   A[1][1] = (nx1*y2 - nx2*y1)/det12;
   A[1][2] = (nx2*x1 - nx1*x2)/det12;
   A[2][1] = (ny1*y2 - ny2*y1)/det12;
   A[2][2] = (ny2*x1 - ny1*x2)/det12;

   s[1] = nxo - A[1][1]*xo - A[1][2]*yo;
   s[2] = nyo - A[2][1]*xo - A[2][2]*yo;
/*
   for (i=0, y=0.0; i<=dx.h; i++, y = y+1.0)
     for (j=0, x=0.0; j<=dx.w; j++, x = x+1.0) {
       dx.ptr[i][j] = (1.0 - A[1][1])*x - A[1][2]*y - s[1];
       dy.ptr[i][j] = - A[2][1]*x + (1.0 - A[2][2])*y - s[2];
     }
*/
#ifdef DEBUG
   printf("constrain line parameters:\n");
   printf("determinant: %f\n", det12);
   printf("a11=%f, a12=%f, a21=%f, a22=%f\n s1=%f, s2=%f\n",
           A[1][1],A[1][2],A[2][1],A[2][2],s[1],s[2]);
#endif
}

void
mot_lsqconf_mask ( image_t *dx_arg, image_t *dy_arg, image_t *conf_arg
                 , image_t *img1_arg, image_t *img2_arg, image_t *mask_arg
                 , int toplev, int botlev)
{
   char filtfile[MAX_STR_LEN];
   FPYR pyr1,pyr2,mask,dx,dy,conf;
   FIMAGE img1,img2,imgmask;
   int finlev=0;

   strcpy_s(filtfile,MAX_STR_LEN,DEFAULT_PYR_FILTER);	/* default filter */
   getpyrfilters(filtfile,&rf,&ef);
   img1.w = img1_arg->width;
   img1.h = img1_arg->height;
   alloc_fimage (&img1);
   image2sarnoff_f (img1, img1_arg);
   img2.w = img2_arg->width;
   img2.h = img2_arg->height;
   alloc_fimage (&img2);
   image2sarnoff_f (img2, img2_arg);
   imgmask.w = mask_arg->width;
   imgmask.h = mask_arg->height;
   alloc_fimage (&imgmask);
   image2sarnoff_f (imgmask, mask_arg);

   if (img1.w != img2.w || img1.h != img2.h) {
     fprintf(stderr,"mot_lsqconf: image size mismatch\n");
     exit(-1);
   }
/* Set up pyramids for coarse to fine estimation */
   def_fpyr(pyr1,botlev,img1.w,img1.h);
   pyr1[botlev].ptr = img1.ptr;
   def_fpyr(pyr2,botlev,img2.w,img2.h);
   pyr2[botlev].ptr = img2.ptr;
#ifdef LAPLACIAN
   alloc_fpyr(pyr1,botlev+1,toplev+1);
   alloc_fpyr(pyr2,botlev+1,toplev+1);
#else
   alloc_fpyr(pyr1,botlev+1,toplev);
   alloc_fpyr(pyr2,botlev+1,toplev);
#endif
   def_fpyr(mask,botlev,imgmask.w,imgmask.h);
   mask[botlev].ptr = imgmask.ptr;
   alloc_fpyr(mask,botlev+1,toplev);
   def_fpyr(dx,botlev,img1.w,img1.h);
   alloc_fpyr(dx,finlev,toplev);
   def_fpyr(dy,botlev,img1.w,img1.h);
   alloc_fpyr(dy,finlev,toplev);
   def_fpyr(conf,botlev,img1.w,img1.h);
   alloc_fpyr(conf,finlev,toplev);
   
/* coarse to fine motion estimation */
   coarse_fine_mask(pyr1,pyr2,mask,dx,dy,conf,botlev,finlev,toplev);

   sarnoff_f2image (dx_arg, dx[finlev]);
   sarnoff_f2image (dy_arg, dy[finlev]);
   sarnoff_f2image (conf_arg, conf[finlev]);
   free_fimage (img1);
   free_fimage (img2);
   free_fimage (imgmask);
#ifdef LAPLACIAN
   free_fpyr(pyr1,botlev+1,toplev+1);
   free_fpyr(pyr2,botlev+1,toplev+1);
#else
   free_fpyr(pyr1,botlev+1,toplev);
   free_fpyr(pyr2,botlev+1,toplev);
#endif
   free_fpyr(mask,botlev+1,toplev);
   free_fpyr(dx,finlev,toplev);
   free_fpyr(dy,finlev,toplev);
   free_fpyr(conf,finlev,toplev);
}

void
mot_warp ( image_t *dest_arg, image_t *source_arg
         , image_t *dx_arg, image_t *dy_arg, double scale)

{
  image_t *tmp;
  FIMAGE in, out, dx, dy;

  in.w = source_arg->width;	in.h = source_arg->height;
  alloc_fimage (&in);
  image2sarnoff_f (in, source_arg);
  dx.w = dx_arg->width;		dx.h = dx_arg->height;
  alloc_fimage (&dx);
  image2sarnoff_f (dx, dx_arg);
  dy.w = dy_arg->width;		dy.h = dy_arg->height;
  alloc_fimage (&dy);
  image2sarnoff_f (dy, dy_arg);
  out.w = in.w;  out.h = in.h;   alloc_fimage(&out);

  bilinear_warp (in, out, dx, dy, scale);

  switch (dest_arg->type) {
  case IMG_U8BIT:
    tmp = allocate_image (dest_arg->width, dest_arg->height, IMG_FLOAT);
    sarnoff_f2image (tmp, out);
    fix_image (dest_arg, tmp);
    deallocate_image (tmp);
    break;
  case IMG_FLOAT:
    sarnoff_f2image (dest_arg, out);
    break;
  }
  free_fimage (in);
  free_fimage (out);
  free_fimage (dx);
  free_fimage (dy);
}

/****************************************************************************
 *
 * conversion between internal image formats
 *
 */

void image2sarnoff_f(FIMAGE fimg, image_t *image)
{
  char *char_ptr;
  unsigned char *uchar_ptr;
  int x, y;
  float *fimg_ptr, *float_ptr;

  switch (image->type) {
  case IMG_1BIT:
    for (y = 0; y < image->height; y++) {
      char_ptr = (char *) image->image[y];
      fimg_ptr = (float *) fimg.ptr[y];
      for (x = 0; x < image->width; x++) {
	if (getBit(char_ptr,x))
	  *fimg_ptr++ = 1.0;
	else
	  *fimg_ptr++ = 0.0;
      }
    }
    break;
  case IMG_U8BIT:
    for (y = 0; y < image->height; y++) {
      uchar_ptr = (unsigned char *) image->image[y];
      fimg_ptr = (float *) fimg.ptr[y];
      for (x = 0; x < image->width; x++) {
	*fimg_ptr++ = (float) *uchar_ptr++;
      }
    }
    break;
  case IMG_FLOAT:
    for (y = 0; y < image->height; y++) {
      float_ptr = (float *) image->image[y];
      fimg_ptr = (float *) fimg.ptr[y];
      for (x = 0; x < image->width; x++) {
	*fimg_ptr++ = *float_ptr++;
      }
    }
    break;
  default:
    fprintf (stderr, "image2sarnoff_f: unsupported type.\n");
    break;
  }
}

void sarnoff_f2image(image_t *image, FIMAGE fimg)
{
  int x, y;
  float *fimg_ptr, *float_ptr;

  if (image->type != IMG_FLOAT) {
    fprintf (stderr, "sarnoff_f2image:  image must be of type float.\n");
  } else {
    for (y = 0; y < image->height; y++) {
      float_ptr = (float *) image->image[y];
      fimg_ptr = (float *) fimg.ptr[y];
      for (x = 0; x < image->width; x++) {
	*float_ptr++ = *fimg_ptr++;
      }
    }
  }
}

void to_fvect(FVEC *fvect_v, float *v, int n)
{
  int i;

  fvect_v->ptr = (float *) calloc (n, sizeof (float));
  fvect_v->s = n;
  for (i = 0; i < n; i++)
    fvect_v->ptr[i] = v[i];
}
