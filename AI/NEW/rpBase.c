/******************************************************************************
 rpBase.c    S.R.Lines    94.1.23
 rewrite of David Beymer's rp_base.c (Feb. 93)
 ******************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>
#include	<un/unBug.h>
#include	<un/unTypes.h>
#include	<un/unMath.h>
#include	<un/unFlags.h>
#include	<un/unMacro.h>
#include "beymer/image.h"
#include "beymer/interface.h"
#include "beymer/alloc.h"
#include "beymer/gradient.h"
#include "beymer/strel.h"
#include "beymer/macros.h"
#include "beymer/geo.h"
#include "beymer/image_binary.h"
#include "beymer/image_grey.h"
#include "beymer/image_grey2.h"
#include "beymer/colors.h"
#include "beymer/boolean.h"
#include "beymer/lists.h"
#include "beymer/label.h"
#include "beymer/rp.h"
#include "beymer/io.h"
#include "beymer/morph.h"
#include "beymer/smooth.h"
#include	<rowMake.h>
#include	"imageGrey2.h"
#include	"labelBase.h"
#include	"mj.h"
#include	"new.h"
#include	"morphBase.h"
#include	"rpBase.h"

int readFaceLbl( LblFace *lbl, char *name)
{
char    path[128];
List    labels;
img_label_t *il;
int 	j=0;

  sprintf(path,"%s.lbl",name);

  if ( ! (labels = readImgLabels(path))) {           /** READ LABELS **/
    if (Options & O_VERB)
      wrn("readFaceLbl(%s)[%d] - No label file found", path, 0);
    return 0;
  }

  if (! (il = findImgLabel("l_eye", labels)))
    die("readFaceLbl: Couldn't find left eye");
  lbl->lex = (flt) il->x;
  lbl->ley = (flt) il->y;

  if (! (il = findImgLabel("r_eye", labels)))
    die("readFaceLbl: Couldn't find right eye");
  lbl->rex = (flt) il->x;
  lbl->rey = (flt) il->y;

  if (! (il = findImgLabel("center", labels)))
    die("readFaceLbl: Couldn't find center");
  lbl->nox = (flt) il->x;
  lbl->noy = (flt) il->y;

  if ((il = findImgLabel("mole", labels))) {
    wrn("readFaceLbl: This face has a MOLE in it at %1.5f %1.5f",il->x,il->y);
    lbl->mex = (flt) il->x;
    lbl->mey = (flt) il->y;
    wrn("readFaceLbl: %1.5f %1.5f   %1.5f %1.5f   %1.5f %1.5f   %1.5f %1.5f"
    ,lbl->lex,lbl->ley,lbl->rex,lbl->rey,lbl->nox,lbl->noy,lbl->mex,lbl->mey);
    return 4;
  }
  else lbl->mex = lbl->mey = 0.0;
  return 3;
}


void
lexy (flt *x, flt *y, List labels)
{
img_label_t *l_eye;

  if (! (l_eye = findImgLabel("l_eye", labels)))
    die("lexy: Couldn't find left eye");
  *x = (flt) l_eye->x;
  *y = (flt) l_eye->y;
}

double
interocularDist (int pose, int width, double fracWidth, double horzRotDeg)
{
  double horizRadians=0.0;
  switch (pose) {
    case  0: horizRadians = horzRotDeg*M_PI/180.0;	break;
    case  1: case 5: case 6: case 10: case 11: case 15:
             horizRadians = 37.0*M_PI/180.0;		break;
    case  2: case 4: case 7: case 9: case 12: case 14:
             horizRadians = 21.0*M_PI/180.0;		break;
    case  3: case 8: case 13: /* horizRadians = 0.0; */	break;
    default: horizRadians = 0.0;
	warn("interocularDist: unknown pose angle set to 0.0");
  }
  horizRadians = width*fracWidth*cos(horizRadians);
  /*wrn("interocularDist: got width:%d, returning %f",width,horizRadians);*/
  return horizRadians;
}

void rexFromEyeLabels(Rexy *rx, List labels)
{
img_label_t *l_eye, *r_eye;

  if (((l_eye = findImgLabel ("l_eye", labels)) == NULL) ||
      ((r_eye = findImgLabel ("r_eye", labels)) == NULL)) {
    warn("rexFromEyeLabels: Couldn't find left and right eyes.");
    return;
  }
  rx->x1 = (flt) l_eye->x;
  rx->y1 = (flt) l_eye->y;
  rx->x2 = (flt) r_eye->x;
  rx->y2 = (flt) r_eye->y;
}



void
canonFaceTrf ( AffTrf *aT, LblFace *dLbl, LblFace *sLbl, int pose
             , int dW, int sW, double fracWidth, double horzRotDeg)
{
dbl 	xx = sLbl->rex - sLbl->lex;
dbl 	yy = sLbl->rey - sLbl->ley;

  dLbl->lex = (flt) aT->xdst = (flt) 0.3333*dW;	/** aT->xdst  = sLbl->lex;	**/
  aT->xsrc  = (flt) sLbl->lex;		/** dLbl->lex - sLbl->lex;	**/
  dLbl->ley = (flt) aT->ydst = (flt) 0.3750*dW;	/** aT->ydst  = sLbl->ley;	**/
  aT->ysrc  = (flt) sLbl->ley;		/** dLbl->ley - sLbl->ley;	**/
  aT->scale = (flt)(0.25/(xx*xx + yy*yy));
  aT->angle = (flt) atan2(sLbl->rey - sLbl->ley, sLbl->rex - sLbl->lex);
  aT->cosa  = (flt) cos(aT->angle);
  aT->sina  = (flt) sin(aT->angle);
}


