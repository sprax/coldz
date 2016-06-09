/*****************************************************
 interface.h -- for Beymer's interface to RH's code
 *****************************************************/
#ifndef beymer_interface_h__
#define beymer_interface_h__
 
#include "sarnoff.h"

typedef struct {
   float *ptr;
   int s;            /* size */
} FVEC;


void
mot_lsqconf ( image_t *dx_arg, image_t *dy_arg, image_t *conf_arg
            , image_t *img1_arg, image_t *img2_arg, int toplev, int botlev);

void
mot_3ptflow ( image_t *dx_arg, image_t *dy_arg, image_t *img1_arg
            , image_t *img2_arg, float *img1_x, float *img1_y, float *img2_x
            , float *img2_y, int toplev, int botlev);

void
mot_lsqconf_mask ( image_t *dx_arg, image_t *dy_arg, image_t *conf_arg
                 , image_t *img1_arg, image_t *img2_arg, image_t *mask_arg
                 , int toplev, int botlev);

void
mot_warp ( image_t *dest_arg, image_t *source_arg
         , image_t *dx_arg, image_t *dy_arg, double scale);


void
aff_2d (FVEC xi, FVEC yi, FVEC nxi, FVEC nyi, float **A, float *s);


void image2sarnoff_f(FIMAGE fimg, image_t *image);

void sarnoff_f2image(image_t *image, FIMAGE fimg);

void to_fvect(FVEC *fvect_v, float *v, int n);

#endif	/* beymer_interface_h__ */


