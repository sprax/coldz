/******************************************************************************
 morphBase.c    S.R.Lines    94.1.23-26
 rewrite of David Beymer's morph_base.c (March, 1993)
 ******************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#define		NDEBUG 1
#include	<assert.h>
#include	<math.h>
#include	<string.h>
#include	<un/unBug.h>
#include	<un/unMath.h>
#include	<un/unTypes.h>
#include	<rowMake.h>
#include	<rowCopy.h>
#include	<rowWarp.h>
#include "beymer/image.h"
#include "beymer/alloc.h"
#include "beymer/strel.h"
#include "beymer/geo.h"
#include "beymer/gradient.h"
#include "beymer/morph.h"
#include "beymer/lists.h"
#include "beymer/label.h"
#include "beymer/interface.h"
#include	"allocImage.h"
#include	"imageGrey.h"
#include	"imageGrey2.h"
#include	"mj.h"
#include	"new.h"
#include	"labelBase.h"
#include	"rpBase.h"
#include	"morphBase.h" 
#include        "new.h"

#if 1	/* new, efficient motWarp */
void
motWarp ( image_t *dest, image_t *source
        , image_t *dx_arg, image_t *dy_arg, double scale)
{
unt	dW = dest->width,   dH = dest->height;
unt	sW = source->width, sH = source->height;
ppf	tmp, src;

  switch(source->type) {
    case IMG_S8BIT:
    case IMG_U8BIT:
      src = rowMalFlt(0,0,sW,sH);
      rowCopyUncToFlt(src,(ppu)source->image,sW,sH);
      break;
    case IMG_FLOAT:
      src = (ppf)source->image;
      break;
    default: die("motWarp: NOIMP for source type %d",source->type);
  }

  if (dest->type != IMG_FLOAT)
    tmp = rowMalFlt(0,0,dW,dH);
  else
    tmp = (ppf)dest->image;

  rowWarpBilFlt(tmp,src,(ppf)dx_arg->image,(ppf)dy_arg->image,0,0,dW,dH,0,0,sW,sH,scale);

  switch(dest->type) {
    case IMG_S8BIT:
    case IMG_U8BIT:
      rowCopyFltToUnc((ppu)dest->image,tmp,dW,dH);
      rowKillFlt(tmp,0,0);
      break;
    case IMG_FLOAT:
      dest->image = (char **)tmp;
      break;
    default: die("motWarp: NOIMP for dest type %d",dest->type);
  }
  if (source->type != IMG_FLOAT)
    rowKillFlt(src,0,0);
}

#else	/* BEG obsolete motWarp */

#include	<imagmem.h>
void
motWarp ( image_t *dest, image_t *source
        , image_t *dx_arg, image_t *dy_arg, double scale)
{
unt	dW = dest->width,   dH = dest->height;
unt	sW = source->width, sH = source->height, sType;
image_t	*tmp;
FIMAGE	in, out, dx, dy;

  in.w = sW = source->width;
  in.h = sH = source->height;
  alloc_fimage (&in);

  switch(source->type) {
    case IMG_S8BIT:
    case IMG_U8BIT: sType = MM_UNC;  break;
    case IMG_FLOAT: sType = MM_FLT;  break;
    default: die("motWarp: NOIMP for source type %d",source->type);
  }
  rowCopyToFlt(in.ptr,(void **)source->image,sW,sH,0,0,sType);

  dx.w = dy.w = out.w = dW;
  dx.h = dy.h = out.h = dH;
  alloc_fimage(&out);

#define ALLOF 0
#if	ALLOF
  alloc_fimage(&dx);
  alloc_fimage(&dy);
  image2sarnoff_f(dx, dx_arg);
  image2sarnoff_f(dy, dy_arg);
#else
  dx.ptr = (ppf)dx_arg->image;
  dy.ptr = (ppf)dy_arg->image;
#endif

  /* was: bilinear_warp (in, out, dx, dy, scale); */
  rowWarpBilFlt(out.ptr,in.ptr,dx.ptr,dy.ptr,0,0,dW,dH,0,0,sW,sH,scale);

  switch (dest->type) {
  case IMG_U8BIT:
    tmp = allocateImage (dest->width, dest->height, IMG_FLOAT);
    rowCopyFltToUnc((ppu)tmp->image,out.ptr,dW,dH);
    fixImage(dest, tmp);
    deallocateImage(tmp);
    free_fimage(out);
    break;
  case IMG_FLOAT:
    dest->image = (char **)out.ptr;
    break;
  }
  free_fimage (in);
#if ALLOF
  free_fimage (dx);
  free_fimage (dy);
#endif
}
#endif	/* END: obsolete motWarp */


	/* assumes face is horizontally aligned:	*/