#if 00
void
affTrfFromTwoPts ( AffTrf *aT, Rexy *twoDstPts, Rexy *twoSrcPts
                 , dbl fracAngle, dbl fracTranslate)
{                                 /** fracTranslate not used **/
dbl     dst_dx, dst_dy, src_dx, src_dy;
dbl     dst_mag, src_mag, dd;

  dst_dx    = twoDstPts->x2 - twoDstPts->x1;
  dst_dy    = twoDstPts->y2 - twoDstPts->y1;
  src_dx    = twoSrcPts->x2 - twoSrcPts->x1;
  src_dy    = twoSrcPts->y2 - twoSrcPts->y1;

  dst_mag   = sqrt(dst_dx*dst_dx + dst_dy*dst_dy);
  src_mag   = sqrt(src_dx*src_dx + src_dy*src_dy);
  aT->scale = src_mag/dst_mag;  /* @@ THIS HAS ALWAYS SEEMED WRONG... */
/*wrn("affTrfFromTwoPts: dst_mag:%8.4f   src_mag:%8.4f", dst_mag, src_mag);*/
  dd        = (dst_dx*src_dx + dst_dy*src_dy)/(dst_mag*src_mag);
  if          (dd >  1.0)  dd =  1.0;
  else if     (dd < -1.0)  dd = -1.0;
  aT->angle = SIGN(dst_dx*src_dy - dst_dy*src_dx)*acos(dd) * fracAngle;
  aT->cos   = cos(aT->angle);
  aT->sin   = sin(aT->angle);
  aT->xdst  = twoDstPts->x1 ;
  aT->ydst  = twoDstPts->y1 ;
  aT->xsrc  = twoSrcPts->x1 ;
  aT->ysrc  = twoSrcPts->y1 ;
/*wrn("affTrfFromTwoPts: fracAngle: %4.2f --> angle: %4.2f",fracAngle,aT->angle);*/
}
#endif


void
deltEyeLocs ( delEyeLoc *del, pRexy eyeRexy, int pose, int dW, int sW
             , double fracWidth, double horzRotDeg)
{
double  interocular;

  if (dW > 0)  sW = dW;
  interocular = interocularDist(pose,sW,fracWidth,horzRotDeg);

  del->olx = eyeRexy->x1;
  del->oly = eyeRexy->y1;
  del->orx = eyeRexy->x2;
  del->ory = eyeRexy->y2;
  del->nlx = (flt)(sW/2.0  -  interocular/2.0);
  del->nrx = (flt)(sW/2.0  +  interocular/2.0);
  del->nly = (flt) del->nry = (flt)((del->oly + del->ory)/2.0);
/****
wrn("deltEyeLocs: old Lxy:  %3.2f  %3.2f  Rxy: %3.2f  %3.2f"
    ,del->olx,del->oly,del->orx,del->ory);
wrn("deltEyeLocs: new Lxy:  %3.2f  %3.2f  Rxy: %3.2f  %3.2f"
    ,del->nlx,del->nly,del->nrx,del->nry);
****/
}


void
delEyeLocs (delEyeLoc *del, List labels, int pose, unt sW)
{ 
double  interocular;
img_label_t *l_eye, *r_eye;

  if (((l_eye = findImgLabel ("l_eye", labels)) == NULL) ||
      ((r_eye = findImgLabel ("r_eye", labels)) == NULL)) {
    wrn("delEyeLocs: Couldn't find left and right eyes.");
    return;
  }

  interocular = idealInterocularDist(pose);

  del->olx = (flt) l_eye->x;
  del->oly = (flt) l_eye->y;
  del->orx = (flt) r_eye->x;
  del->ory = (flt) r_eye->y;
  del->nlx = (flt)(sW/2.0  -  interocular/2.0);
  del->nrx = (flt)(sW/2.0  +  interocular/2.0);
  del->nly = del->nry = (flt)((del->oly + del->ory)/2.0);
/****
wrn("delEyeLocs: old Lxy:  %3.2f  %3.2f  Rxy: %3.2f  %3.2f"
    ,del->olx,del->oly,del->orx,del->ory);
wrn("delEyeLocs: new Lxy:  %3.2f  %3.2f  Rxy: %3.2f  %3.2f"
    ,del->nlx,del->nly,del->nrx,del->nry);
****/
}


void
affTrfXY(dbl *x, dbl *y, AffTrf *aT)
{
  dbl a11 =  aT->cosa;   dbl a12 = -aT->sina;
  dbl a21 =  aT->sina;   dbl a22 =  aT->cosa;
  dbl tx  =  aT->xdst;   dbl ty  =  aT->ydst;
  dbl ux  =  aT->xsrc;   dbl uy  =  aT->ysrc;
  dbl x_org = *x - ux;   dbl y_org = *y - uy;
  *x = a11*x_org + a12*y_org + tx;
  *y = a21*x_org + a22*y_org + ty;
}
/*____________________________________________________________________________*/

/* put eyes at a fixed interocular distance, used for operating recognizer
 * at a reduced resolution.  Image warped with a 2D rotation & scale.
 * image and labels are both modified
 */


