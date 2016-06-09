/*
 * $Id: utype.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
 *
 *. This file is part of BIT package.
 *
 *
 *  Copyright(c) 1993 by T.C. Zhao.  All rights reserved.
 *
 *  Permission to do anything with this software non-commercial and
 *  without fee is hereby granted, provided that the copyright notice
 *  and disclaimer appear in all copies and their documentation.
 *
 *  This software is provided "as is" without expressed or implied
 *  warranty of any kind.
 *.
 *
 * typedefs, enums and macros. This file should be a basic one, i.e., it
 * should not dependent on any other non-system headers, except errors.
 */

#ifndef BIT_UTYPE_H
#define BIT_UTYPE_H

#include "ulib.h"

#ifndef PROTECT
#ifdef  ADD_TC
#undef  ADD_TC
#endif
#endif

/*-----------------------------------------------
 * Basic types used
 *-----------------------------------------------*/
#ifndef type_pc_t
#define type_pc_t

/* raster types */
typedef unsigned char pc_t;	/* primary color type  */
typedef unsigned short ci_t;	/* colorindex type     */
typedef unsigned long rgba_t;	/* rgba type           */

/* derived types */
typedef rgba_t gray_t;		/* gray scale type     */
typedef ci_t mapg_t;		/* graymap             */
typedef ci_t bw_t;		/* black & white       */

#endif /* !def type_pc_t */

/*-------------------------------------------------------------------
 * Image types. For format capable of holding more than one type of images
 * assign type T_FLEX to it so that conversion routine will pass the image
 * un-altered to the output routine.
 *
 * all grayscale images can be represented by color maps, and subpixel sampling
 * becomes interpolation of the color indexes. The existence of T_GRAY is a
 * bug.
 *
 * NOTE: after description, type must NOT be T_FLEX.
 *--------------------------------------------------------------------*/
typedef enum
  {
      T_RGBA,			/* packed RGBA values            */
      T_GRAY,			/* packed RGBA grayscale         */
      T_CMAP,			/* mapped images having ci_t type */
      T_GMAP,			/* gray scale mapped images       */
      T_BW,			/* Black & White images          */
      T_RGB,			/* Internal type  (unused)       */
      T_FLEX			/* flexible(unknown) types       */
  } IMG_TYPE;

/*
 * macros to check the types. B&W image is currently implemented as a
 * colormapped image with 2 entries in the colormap
 */
#define IS_RGBA(i)     (i && i->type == T_RGBA)
#define IS_MGRAY(i)    (i && i->type==T_GMAP)
#define IS_BW(i)       (i && i->type==T_BW)
#define IS_CI(i)       (i && (i->type==T_CMAP || IS_MGRAY(i) || IS_BW(i)))
#define IS_RGBGRAY(i)  (i && i->type==T_GRAY)
#define IS_CPACK(i)    (IS_RGBA(i) || IS_RGBGRAY(i))
#define IS_GRAY(i)     (IS_MGRAY(i) || IS_RGBGRAY(i))

/*------------------------------------------------------------
 * Colormaps: Implemented as straight RGB arrays. Typical hardware will
 * support 12bits (4096), but it is not likely any map-based image
 * will use that many. default to 9 for BIT. Less than 9 might not work.
 *--------------------------------------------------------------*/

#include <limits.h>
#ifndef CMAPBITS
#define CMAPBITS 9
#endif

/*
 * Note is PCBITS is not 8, i.e., sizeof(pc_t) != 1, PostScript support need
 * to change
 */

/* MAXCML-1 MUST be full bits, as it is used as masks sometimes */
#define MAXCML (1 << CMAPBITS)	/* max. length of cmap */
#define PCBITS (sizeof(pc_t)*CHAR_BIT)	/* primary color bits  */
#define PCMAX  (1 << PCBITS)	/* and its max value + 1   */
#define PCMAXV (PCMAX-1)	/* max value   */

#define BKINDEX (MAXCML-1)	/* bkgrd index if colormap */

/* normalized color is from 0.0 to 1.0 */
#define C2NC(c)      ((1.0/PCMAXV) * (c))
#define NC2C(c)      (int)((c) * PCMAXV + 0.4)

/* Histograms + peak & ave  & value where peak occured   */
typedef unsigned Hist_t[PCMAX + 3];
typedef struct
  {
      int colors;		/* no. of colors in map        */
      int ucolors;		/* no. of uniq. colors         */
      short packed;		/* if packed >0, if histo <0   */
      short sorted;		/* if sorted                   */
      union
	{			/* packed  cmap or histogram   */
	    rgba_t rgba[MAXCML];/* packed  RGB                 */
	    Hist_t hist[4];	/* for RGB & gray histogram    */
	}
      p_h;
      pc_t ct[4][MAXCML];	/* RGB triplets                */
  }