/* rotate and scale image2 so that its eye positions match those of image 1 */

ppu
matchEyeLocations ( ppu image2, int dX, int dY, unt dW, unt dH
                  , int sX, int sY, unt sW, unt sH, List labels2, List labels1)
{
  float dx[2], dy[2], sx[2], sy[2];
  img_label_t *img_label, *l_eye1, *r_eye1, *l_eye2, *r_eye2;
  ppu dst;

  if (((l_eye1 = findImgLabel ("l_eye", labels1)) == NULL) ||
      ((r_eye1 = findImgLabel ("r_eye", labels1)) == NULL) ||
      ((l_eye2 = findImgLabel ("l_eye", labels2)) == NULL) ||
      ((r_eye2 = findImgLabel ("r_eye", labels2)) == NULL))
    die("matchEyeLocations: Couldn't find left and right eyes.");
  sx[0] = (flt) l_eye2->x;		sy[0] = (flt) l_eye2->y;
  sx[1] = (flt) r_eye2->x;		sy[1] = (flt) r_eye2->y;
  dx[0] = (flt) l_eye1->x;		dy[0] = (flt) l_eye1->y;
  dx[1] = (flt) r_eye1->x;		dy[1] = (flt) r_eye1->y;

  dst = rowMalUnc(dX,dY,dW,dH);
  warpRotScale(dst,image2, dX,dY,dW,dH, sX,sY,sW,sH, dx,dy,sx,sy);

  /* transform label points */
  while (labels2) {
    img_label = (img_label_t *) L_CAR(labels2);
    transformRotScale (&(img_label->x), &(img_label->y), dx, dy, sx, sy);
    labels2 = L_CDR(labels2);
  }
  return dst;
}


ppu
matchEyeLocTmp ( ppu src
, int dX, int dY, unt dW, unt dH
, int sX, int sY, unt sW, unt sH
, delEyeLoc *del
, List labels2, List labels1
)
{
AffTrf  aT;
Dmj     dD,sD;
Rexy    dstR, srcR;

  img_label_t *l_eye1, *r_eye1, *l_eye2, *r_eye2;
  ppu dst = rowMalUnc(dX,dY,dW,dH);


  if (((l_eye1 = findImgLabel ("l_eye", labels1)) == NULL) ||
      ((r_eye1 = findImgLabel ("r_eye", labels1)) == NULL) ||
      ((l_eye2 = findImgLabel ("l_eye", labels2)) == NULL) ||
      ((r_eye2 = findImgLabel ("r_eye", labels2)) == NULL))
    die("matchEyeLocations: Couldn't find left and right eyes.");

  del->olx = (flt) l_eye2->x;         del->oly = (flt) l_eye2->y; /* eyeLocs in inpU[j>0] */
  del->orx = (flt) r_eye2->x;         del->ory = (flt) r_eye2->y;
  del->nlx = (flt) l_eye1->x;         del->nly = (flt) l_eye1->y; /* eyeLocs in wrs[0] -- ideal */
  del->nrx = (flt) r_eye1->x;         del->nry = (flt) r_eye1->y;

  dstR.x1 = (flt) l_eye1->x;
  dstR.y1 = (flt) l_eye1->y;
  dstR.x2 = (flt) r_eye1->x;
  dstR.y2 = (flt) r_eye1->y;

  srcR.x1 = (flt) l_eye2->x;
  srcR.y1 = (flt) l_eye2->y;
  srcR.x2 = (flt) r_eye2->x;
  srcR.y2 = (flt) r_eye2->y;

#if 1
  affTrfFromTwoPts(&aT, &dstR, &srcR, 1.0, 1.0);	
#else
  affTrfFromTwoPts(&aT, &dstR, &srcR, fracAngle,fracTrans);	/* @@ */
#endif

  DIM_PAR(dD,dX,dY,dW,dH,1);
  DIM_PAR(sD,sX,sY,sW,sH,1);

  rowWarpAff(dst,src,&dD,&sD,&aT);

  affTrfLbl(labels2,&aT,1.0,del);  /* probably not needed */

  return dst;
}