void
affTrfRexy ( Rexy *rex, AffTrf *aT, dbl frac)
{
  dbl a11 =  aT->cosa;  dbl a12 = -aT->sina;
  dbl a21 =  aT->sina;  dbl a22 =  aT->cosa;
  dbl tx  =  aT->xdst;  dbl ty  =  aT->ydst;
  dbl ux  =  aT->xsrc;  dbl uy  =  aT->ysrc;
  dbl xo  =  rex->x1 - ux;
  dbl yo  =  rex->y1 - uy;
  rex->x1 = (flt)(a11*xo + a12*yo + tx);
  rex->y1 = (flt)(a21*xo + a22*yo + ty);

  xo      = rex->x2 - ux;
  yo      = rex->y2 - uy;
  rex->x2 = (flt)(a11*xo + a12*yo + tx);
  rex->y2 = (flt)(a21*xo + a22*yo + ty);
}


void
affTrfLbl ( List labels, AffTrf *aT, dbl frac, delEyeLoc *del)
{
img_label_t *img_label;

#if 1
  while (labels) {				/* transform label points */
    img_label = (img_label_t *) L_CAR(labels);
    affTrfXY(&(img_label->x), &(img_label->y), aT);
    labels = L_CDR(labels);
  }
#else
  while (labels) {				/* transform label points */
    img_label = (img_label_t *) L_CAR(labels);
    warpRotScaleLbl (&(img_label->x), &(img_label->y), del);
    labels = L_CDR(labels);
  }
#endif
}




ppu
fixEyeLocRecTrf ( ppu src, delEyeLoc *del, int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, List labels, int pose)
{
img_label_t *img_label;
ppu	dst;

  delEyeLocs(del, labels, pose, sW);

  dst = rowMalUnc(sX,sY,sW,sH);
  warpRotScaleImg(dst,src, dX,dY,dW,dH,sX,sY,sW,sH, del, 1.0, 1.0);
  while (labels) {				/* transform label points */
    img_label = (img_label_t *) L_CAR(labels);
    warpRotScaleLbl (&(img_label->x), &(img_label->y), del);
    labels = L_CDR(labels);
  }
  return dst;
}




#if 0
ppu
fixEyeLocations ( ppu src, int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, List labels, int pose)
{
flt	dfx[2], dfy[2], sfx[2], sfy[2];
double	interocular;
img_label_t *img_label, *l_eye, *r_eye;
ppu	dst;

  if (((l_eye = findImgLabel ("l_eye", labels)) == NULL) ||
      ((r_eye = findImgLabel ("r_eye", labels)) == NULL)) {
    wrn("fixEyeLocations: Couldn't find left and right eyes.");
    return NULL;
  }

  interocular = idealInterocularDist (pose);
  sfx[0] = l_eye->x;		sfy[0] = l_eye->y;
  sfx[1] = r_eye->x;		sfy[1] = r_eye->y;
  dfx[0] = sW/2.0 - interocular/2.0;
  dfx[1] = sW/2.0 + interocular/2.0;
  dfy[0] = dfy[1] = (sfy[0] + sfy[1])/2.0;

  dst = rowMalUnc(      sX,sY,sW,sH);
  warpRotScale(dst,src, dX,dY,dW,dH,sX,sY,sW,sH, dfx,dfy,sfx,sfy);

  while (labels) {				/* transform label points */
    img_label = (img_label_t *) L_CAR(labels);
    transformRotScale (&(img_label->x), &(img_label->y), dfx, dfy, sfx, sfy);
    labels = L_CDR(labels);
  }
  return dst;
}
#endif




double
idealInterocularDist (int pose)
{
  /*wrn("idealInterocularDist: pose=%d",pose);*/
  switch (pose) {
  case 1:
  case 5:
  case 6:
  case 10:
  case 11:
  case 15:
    return 48.0;
  case 2:
  case 4:
  case 7:
  case 9:
  case 12:
  case 14:
    return 56.0;
  case 3:
  case 8:
  case 13:
    return 60.0;
  }
  return 0.0;
}

int
transformRotScale (double *x, double *y, float *dx, float *dy, float *sx, float *sy)
{
double dst_dx, dst_dy, src_dx, src_dy;
double dst_mag, src_mag, scale, angle, cross;
double a11, a12, a21, a22, tx, ty, ux, uy;
double x_org, y_org;

  dst_dx = dx[1] - dx[0];
  dst_dy = dy[1] - dy[0];
  src_dx = sx[1] - sx[0];
  src_dy = sy[1] - sy[0];

  dst_mag = sqrt(dst_dx*dst_dx + dst_dy*dst_dy);
  src_mag = sqrt(src_dx*src_dx + src_dy*src_dy);
  scale = dst_mag/src_mag;
  angle = (dst_dx*src_dx + dst_dy*src_dy)/(dst_mag*src_mag);
  cross = dst_dx*src_dy - dst_dy*src_dx;
  angle = Max(-1.0, angle);
  angle = Min(1.0, angle);
  angle = -Sign(cross)*acos (angle);
  a11   = scale*cos(angle);	a12 = -scale*sin(angle);
  a21   = scale*sin(angle);	a22 =  scale*cos(angle);
  tx = dx[0];			ty = dy[0];
  ux = sx[0];			uy = sy[0];
  x_org = *x - ux;		y_org = *y - uy;
  *x = a11*x_org + a12*y_org + tx;
  *y = a21*x_org + a22*y_org + ty;
return 00;
}