C_MAP, *CMPTR;

/************************************************************
 * conversion between rgb and rgba
 ************************************************************/
#define Pack3(r,g,b)  ((rgba_t)(r)|((rgba_t)(g)<<8)|((rgba_t)(b)<<16))
#define Pack4(r,g,b,a) (Pack3(r,g,b) | (rgba_t)(a) << 24)
#define Unpack(c,r,g,b)                  \
           do {                             \
             r = (  (c)        & PCMAXV);\
             g = (( (c) >> 8 ) & PCMAXV);\
             b = (( (c) >> 16) & PCMAXV);\
           } while (ZERO)

#define Pack         Pack3
#define get_R(a)    ( (a)         & PCMAXV)
#define get_G(a)    ( ((a) >> 8)  & PCMAXV)
#define get_B(a)    ( ((a) >> 16) & PCMAXV)

#define CPACK2GRAY(c) RGB2GRAY(get_R(c), get_G(c), get_B(c))

#define RGB2CPACK Pack
#define CPACK2RGB Unpack

/*------------------------------------------------------------------
 * possible operations on the images. Note that it is not possible to
 * operate on a colormap based image without first converting it to
 * RGB format
 *-----------------------------------------------------------------*/
typedef enum
  {
      O_NONE, O_MASK, O_ADD, O_XOR, O_OR, O_NEG
  } Matop_t;



/*----------------------------------------------------------
 * help index. does not need to be sorted or in sequence
 *---------------------------------------------------------*/
typedef enum
  {
      HELP_CNTL,		/* control panel      */
      HELP_COLEDIT,		/* 1 to 1             */
      HELP_COLOR,		/* color selection    */
      HELP_CP,			/* cut & paste        */
      HELP_CROP,		/* crop               */
      HELP_EDIT,		/* edit control panel */
      HELP_FSELECT,		/* file selector      */
      HELP_INFO,		/* program info       */
      HELP_LOAD,		/* load/remover       */
      HELP_LSCAN,		/* linear scan        */
      HELP_OPTION,		/* set options        */
      HELP_ROTATE,		/* raster rotate      */
      HELP_SCALE,		/* raster scale       */
      HELP_TEXT,		/* text               */
      HELP_WRITE,		/* write to disk      */
      HELP_ZOOM,		/* magnification      */
      HELP_QUANT,		/* qunatization       */
      HELP_MARKING,		/* sgfs               */
      HELP_ERASER,		/* simple cut&paste   */
      HELP_PS,			/* PostScript help    */
      HELP_PEDIT,		/* pixel edit help    */
      HELP_FILTER,		/* external filters   */
      HELP_CONVOLV,		/* extern convolution */
      HELP_HIST,		/* histogram          */
      HELP_JPEG,		/* jpeg help          */
      HELP_SMOOTH,		/* smoothing          */
      HELP_MERGE,		/* merge two images   */
      HELP_IBR,			/* image browser      */
      HELP_TEST			/* temporary          */
  } helpi_t;

/*--------------------------------------------------------------
 * function pointers to binding functions, addto_action etc.
 *-------------------------------------------------------------*/
typedef void (*VLfptr) (long);
typedef void (*VVfptr) (void);
typedef void (*VIIpfptr) (int, int *);
typedef void (*Grepaint) (int /* event_redraw */ );
typedef long (*GLQevent) (long /* dev */ , short /* val */ );
typedef long (*GLQhandler) (long /* dev */ , short /* val */ );

/****************************************************************
 * Pixel representation independent rectwrite
 ****************************************************************/

#define PI_rectwrite(xi, yi, xf, yf, a)                  \
	   if(IS_CPACK(imgptr))                          \
	     lrectwrite(xi, yi, xf, yf, (rgba_t *)(a));  \
           else                                          \
	      rectwrite(xi, yi, xf, yf, (ci_t *)(a))

#define PI_rectread(xi, yi, xf, yf, a)                  \
	   (IS_CPACK(imgptr) ?                          \
	     lrectread(xi, yi, xf, yf, (rgba_t *)(a)):  \
	     rectread(xi, yi, xf, yf, (ci_t *)(a)))

/*---------------------------------------------------------*
 * rubber bands
 *---------------------------------------------------------*/
typedef enum
  {
      RB_RECT, RB_CIRC, RB_LINE, RB_POLY
  } rubber_obj;

#endif /* !def RT_UTYPE_H */