void
extractWholeFace ( ppu *dst, unt *dW, unt *dH, ppu src
                 , int dX, int dY, int dR, int dB
                 , int sX, int sY, unt sW, unt sH, List image_labels )
{
double	dx, dy, dist, ul_x, ul_y, lr_x, lr_y;
img_label_t *l_eye, *r_eye, *img_label;
int	oX, oY, gX, gY;
unt	oW, oH;

  if (((l_eye = findImgLabel ("l_eye", image_labels)) == NULL) ||
      ((r_eye = findImgLabel ("r_eye", image_labels)) == NULL)) {
    die("extractWholeFace: Couldn't find left and right eyes.\n");
  } else {

    dx   = r_eye->x - l_eye->x;
    dy   = r_eye->y - l_eye->y;
    dist = sqrt(dx*dx + dy*dy);
    ul_x = l_eye->x - 0.8*dist;
    lr_x = r_eye->x + 0.8*dist;
    ul_y = l_eye->y - 0.6*dist;
    lr_y = r_eye->y + 2.0*dist;
    oX   = irint (ul_x);
    oY   = irint (ul_y);
    oW   = irint (lr_x - ul_x);
    oH   = irint (lr_y - ul_y);
    *dW  = oW - dX + dR;	/* Expect (dX <= 0), with */
    *dH  = oH - dY + dB;	/* dX < 0 for positive-width border */
    *dst = rowMalUnc(  dX,dY,*dW,*dH);
    gX = oX+dX;  gY = oY+dY;

    assert(gX >= 0 && gY >= 0);
    assert(gX+*dW < sX+sW && gY+*dH < sY+sH);

    rowCopyUU(*dst,src,dX,dY,*dW,*dH,gX,gY);

    while (image_labels) {
      img_label     = (img_label_t *) L_CAR(image_labels);
      img_label->x -= oX;	/* Adjust offsets from nominal origin */
      img_label->y -= oY;	/* ie from dst[0][0] */
      image_labels  = L_CDR(image_labels);
    }
  }
}





/* assumes face is horizontally aligned */
void
extractWholeFace2(image_t **subImage, image_t *image, List image_labels)
{
  int x, y, width, height;
  double dx, dy, dist, ul_x, ul_y, lr_x, lr_y;
  img_label_t *img_label, *l_eye, *r_eye;

  if (((l_eye = findImgLabel ("l_eye", image_labels)) == NULL) ||
      ((r_eye = findImgLabel ("r_eye", image_labels)) == NULL)) {
    fprintf (stderr,
	     "extract_whole_face: couldn't find left and right eyes.\n");
  } else {
    dx = r_eye->x - l_eye->x;
    dy = r_eye->y - l_eye->y;
    dist = sqrt(dx*dx + dy*dy);
    ul_x = l_eye->x - .6*dist;
    lr_x = r_eye->x + .6*dist;
    ul_y = l_eye->y - .4*dist;
    lr_y = r_eye->y + 1.8*dist;
    x = irint (ul_x);
    y = irint (ul_y);
    width = irint (lr_x - ul_x);
    height = irint (lr_y - ul_y);
    *subImage = allocateImage (width, height, IMG_U8BIT);
    extractImage(*subImage, image, x, y, width, height);
    while (image_labels) {
      img_label = (img_label_t *) L_CAR(image_labels);
      img_label->x -= x;
      img_label->y -= y;
      image_labels = L_CDR(image_labels);
    }
  }
}


/* assumes face is horizontally aligned */
void
extractWholeFaceSmall(image_t **subImage,image_t *image,List image_labels)
{
  int x, y, width, height;
  double dx, dy, dist, ul_x, ul_y, lr_x, lr_y;
  img_label_t *l_eye, *r_eye, *img_label;

  if (((l_eye = findImgLabel ("l_eye", image_labels)) == NULL) ||
      ((r_eye = findImgLabel ("r_eye", image_labels)) == NULL)) {
    fprintf (stderr,
	     "extractWholeFaceSmall: couldn't find left and right eyes.\n");
  } else {
    dx = r_eye->x - l_eye->x;
    dy = r_eye->y - l_eye->y;
    dist = sqrt(dx*dx + dy*dy);
    ul_x = l_eye->x - .6*dist;
    lr_x = r_eye->x + .6*dist;
    ul_y = l_eye->y - .6*dist;
    lr_y = r_eye->y + 1.8*dist;
    x = irint (ul_x);
    y = irint (ul_y);
    width = irint (lr_x - ul_x);
    height = irint (lr_y - ul_y);
    *subImage = allocateImage (width, height, image->type);
    extractImage(*subImage, image, x, y, width, height);
    while (image_labels) {
      img_label = (img_label_t *) L_CAR(image_labels);
      img_label->x -= x;
      img_label->y -= y;
      image_labels = L_CDR(image_labels);
    }
  }
}