int
warpRotScaleLbl (double *x, double *y, delEyeLoc *del)
{
dbl	dst_dx, dst_dy, src_dx, src_dy;
dbl	dst_mag, src_mag, scale, angle, cross;
dbl	a11, a12, a21, a22, tx, ty, ux, uy;
dbl	x_org, y_org;

  dst_dx = del->nrx - del->nlx;
  dst_dy = del->nry - del->nly;
  src_dx = del->orx - del->olx;
  src_dy = del->ory - del->oly;

  dst_mag = sqrt(dst_dx*dst_dx + dst_dy*dst_dy);
  src_mag = sqrt(src_dx*src_dx + src_dy*src_dy);
  scale = dst_mag/src_mag;
  angle = (dst_dx*src_dx + dst_dy*src_dy)/(dst_mag*src_mag);
  cross = dst_dx*src_dy - dst_dy*src_dx;
  angle = Max(-1.0, angle);
  angle = Min(1.0, angle);
  angle = -Sign(cross)*acos (angle);
  a11   = scale*cos(angle);	a12 = -scale*sin(angle);
  a21   = scale*sin(angle);	a22 = scale*cos(angle);

  tx = del->nlx;		ty = del->nly;
  ux = del->olx;		uy = del->oly;

  x_org = *x - ux;		y_org = *y - uy;
  *x = a11*x_org + a12*y_org + tx;
  *y = a21*x_org + a22*y_org + ty;
return 00;
}


#if 0000

/* warp novel image using an affine transform to bring three points
 * on novel face into alignment with the same three points on model
 */

warp_novel_affine (image_t *warped, image_t *novel, List model_labels, List novel_labels, int pose)
{
  char **label_names;
  int i;
  float dx[3], dy[3], sx[3], sy[3];
  img_label_t *s_img_label, *d_img_label;

  if ((pose-1)%5 < 3)
    label_names = label_names_left;
  else
    label_names = label_names_right;
  for (i = 0; i < 3; i++) {
    if ((s_img_label = findImgLabel (label_names[i], novel_labels)) == NULL) {
      fprintf (stderr, "couldn't find novel label %s\n", label_names[i]);
      exit (-1);
    } else if ((d_img_label = findImgLabel (label_names[i], model_labels)) == NULL) {
      fprintf (stderr, "couldn't find model label %s\n", label_names[i]);
      exit (-1);
    } else {
      sx[i] = (float) s_img_label->x;	sy[i] = (float) s_img_label->y;
      dx[i] = (float) d_img_label->x;	dy[i] = (float) d_img_label->y;
    }
  }
  warp_affine (warped, novel, dx, dy, sx, sy);

return 00;
}

warp_novel_affine2 (image_t *warped, image_t *novel, List model_labels, List novel_labels, int pose, List bboxes)
{
  char **label_names;
  int i;
  float dx[3], dy[3], sx[3], sy[3];
  bbox_t *whole_face;
  img_label_t *s_img_label, *d_img_label;

  if ((pose-1)%5 < 3)
    label_names = label_names_left;
  else
    label_names = label_names_right;
  for (i = 0; i < 3; i++) {
    if ((s_img_label = findImgLabel (label_names[i], novel_labels)) == NULL) {
      fprintf (stderr, "couldn't find novel label %s\n", label_names[i]);
      exit (-1);
    } else if ((d_img_label = findImgLabel (label_names[i], model_labels)) == NULL) {
      fprintf (stderr, "couldn't find model label %s\n", label_names[i]);
      exit (-1);
    } else {
      sx[i] = (float) s_img_label->x;	sy[i] = (float) s_img_label->y;
      dx[i] = (float) d_img_label->x;	dy[i] = (float) d_img_label->y;
    }
  }
  whole_face = whole_face_bbox (bboxes, warped->width, warped->height);
  warp_affine_bbox (warped, novel, dx, dy, sx, sy, whole_face);
  free (whole_face);

return 00;
}


extract_sub_images (image_t **sub_model, image_t **sub_warped, List *sub_bboxes, image_t *model, image_t *warped, List bboxes)
{
  bbox_t *whole_face;

  whole_face = whole_face_bbox (bboxes, model->width, model->height);
  *sub_model = allocate_image (whole_face->width, whole_face->height,
			       IMG_U8BIT);
  *sub_warped = allocate_image (whole_face->width, whole_face->height,
				IMG_U8BIT);
  extract_image (*sub_model, model, whole_face->x, whole_face->y,
		 whole_face->width, whole_face->height);
  extract_image (*sub_warped, warped, whole_face->x, whole_face->y,
		 whole_face->width, whole_face->height);
  *sub_bboxes = offset_bboxes (bboxes, -whole_face->x, -whole_face->y);
  free (whole_face);

return 00;
}

warp_novel_motion (image_t **dx, image_t **dy, image_t *warped, image_t *model, int num_reductions)
{
  image_t *conf, *tmp;

  *dx = allocate_image (model->width, model->height, IMG_FLOAT);
  *dy = allocate_image (model->width, model->height, IMG_FLOAT);
  conf = allocate_image (model->width, model->height, IMG_FLOAT);
  tmp = allocate_image_like (warped);

  mot_lsqconf (*dx, *dy, conf, warped, model,
	       top_level_value (num_reductions), 0);
  mot_warp (tmp, warped, *dx, *dy, -1.0);
  op_image_image (warped, tmp, COPY);

  deallocate_image (conf);
  deallocate_image (tmp);

return 00;
}

