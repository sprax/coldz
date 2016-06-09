/*****************************************************************************
 morphBase.h -- prototypes from morphBase.c   94.1.23
#include <unTypes.h>
 *****************************************************************************/

ppu
matchEyeLocTmp ( ppu src
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, delEyeLoc *del 
, List labels2, List labels1);


void
extractWholeFace ( ppu *dst, unt *dW, unt *dH, ppu src
                 , int dX, int dY, int dR, int dB
                 , int sX, int sY, unt sW, unt sH, List image_labels);

ppu
matchEyeLocations ( ppu image2, int dX, int dY, unt dW, unt dH
                  , int sX, int sY, unt sW, unt sH
                  , List labels2, List labels1);

#if 0

void
motWarp ( image_t *dest_arg, image_t *source_arg
        , image_t *dx_arg, image_t *dy_arg, double scale);

void
extractWholeFace2(image_t **subImage, image_t *image, List image_labels);

void
extractWholeFace_small(image_t **subImage,image_t *image,List image_labels);

void
extractWholeFaceBordLR ( image_t **subImage, image_t *image, List image_labels
                       , int LTborder, int RBborder);

void
crossDissolve ( image_t *seq[], image_t *image1, image_t *image2
               , image_t *dx, image_t *dy, int numInbetween);

void
crossDissolveRend ( image_t *seq[], image_t *image1, image_t *image2
                    , image_t *dx, image_t *dy, int numInbetween);

void
multiDissolve ( image_t *dest, image_t *image[]
              , image_t *dx[], image_t *dy[], double alpha, double beta);

void
multiDissoNew ( image_t *dest, image_t *image[]
              , image_t *dx[], image_t *dy[], double alpha, double beta);

void
interpolateWarping ( image_t *dx_bilin, image_t *dy_bilin
                    , image_t *dx[], image_t *dy[], double alpha, double beta);

void
rend ( image_t *img, image_t *dx, image_t *dy
     , image_t *imgRef, image_t *dxRef, image_t *dyRef);

int
readPoseIndex (char *pathname);

void
readImgLabelCoords(float *x, float *y, char *label_names[], int n, List img_labels);

#endif