/**************************************************************************
NB: extractWholeFaceBordLR produces generic,
primary images -- the row ptrs are not adjusted to point to an interior
or sub- image, but rather, the images are just increased in size by the
absolute number of pixels in both directions: LTborder pixels on the Top
and Left sides, and RBborder pixels on the Right and Bottom sides.
This "border" is to be used for pixval interpolation (by warping routines,
etc.) -- S.R.Lines
**************************************************************************/
void
extractWholeFaceBordLR ( image_t **subImage, image_t *image, List image_labels
                       , int LTborder, int RBborder)
{
  int x, y, width, height;
  double dx, dy, dist, ul_x, ul_y, lr_x, lr_y;
  img_label_t *l_eye, *r_eye=NULL, *img_label;

  if (((l_eye = findImgLabel ("l_eye", image_labels)) == NULL) ||
      ((r_eye = findImgLabel ("r_eye", image_labels)) == NULL))
    die("extractWholeFaceBordLR: couldn't find left and right eyes.\n");

  dx = r_eye->x - l_eye->x;
  dy = r_eye->y - l_eye->y;
  dist = sqrt(dx*dx + dy*dy);
  ul_x = l_eye->x - .8*dist;
  lr_x = r_eye->x + .8*dist;
  ul_y = l_eye->y - .6*dist;
  lr_y = r_eye->y + 1.8*dist;
  x = irint (ul_x) - LTborder;
  y = irint (ul_y) - LTborder;
  width  = irint (lr_x - ul_x) + LTborder + RBborder;
  height = irint (lr_y - ul_y) + LTborder + RBborder;

  if (x < 0 || y < 0 || x+width > image->width || y+height > image->height)
    die("extractWholeFaceBordLR: subimage from %d %d, size %d %d\n\
	would go outside the bounds of primary image, size %d %d\n\
	with LT & RB border sizes %d & %d\n"
        ,x,y,width,height,image->width,image->height,LTborder,RBborder);

  *subImage = allocateImage (width, height, image->type);
  extractImage(*subImage, image, x, y, width, height);
  while (image_labels) {
    img_label = (img_label_t *) L_CAR(image_labels);
    img_label->x -= x;
    img_label->y -= y;
    image_labels = L_CDR(image_labels);
  }
}



#if	0000

void
crossDissolve ( image_t *seq[], image_t *image1, image_t *image2
               , image_t *dx, image_t *dy, int num_inbetween)
{
  int i;
  float alpha;
  image_t *mix1, *mix2;

  mix1 = allocateImageLike (image1);
  mix2 = allocateImageLike (image2);
  for (i = 0; i <= num_inbetween; i++) {
    seq[i] = allocateImageLike (mix1);
    alpha = (double) i / num_inbetween;
    motWarp (mix1, image1, dx, dy, (double) -alpha);
    motWarp (mix2, image2, dx, dy, (double) (1.0 - alpha));
    scaleImage (mix1, mix1, (double) (1.0 - alpha));
    scaleImage (mix2, mix2, (double) alpha);
    addImages (seq[i], mix1, mix2);
  }
  deallocateImage (mix1);
  deallocateImage (mix2);
}