top_level_value (int num_reductions)
{
  switch (num_reductions) {
  case 0:
  case 1:
    return 4;
  case 2:
    return 3;
  case 3:
    return 2;
  }
  return 0;
}



/* given an index specifying the pose and labeled image features,
 * return bounding boxes for templates to extract from this image
 */

List
model_template_bboxes (List model_labels, int pose)
{
  List bboxes;

  bboxes = NULL;
  switch (pose) {
  case 1:
    push (make_centered_bbox (model_labels, "l_eye", .9, .7, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .5, .3, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 2:
    push (make_centered_bbox (model_labels, "l_eye", .8, .8, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .6, .4, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 3:
    push (make_centered_bbox (model_labels, "l_eye", .8, .8, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .8, .8, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 4:
    push (make_centered_bbox (model_labels, "l_eye", .4, .6, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .8, .8, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 5:
    push (make_centered_bbox (model_labels, "l_eye", .3, .5, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .7, .9, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 6:
    push (make_centered_bbox (model_labels, "l_eye", .9, .7, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .5, .3, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 7:
    push (make_centered_bbox (model_labels, "l_eye", .8, .8, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .6, .4, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 8:
    push (make_centered_bbox (model_labels, "l_eye", .8, .8, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .8, .8, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 9:
    push (make_centered_bbox (model_labels, "l_eye", .4, .6, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .8, .8, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 10:
    push (make_centered_bbox (model_labels, "l_eye", .3, .5, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .7, .9, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 11:
    push (make_centered_bbox (model_labels, "l_eye", .9, .7, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .5, .3, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 12:
    push (make_centered_bbox (model_labels, "l_eye", .8, .8, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .6, .4, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 13:
    push (make_centered_bbox (model_labels, "l_eye", .8, .8, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .8, .8, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 14:
    push (make_centered_bbox (model_labels, "l_eye", .4, .6, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .8, .8, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  case 15:
    push (make_centered_bbox (model_labels, "l_eye", .3, .5, .6, .4),
	  bboxes);
    push (make_centered_bbox (model_labels, "r_eye", .7, .9, .6, .4),
	  bboxes);
    push (make_nose_bbox (model_labels, .2, .2, .7, .3),
	  bboxes);
    push (make_mouth_bbox (model_labels, .1, .1, .3, .3),
	  bboxes);
    break;
  }
  return list_reverse (bboxes);
}

bbox_t *
whole_face_bbox (List bboxes, int width, int height)
{
  int x_min, y_min, x_max, y_max, horiz_pad, vert_pad;
  bbox_t *bbox;

  x_min = 1000000;	y_min = 1000000;
  x_max = -1000000;	y_max = -1000000;
  while (!endp (bboxes)) {
    bbox = (bbox_t *) car (bboxes);
    x_min = Min (x_min, bbox->x);
    y_min = Min (y_min, bbox->y);
    x_max = Max (x_max, bbox->x + bbox->width - 1);
    y_max = Max (y_max, bbox->y + bbox->height - 1);
    bboxes = cdr (bboxes);
  }
  bbox = (bbox_t *) calloc (1, sizeof (bbox_t));
  horiz_pad = irint (FACE_TEMPLATE_PAD_HORIZ*(x_max - x_min));
  vert_pad = irint (FACE_TEMPLATE_PAD_VERT*(y_max - y_min));
  x_min = Max (x_min - horiz_pad, 0);
  y_min = Max (y_min - vert_pad, 0);
  x_max = Min (x_max + horiz_pad, width-1);
  y_max = Min (y_max + vert_pad, height-1);
  bbox->x = x_min;
  bbox->y = y_min;
  bbox->width = x_max - x_min + 1;
  bbox->height = y_max - y_min + 1;
  return bbox;
}

/* for making eye bboxes
 */

bbox_t
*make_centered_bbox (List labels, char *centered_name, double l, double r, double u, double d)
{
  int min_x, min_y, max_x, max_y;
  double midpt_x, midpt_y, ux, uy, vx, vy;
  bbox_t *bbox;
  img_label_t *centered_label, *l_eye, *r_eye;

  if ((centered_label = findImgLabel (centered_name, labels)) == NULL) {
    fprintf (stderr, "make_centered_bbox:  cannot find %s label.\n",
	     centered_name);
    return NULL;
  } else if ((l_eye = findImgLabel ("l_eye", labels)) == NULL) {
    fprintf (stderr, "make_centered_bbox:  cannot find l_eye label.\n");
    return NULL;
  } else if ((r_eye = findImgLabel ("r_eye", labels)) == NULL) {
    fprintf (stderr, "make_centered_bbox:  cannot find r_eye label.\n");
    return NULL;
  } else {
    midpt_x = (l_eye->x + r_eye->x)/2.0;
    midpt_y = (l_eye->y + r_eye->y)/2.0;
    ux = r_eye->x - midpt_x;
    uy = r_eye->y - midpt_y;
    vx = uy;
    vy = -ux;
    min_x = irint (centered_label->x - l*ux);
    max_x = irint (centered_label->x + r*ux);
    min_y = irint (centered_label->y + u*vy);
    max_y = irint (centered_label->y - d*vy);
    bbox = (bbox_t *) calloc (1, sizeof (bbox_t));
    bbox->x = min_x;
    bbox->y = min_y;
    bbox->width = max_x - min_x + 1;
    bbox->height = max_y - min_y + 1;
    return bbox;
  }
}

bbox_t
*make_nose_bbox (List labels, double l, double r, double u, double d)
{
  int min_x, max_x, min_y, max_y;
  double eye_midpt_x, eye_midpt_y, lb_midpt_x, lb_midpt_y, ux, uy, vx, vy;
  double nose_eye_x, nose_eye_y;
  bbox_t *bbox;
  img_label_t *l_eye, *r_eye, *l_lb, *r_lb;

  if ((l_eye = findImgLabel ("l_eye", labels)) == NULL) {
    fprintf (stderr, "make_nose_bbox:  cannot find l_eye label.\n");
    return NULL;
  } else if ((r_eye = findImgLabel ("r_eye", labels)) == NULL) {
    fprintf (stderr, "make_nose_bbox:  cannot find r_eye label.\n");
    return NULL;
  } else if ((l_lb = findImgLabel ("l_lb", labels)) == NULL) {
    fprintf (stderr, "make_nose_bbox:  cannot find l_lb label.\n");
    return NULL;
  } else if ((r_lb = findImgLabel ("r_lb", labels)) == NULL) {
    fprintf (stderr, "make_nose_bbox:  cannot find r_lb label.\n");
    return NULL;
  } else {
    eye_midpt_x = (l_eye->x + r_eye->x)/2.0;
    eye_midpt_y = (l_eye->y + r_eye->y)/2.0;
    lb_midpt_x = (l_lb->x + r_lb->x)/2.0;
    lb_midpt_y = (l_lb->y + r_lb->y)/2.0;
    nose_eye_x = eye_midpt_x - lb_midpt_x;
    nose_eye_y = eye_midpt_y - lb_midpt_y;
    ux = r_eye->x - eye_midpt_x;
    uy = r_eye->y - eye_midpt_y;
    vx = uy;
    vy = -ux;
    min_x = irint (l_lb->x - l*ux);
    max_x = irint (r_lb->x + r*ux);
    min_y = irint (lb_midpt_y + u*nose_eye_y);
    max_y = irint (lb_midpt_y - d*nose_eye_y);
    bbox = (bbox_t *) calloc (1, sizeof (bbox_t));
    bbox->x = min_x;
    bbox->y = min_y;
    bbox->width = max_x - min_x + 1;
    bbox->height = max_y - min_y + 1;
    return bbox;
  }
}

bbox_t
*make_mouth_bbox (List labels, double l, double r, double u, double d)
{
  int min_x, max_x, min_y, max_y, dx;
  bbox_t *bbox;
  img_label_t *l_mth, *r_mth;

  if ((l_mth = findImgLabel ("l_mth", labels)) == NULL) {
    fprintf (stderr, "make_mouth_bbox:  cannot find l_mth label.\n");
    return NULL;
  } else if ((r_mth = findImgLabel ("r_mth", labels)) == NULL) {
    fprintf (stderr, "make_mouth_bbox:  cannot find r_mth label.\n");
    return NULL;
  } else {
    dx = r_mth->x - l_mth->x;
    min_x = irint (l_mth->x - l*dx);
    max_x = irint (r_mth->x + r*dx);
    min_y = irint (Min(l_mth->y,r_mth->y) - u*dx);
    max_y = irint (Max(l_mth->y,r_mth->y) + d*dx);
    bbox = (bbox_t *) calloc (1, sizeof (bbox_t));
    bbox->x = min_x;
    bbox->y = min_y;
    bbox->width = max_x - min_x + 1;
    bbox->height = max_y - min_y + 1;
    return bbox;
  }
}

List
offset_bboxes (List bboxes, int dx, int dy)
{
  bbox_t *bbox, *new;
  List lst;

  lst = NULL;
  while (!endp (bboxes)) {
    bbox = (bbox_t *) car (bboxes);
    new = (bbox_t *) calloc (1, sizeof (bbox_t));
    new->x = bbox->x + dx;
    new->y = bbox->y + dy;
    new->width = bbox->width;
    new->height = bbox->height;
    push (new, lst);
    bboxes = cdr (bboxes);
  }
  return list_reverse (lst);
}

/************************************************************************
 *
 * correlation routines
 *
 */

double
normalized_correlation_at_point (image_t *magn, image_t *templ_mag, int ul_x, int ul_y, int num_reductions)
{
  int x, y, x_t, y_t, best_x_t, best_y_t, size, horiz_radius, vert_radius;
  float *mag_ptr, *templ_mag_ptr;
  double acc, best_acc, sum_mag, sum_mag_sq;
  double mean_acc, mean_mag, mean_templ, std_dev_mag, std_dev_templ;

  best_acc = -100000.0;
  horiz_radius = horiz_radius_value (num_reductions);
  vert_radius = vert_radius_value (num_reductions);
  image_mean_std_dev (templ_mag, &mean_templ, &std_dev_templ);
  for (y_t = -vert_radius; y_t <= vert_radius; y_t++) {
    for (x_t = -horiz_radius; x_t <= horiz_radius; x_t++) {
      acc = sum_mag = sum_mag_sq = 0.0;
      for (y = 0; y < templ_mag->height; y++) {
	templ_mag_ptr = (float *) templ_mag->image[y];
	mag_ptr = (float *) magn->image[y + ul_y + y_t] + ul_x + x_t;
	for (x = 0; x < templ_mag->width; x++) {
	  acc += (*templ_mag_ptr) * (*mag_ptr);
	  sum_mag += *mag_ptr;
	  sum_mag_sq += (*mag_ptr) * (*mag_ptr);
	  mag_ptr++;	templ_mag_ptr++;
	}
      }
      size = templ_mag->width*templ_mag->height;
      mean_acc = acc/size;
      mean_mag = sum_mag/size;
      std_dev_mag = sqrt(sum_mag_sq/size - mean_mag*mean_mag);
      acc = (mean_acc - mean_mag*mean_templ)/(std_dev_templ*std_dev_mag);
      if (acc > best_acc) {
	best_acc = acc;
	best_x_t = x_t;
	best_y_t = y_t;
      }
    }
  }
/*  fprintf (stderr, "search: trans (%d,%d), best acc = %f\n",
	   best_x_t, best_y_t, best_acc); */
  return best_acc;
}

/* assumes that gradients of both the template and image
 * have been normalized
 */

double
vector_correlation_at_point (gradient_t *reduced_grad, gradient_t *templ_grad, int ul_x, int ul_y)
{
  int x, y, x_t, y_t, best_x_t, best_y_t, count, horiz_radius, vert_radius;
  float *templ_dx_ptr, *templ_dy_ptr, *reduced_dx_ptr, *reduced_dy_ptr;
  double acc, best_acc;
  image_t *templ_dx, *templ_dy, *reduced_dx, *reduced_dy;

  templ_dx = templ_grad->dx;
  templ_dy = templ_grad->dy;
  reduced_dx = reduced_grad->dx;
  reduced_dy = reduced_grad->dy;
  horiz_radius = HORIZ_RADIUS;
  vert_radius = VERT_RADIUS;
  best_acc = -100000.0;
  for (y_t = -vert_radius; y_t <= vert_radius; y_t++) {
    for (x_t = -horiz_radius; x_t <= horiz_radius; x_t++) {
      acc = 0.0;
      count = 0;
      for (y = 0; y < templ_dx->height; y++) {
	templ_dx_ptr = (float *) templ_dx->image[y];
	templ_dy_ptr = (float *) templ_dy->image[y];
	reduced_dx_ptr = (float *) reduced_dx->image[y + ul_y + y_t]
	  + ul_x + x_t;
	reduced_dy_ptr = (float *) reduced_dy->image[y + ul_y + y_t]
	  + ul_x + x_t;
	for (x = 0; x < templ_dx->width; x++) {
	  acc += (*templ_dx_ptr++)*(*reduced_dx_ptr++)
	    + (*templ_dy_ptr++)*(*reduced_dy_ptr++);
	  count++;
	}
      }
      acc /= count;
      if (acc > best_acc) {
	best_acc = acc;
	best_x_t = x_t;
	best_y_t = y_t;
      }
    }
  }
/*  fprintf (stderr, "search: trans (%d,%d), best acc = %f\n",
	   best_x_t, best_y_t, best_acc); */
  return best_acc;
}

image_mean_std_dev (image_t *image, double *mean, double *std_dev)
{
  int x, y, size;
  float *ptr;
  double sum, sum_sq;

  sum = sum_sq = 0.0;
  for (y = 0; y < image->height; y++) {
    ptr = (float *) image->image[y];
    for (x = 0; x < image->width; x++) {
      sum_sq += (*ptr)*(*ptr);
      sum += *ptr++;
    }
  }
  size = image->width*image->height;
  *mean = sum/size;
  *std_dev = sqrt (sum_sq/size - (*mean)*(*mean));
return 00;
}

horiz_radius_value (int num_reductions)
{
  switch (num_reductions) {
  case 0:
  case 1:
    return 3;
  case 2:
    return 2;
  case 3:
    return 1;
  }
  return 0;
}

vert_radius_value (int num_reductions)
{
  switch (num_reductions) {
  case 0:
  case 1:
    return 3;
  case 2:
    return 2;
  case 3:
    return 1;
  }
  return 0;
}

/****************************************************************************
 *
 * distortion in a motion field
 *
 */

/* first try:  at each pixel, find maximum angle with eight-connected
 * neighbors, average over template
 *
 * doesn't seem to work well, 2/19/93
 */
eval_distortion1 (List bboxes, image_t *dx, image_t *dy)
{
  int x, y, count;
  float *xptr1, *xptr2, *xptr3, *yptr1, *yptr2, *yptr3;
  double ang, big, acc;
  bbox_t *bbox;

  printf ("distortion1: ");
  while (!endp (bboxes)) {
    bbox = (bbox_t *) car (bboxes);
    acc = 0.0;
    count = 0;
    for (y = bbox->y; y < bbox->y + bbox->height; y++) {
      xptr1 = (float *) dx->image[y-1] + bbox->x;
      xptr2 = (float *) dx->image[y] + bbox->x;
      xptr3 = (float *) dx->image[y+1] + bbox->x;
      yptr1 = (float *) dy->image[y-1] + bbox->x;
      yptr2 = (float *) dy->image[y] + bbox->x;
      yptr3 = (float *) dy->image[y+1] + bbox->x;
      for (x = bbox->x; x < bbox->x + bbox->width; x++) {
	ang = angle_between ((double) *xptr2, (double) *yptr2,
			     (double) *xptr1, (double) *yptr1);
	big = ang;
	ang = angle_between ((double) *xptr2, (double) *yptr2,
			     (double) *(xptr1-1), (double) *(yptr1-1));
	big = Max (big, ang);
	ang = angle_between ((double) *xptr2, (double) *yptr2,
			     (double) *(xptr1+1), (double) *(yptr1+1));
	big = Max (big, ang);
	ang = angle_between ((double) *xptr2, (double) *yptr2,
			     (double) *(xptr2-1), (double) *(yptr2-1));
	big = Max (big, ang);
	ang = angle_between ((double) *xptr2, (double) *yptr2,
			     (double) *(xptr2+1), (double) *(yptr2+1));
	big = Max (big, ang);
	ang = angle_between ((double) *xptr2, (double) *yptr2,
			     (double) *(xptr3-1), (double) *(yptr3-1));
	big = Max (big, ang);
	ang = angle_between ((double) *xptr2, (double) *yptr2,
			     (double) *xptr3, (double) *yptr3);
	big = Max (big, ang);
	ang = angle_between ((double) *xptr2, (double) *yptr2,
			     (double) *(xptr3+1), (double) *(yptr3+1));
	big = Max (big, ang);
	xptr1++;	xptr2++;	xptr3++;
	yptr1++;	yptr2++;	yptr3++;
	acc += big;
	count++;
      }
    }
    acc /= count;
    printf ("\t%4.3f", acc);
    bboxes = cdr (bboxes);
  }
  printf ("\n");
return 00;
}

/* second try: average magnitude of vector flow field
 *
 * works OK, 2/19/93
 */
eval_distortion2 (List bboxes, image_t *dx, image_t *dy)
{
  int x, y, count;
  float *xptr, *yptr;
  double m, acc;
  bbox_t *bbox;

  printf ("distortion2: ");
  while (!endp (bboxes)) {
    bbox = (bbox_t *) car (bboxes);
    acc = 0.0;
    count = 0;
    for (y = bbox->y; y < bbox->y + bbox->height; y++) {
      xptr = (float *) dx->image[y] + bbox->x;
      yptr = (float *) dy->image[y] + bbox->x;
      for (x = bbox->x; x < bbox->x + bbox->width; x++) {
	m = sqrt((double)*xptr**xptr + (double) *yptr**yptr);
	xptr++;		yptr++;
	acc += m;
	count++;
      }
    }
    acc /= count;
    printf ("\t%4.3f", acc);
    bboxes = cdr (bboxes);
  }
  printf ("\n");
return 00;
}

/* same computation as eval_distortion2, but only on one bbox
 */

double
warp_distortion (image_t *dx, image_t *dy, bbox_t *bbox)
{
  int x, y, count;
  float *xptr, *yptr;
  double acc, m;

  acc = 0.0;
  count = 0;
  for (y = bbox->y; y < bbox->y + bbox->height; y++) {
    xptr = (float *) dx->image[y] + bbox->x;
    yptr = (float *) dy->image[y] + bbox->x;
    for (x = bbox->x; x < bbox->x + bbox->width; x++) {
      m = sqrt((double)*xptr**xptr + (double)*yptr**yptr);
      xptr++;		yptr++;
      acc += m;
      count++;
    }
  }
  acc /= count;
  return acc;
}

/*************************************************************************
 *
 * I/O routines
 *
 */
int
read_image_and_labels (image_t **image, List *labels, char *filename)
{
  static char pathname[256];

  if ((*image = read_image (filename)) == NULL) {
    fprintf (stderr, "read_image_and_labels: bad read_image(%s)\n", filename);
    return FALSE;
  }
#if 1
  sprintf (pathname, "%s.labels", filename);
#else
  sprintf (pathname, "%s/%s.labels", read_image_dir(), filename);
#endif
  if ((*labels = read_img_labels (pathname)) == NULL) {
    fprintf (stderr, "read_image_and_labels: bad read_img_labels(%s)\n", pathname);
    return FALSE;
  }
  return TRUE;
}




reduce_image_and_labels (image_t **image, List labels, int num_reductions)
{
  int i, width, height;
  img_label_t *img_label;
  image_t *last_reduced, *reduced;
  List lst;

  last_reduced = *image;
  for (i = 0; i < num_reductions; i++) {
    width = irint (ceil (last_reduced->width/2.0));
    height = irint (ceil (last_reduced->height/2.0));
    reduced = allocate_image (width, height, IMG_U8BIT);
    reduce_image (reduced, last_reduced, 0.4, BOUNDARY_REFLECT);
    deallocate_image (last_reduced);
    last_reduced = reduced;
    lst = labels;
    while (!endp (lst)) {
      img_label = (img_label_t *) car (lst);
      img_label->x /= 2.0;
      img_label->y /= 2.0;
      lst = cdr (lst);
    }
  }
  *image = last_reduced;
return 00;
}

change_slash_to_period (char *str)
{
  char *num_str;

  if ((num_str = strstr (str, "/")) != NULL) {
    *num_str = '.';
  }
return 00;
}

#endif	/* 0000 */

