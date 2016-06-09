/*
 * $Id: bit.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
 *
 * This file is part of BIT package.
 *
 *
 * Copyright(c) 1993 by T.C. Zhao.  All rights reserved.
 *
 * Permission to do anything with this software non-commercial and
 * without fee is hereby granted, provided that the copyright notice
 * and disclaimer appear in all copies and their documentation.
 *
 * This software is provided "as is" without expressed or implied
 * warranty of any kind.
 *
 * includes, limits, defines, and macros
 */
#ifndef BIT_H_
#define BIT_H_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <limits.h>
#include "unistd.h"
#include "gl/gl.h"
#include "gl/device.h"
#include "utype.h"
#include "ulib.h"

/*-----------------------------------------------------*
 * Some limits
 *-----------------------------------------------------*/
#ifndef PATH_MAX
#define MAXDLEN    1024		/* directory name length         */
#else
#define MAXDLEN    PATH_MAX	/* max directory name length     */
#endif

#define MAXSIGL     12		/* max signature length          */
#define MAXISIG      4		/* max sig. one image can have   */
#define MAXINFO     32		/* max misc. info length         */
#define MAXFLEN     128		/* file name without directory   */

#ifndef MAXMEMK
#define MAXMEMK     3000000L	/* max mem for extra copy mem    */
#endif

/*-----------------------------------------------------*
 * vital function prototypes
 *-----------------------------------------------------*/

typedef struct tc_image *IPTR;
typedef int (*I_IO) (IPTR);	/* get info, load, dump */
typedef void (*I_disp) (IPTR, int, int);	/* display image        */
typedef void (*I_sp) (IPTR);	/* special              */
typedef const char *(*I_wdef) (const IPTR);	/* show dump parameter  */
typedef int (*I_next) (IPTR, int);	/* get next image */

/*-----------------------------------------------------*
 * image structures
 *------------------------------------------------------*/

/* important to use unsigned char for signatures. Also the
   length is necessary because of non-ascii signatures
 */
typedef struct
  {
      int len;			/* signature length     */
      const unsigned char *sig;	/* the signature itself */
  }
SIG;

/*
 * structure that completely specifies raster identification and its IO
 * operations. If ncols is negative, the colormap may have arbitrary no. of
 * entries in it.  Of course, in reality, it can't have more than 16bits or
 * whatever the ci_t type can hold, also subject to limits set by
 * CMAPBITS.
 */
typedef struct
  {
      IMG_TYPE type;		/* image type, RGB or cmap */
      int ncols;		/* max. no colors for cmap */
      SIG sig[MAXISIG];		/* signatures              */
      const char *info;		/* Full description        */
      const char *key;		/* A short description     */
      const char *ext;		/* image file extensions   */
      I_IO desc;		/* get image descriptions  */
      I_IO load;		/* read it                 */
      I_IO dump;		/* write it                */
      I_next inext;		/* get next image          */
      I_wdef wdef;		/* get the write settings  */
      I_IO dumpi;		/* set parameters          */
      I_disp display;		/* display it              */
      I_sp special;		/* special stuff           */
      short t2b;		/* raster direction        */
      short textsgf;		/* if capable of text &sgf */
  }
IMG_IO;

#define TC_FL MAXFLEN
/*
 * Structure that holds the current image, including screen locations and
 * other misc. info.
 */
typedef struct tc_image
  {
      IMG_TYPE type;		/* RGB or CMAP               */
      IMG_IO *io;		/* handles to IO routines    */
      const char *key, *info;	/* same as in io             */
      void *raster;		/* raster stream             */
      void *mraster;		/* raster stream handle      */
      FILE *fp;			/* current stream            */
      pc_t **pc[3];		/* the RGB matrix           */
      int w, h;			/* dimension of the image    */
      int colors;		/* no. of colors, 0 for RGB  */
      unsigned long size;	/* raster size in bytes     */
      CMPTR cmap;		/* 0 if RGB                  */
      int xi, yi;		/* image's screen position   */
      int xf, yf;		/* image's screen position   */
      short esize;		/* pixel size                */
      short xoff1;		/* horizontal offscreen      */
      short xoff2;		/* horizontal offscreen      */
      short yoff1;		/* vertical invisible part   */
      short yoff2;		/* vertical invisible part   */
      short lsx, lsy;		/* logical screens (unused)  */
      short t2b;		/* raster direction          */
      short interlace;		/* interlace                 */
      short ok;			/* signals raster ok         */
      short aspect;		/* aspect ratio X 1000       */
      short gamma;		/* gamma  X 100 (unused)     */
      short more;		/* number of images          */
      char ifile[TC_FL];	/* input filename            */
      char ofile[TC_FL];	/* output filename           */
      char misc[MAXINFO];	/* other info                */
  }
TC_IMAGE;

/* functions to handle extern window manager redraw events */
typedef int (*WMhandler) (IPTR, int);

/* functions to handle redraw of other windows, MultiWindows */
typedef int (*MWhandler) (IPTR, int);

#define get_RM(im)  (im->pc[0])
#define get_GM(im)  (im->pc[1])
#define get_BM(im)  (im->pc[2])

#include "gui.h"
#include "mac.h"

/* all prototypes */
#include "proto.h"

/******************
#include <assert.h>
#define winset(win)	{static tc; winset(win); if(++tc==8) assert(1==0);}
*******************/

#endif