/* uses new "rend" routine */
void
crossDissolveRend ( image_t *seq[], image_t *image1, image_t *image2
                    , image_t *dx, image_t *dy, int num_inbetween)
{
  int i;
  float alpha;
  image_t *mix1, *mix2, *dx_inter, *dy_inter;

  mix1 = allocateImageLike (image1);
  mix2 = allocateImageLike (image2);
  dx_inter = allocateImageLike (dx);
  dy_inter = allocateImageLike (dy);

  for (i = 0; i <= num_inbetween; i++) {
    seq[i] = allocateImageLike (mix1);
    alpha = (double) i / num_inbetween;
    scaleImage (dx_inter, dx, alpha);
    scaleImage (dy_inter, dy, alpha);
    rend (mix1, dx_inter, dy_inter, image1, NULL, NULL);
    rend (mix2, dx_inter, dy_inter, image2, dx, dy);
    scaleImage (mix1, mix1, (double) (1.0 - alpha));
    scaleImage (mix2, mix2, (double) alpha);
    addImages (seq[i], mix1, mix2);
  }

  deallocateImage (mix1);
  deallocateImage (mix2);
  deallocateImage (dx_inter);
  deallocateImage (dy_inter);
}



/* THIS IS THE (LATEST) REPLACEMENT OF multi_dissolve, 23 Jan 94 */
void
multiDissoNew ( image_t *dst, image_t *image[]
              , image_t *dx[], image_t *dy[], double alpha, double beta)
{
  int i;
  double s[4];
  image_t *dx_bilin, *dy_bilin, *dx_prime[3], *dy_prime[3];
  image_t *mix[4];

  s[0] = (1.0 - alpha)*(1.0 - beta); /* coeff of image 0 */
  s[1] = alpha*(1.0 - beta);	/* coeff of image 1 */
  s[2] = (1.0 - alpha)*beta;	/* coeff of image 2 */
  s[3] = alpha*beta;		/* coeff of image 3 */
  dx_bilin = allocateImageLike (dx[0]);
  dy_bilin = allocateImageLike (dy[0]);
  for (i = 0; i < 3; i++) {
    dx_prime[i] = allocateImageLike (dx[0]);
    dy_prime[i] = allocateImageLike (dy[0]);
  }
  for (i = 0; i < 4; i++)
    mix[i] = allocateImageLike (image[i]);
  interpolateWarping (dx_bilin, dy_bilin, dx, dy, alpha, beta);
  for (i = 0; i < 3; i++) {
    subtractImages (dx_prime[i], dx[i], dx_bilin);
    subtractImages (dy_prime[i], dy[i], dy_bilin);
  }
  motWarp (mix[0], image[0], dx_bilin, dy_bilin, (double) -1.0);
  scaleImage (mix[0], mix[0], s[0]);
  for (i = 1; i < 4; i++) {
    motWarp (mix[i], image[i], dx_prime[i-1], dy_prime[i-1], (double) 1.0);
    scaleImage (mix[i], mix[i], s[i]);
  }
  addImages (dst, mix[0], mix[1]);
  addImages (dst, dst, mix[2]);
  addImages (dst, dst, mix[3]);

  deallocateImage (dx_bilin);
  deallocateImage (dy_bilin);
  for (i = 0; i < 3; i++) {
    deallocateImage (dx_prime[i]);
    deallocateImage (dy_prime[i]);
  }
  for (i = 0; i < 4; i++)
    deallocateImage (mix[i]);
}


/*    image setup    0  1
 *                   2  3
 */

void
interpolateWarping ( image_t *dx_bilin, image_t *dy_bilin
                    , image_t *dx[], image_t *dy[], double alpha, double beta)
{
  int i, x, y;
  float *dx_bilin_ptr, *dy_bilin_ptr, *dx_ptr[3], *dy_ptr[3];
  double c0, c1, c2;

  c0 = alpha*(1.0 - beta);	/* coeff of image 1 */
  c1 = (1.0 - alpha)*beta;	/* coeff of image 2 */
  c2 = alpha*beta;		/* coeff of image 3 */
  for (y = 0; y < dx_bilin->height; y++) {
    dx_bilin_ptr = (float *) dx_bilin->image[y];
    dy_bilin_ptr = (float *) dy_bilin->image[y];
    for (i = 0; i < 3; i++)
      dx_ptr[i] = (float *) dx[i]->image[y];
    for (i = 0; i < 3; i++)
      dy_ptr[i] = (float *) dy[i]->image[y];
    for (x = 0; x < dx_bilin->width; x++) {
      *dx_bilin_ptr++ = c0*(*dx_ptr[0]++) + c1*(*dx_ptr[1]++)
	+ c2*(*dx_ptr[2]++);
      *dy_bilin_ptr++ = c0*(*dy_ptr[0]++) + c1*(*dy_ptr[1]++)
	+ c2*(*dy_ptr[2]++);
    }
  }
}


void
rendImage ( image_t *img, image_t *dx, image_t *dy
          , image_t *img_ref, image_t *dx_ref, image_t *dy_ref)
{
  image_t *ddx, *ddy, *dx_new, *dy_new;

  if ((dx_ref == NULL) || (dy_ref == NULL)) {
    motWarp (img, img_ref, dx, dy, (double) -1.0);
  } else {
    ddx = allocateImageLike (dx);
    ddy = allocateImageLike (dx);
    dx_new = allocateImageLike (dx);
    dy_new = allocateImageLike (dx);

    subtractImages (ddx, dx, dx_ref);
    subtractImages (ddy, dy, dy_ref);
    motWarp (dx_new, ddx, dx_ref, dy_ref,  (double) -1.0);
    motWarp (dy_new, ddy, dx_ref, dy_ref,  (double) -1.0);
    motWarp (img, img_ref, dx_new, dy_new, (double) -1.0);

    deallocateImage (ddx);
    deallocateImage (ddy);
    deallocateImage (dx_new);
    deallocateImage (dy_new);
  }
}


int
readPoseIndex (char *pathname)
{
  char *num_str;
  int pose_index;

  if ((num_str = strstr (pathname, "/m")) == NULL) {
    fprintf (stderr, "model names must end with \"/m<number>\" suffix.\n");
    return -1;
  } else {
    num_str += 2;
    if (sscanf (num_str, "%d", &pose_index) != 1) {
      fprintf (stderr, "model names must end with \"/m<number>\" suffix.\n");
      return -1;
    } else {
      return pose_index;
    }
  }
}


void
multiDissolve ( image_t *dst, image_t *image[]
              , image_t *dx[], image_t *dy[], double alpha, double beta)
{
  int i;
  double s[4];
  image_t *dx_bilin, *dy_bilin, *dx_prime[3], *dy_prime[3];
  image_t *mix[4];

  s[0] = (1.0 - alpha)*(1.0 - beta); /* coeff of image 0 */
  s[1] = alpha*(1.0 - beta);	/* coeff of image 1 */
  s[2] = (1.0 - alpha)*beta;	/* coeff of image 2 */
  s[3] = alpha*beta;		/* coeff of image 3 */
  dx_bilin = allocateImageLike (dx[0]);
  dy_bilin = allocateImageLike (dy[0]);
  for (i = 0; i < 3; i++) {
    dx_prime[i] = allocateImageLike (dx[0]);
    dy_prime[i] = allocateImageLike (dy[0]);
  }
  for (i = 0; i < 4; i++)
    mix[i] = allocateImageLike (image[i]);
  interpolateWarping (dx_bilin, dy_bilin, dx, dy, alpha, beta);
  for (i = 0; i < 3; i++) {
    subtractImages (dx_prime[i], dx[i], dx_bilin);
    subtractImages (dy_prime[i], dy[i], dy_bilin);
  }
  motWarp (mix[0], image[0], dx_bilin, dy_bilin, (double) -1.0);
  scaleImage (mix[0], mix[0], s[0]);
  for (i = 1; i < 4; i++) {
    motWarp (mix[i], image[i], dx_prime[i-1], dy_prime[i-1], (double) 1.0);
    scaleImage (mix[i], mix[i], s[i]);
  }
  addImages (dst, mix[0], mix[1]);
  addImages (dst, dst, mix[2]);
  addImages (dst, dst, mix[3]);

  deallocateImage (dx_bilin);
  deallocateImage (dy_bilin);
  for (i = 0; i < 3; i++) {
    deallocateImage (dx_prime[i]);
    deallocateImage (dy_prime[i]);
  }
  for (i = 0; i < 4; i++)
    deallocateImage (mix[i]);
}


void
readImgLabelCoords (float *x, float *y, char *label_names[], int n, List img_labels)
{
  int i;
  img_label_t *img_label;

  for (i = 0; i < n; i++) {
    if ((img_label = findImgLabel (label_names[i], img_labels)) == NULL) {
      fprintf (stderr, "couldn't find label %s\n", label_names[i]);
      exit (-1);
    } else {
      x[i] = (float) img_label->x;      y[i] = (float) img_label->y;
    }
  }
}


#endif	/* 0000 */

