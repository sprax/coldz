/*
 * $Id: global.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
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
 * Global varaibles
 */

#ifndef BIT_GLOBAL_H
#define BIT_GLOBAL_H

/*******************************************************************
 * img_io specifies how to identify an image format by signature or
 * filename extension. other fields specifies how to read or write the
 * image format in question.
 *
 * Special Note: if the order is changed, remember to change the order
 * in tc.c too. Images only identifiable by the extension should appear
 * after all images that have a signature.
 *
 * Raster direction specifies the direction the load routines handed
 * over to the driver routine, NOT the direction specified in the
 * format itself.
 *******************************************************************/

/* some compilers bitch about the signedness of the signature */

#define UC_ (unsigned char *)

/* some short cuts */
#define Null_next         0
#define Null_load         0
#define Null_dump         0
#define Null_get_option   0
#define Null_set_option   0
#define Null_special      0
#define No_text           0
#define Yes_text          1
#define Built_in_display  0
#define Top_to_bottom     1
#define Bottom_to_top     0

IMG_IO img_io[] =
{
 /* IRIS RGB format. Always built */

    {T_RGBA,			/* type                            */
     0,				/* no. of colors. does not apply   */
     {
	 {2, UC_ "\001\332"},	/* signatures                      */
	 {2, UC_ "\332\001"},	/* signatures with byte swapped    */
     },
     "IRIS RGB Format",		/* Formal name                     */
     "IRIS",			/* short name                      */
     "rgb",			/* extension name                  */
     RGB_desc,			/* get image size, pixel size info */
     RGB_load,			/* load raster                     */
     RGB_dump,			/* routine to write the image      */
     Null_next,			/* load next one                   */
     Null_get_option,		/* get write parameters            */
     Null_set_option,		/* function to call before write   */
     Built_in_display,		/* display function                */
     Null_special,		/* special for this format         */
     Bottom_to_top,		/* raster directions               */
     No_text			/* if do text&sgf info by driver   */
    }

#ifndef  NO_PNM

 /**** PNM includes PPM, PGM and PBM ***/
    ,
    {T_RGBA,			/* ppm */
     0,
     {
	 {2, UC_ "P6"},
	 {2, UC_ "P3"}
     },
     "PPM Format", "PPM", "ppm",
     PNM_desc,
     PPM_load,
     PPM_dump,
     Null_next,
     PPM_wdefault,
     PPMdump_init,
     Built_in_display,
     Null_special,
     Bottom_to_top,
     Yes_text			/* capable of text */
    }
    ,

    {T_GRAY,			/* pgm */
     0,
     {
	 {2, UC_ "P5"},
	 {2, UC_ "P2"}
     },
     "PGM Format",
     "PGM", "pgm",
     PNM_desc,
     PPM_load,
     PPM_dump,
     Null_next,
     PPM_wdefault,
     PPMdump_init,
     Built_in_display,
     Null_special,
     Bottom_to_top,
     Yes_text
    }

    ,
    {T_BW,			/* pbm */
     2,
     {
	 {2, UC_ "P4",},
	 {2, UC_ "P1"}
     },
     "PBM format", "PBM", "pbm",
     PNM_desc,
     PBM_load,
     PBM_dump,
     Null_next,
     PPM_wdefault,
     PPMdump_init,
     Built_in_display,
     bw_special,
     Bottom_to_top,
     Yes_text
    }

#endif


#ifndef NO_GIF
 /******* CompuServ GIF *******/
    ,
    {T_CMAP,
     256,
     {
	 {6, UC_ "GIF87a"},
	 {6, UC_ "GIF89a"}
     },
     "CompuServ GIF", "GIF", "gif",
     GIF_desc,
     GIF_load,
     GIF_dump,
     Null_next,
     GIF_wdefault,
     GIFdump_init,
     Built_in_display,
     Null_special,
     Top_to_bottom,
     No_text
    }
#endif


#ifndef NO_JPEG

 /***** JPEG from independent JPEG group ******/

 /* 255, 216, 255, 224, 0, 16, JFIF                */
    ,
    {T_RGBA,
     0,
     {
	 {10, UC_ "\377\330\377\340\000\020JFIF"},
	 {2, UC_ "\377\330"}
     },
     "JFIF format", "JPEG", "jpg",
     JPEG1_desc,
     JPEG1_load,
     JPEG1_dump,
     Null_next,
     JPEG1_wdefault,
     JPEG1dump_init,
     Built_in_display,
     Null_special,
     Bottom_to_top,
     No_text
    }
#endif

#ifndef NO_PS

 /********* PostScript ****************/
    ,
    {T_RGBA,
     0,
     {
	 {10, UC_ "%!PS-Adobe"}
     },
     "PostScript", "PS", "ps",
     PS_desc,
     Null_load,
     PS_dump,
     Null_next,
     PS_wdefault,
     PSdump_init,
     Built_in_display,
     Null_special,
     Bottom_to_top,
     No_text			/* will be taken care of in dump */
    }
#endif

#ifndef NO_XBM

 /******* X11 bitmaps **********/
    ,
    {T_BW,
     2,
     {
	 {4, UC_ "#def"}
     },
     "X Window Bitmap", "XBM", "xbm",
     XBM_desc,
     XBM_load,
     XBM_dump,
     Null_next,
     Null_get_option,
     Null_set_option,
     Built_in_display,
     bw_special,
     Bottom_to_top,
     No_text
    }
#endif

#ifndef NO_XPS

 /********* Electron Hologram ******************/
    ,
    {T_CMAP,
     256,
     {
	 {4, UC_ "DSAA"},
	 {4, UC_ "DSBB"}
     },
     "XPS Hologram", "XPS", "xps",
     HOLO_desc,
     HOLO_load,
     HOLO_dump,
     Null_next,
     HOLO_wdefault,
     HOLOdump_init,
     Built_in_display,
     HOLO_special,
     Bottom_to_top,
     Yes_text
    }
#endif

#ifdef ADD_TC

 /******** My own stuff ************/
    ,
    {T_FLEX,
     -1,
     {
	 {6, UC_ "TCZ1.0"}
     },
     "UniFormat",
     "UniF",
     "dat",
     TC_desc,
     TC_load,
     Null_dump,
     Null_next,
     Null_set_option,
     Null_get_option,
     Null_special,
     Bottom_to_top,
     No_text
    }
#endif

#define NO_MPEG

#ifndef NO_MPEG

 /******* MPEG ***********/
    ,
    {T_RGBA,
     0,
     {
	 {4, UC_ "\000\000\001\263"}
     }
     "MPEG",
     "MPEG",
     "mpg",
     MPEG_desc,
     MPEG_load,
     Null_dump,
     Null_get_option,
     Null_set_option,
     Built_in_display,
     Null_special,
     Bottom_to_top,
     No_text
    }
#endif /* MPEG */


/********* Scratch files. Always defined *********/
    ,
    {T_FLEX,
     -1,
     {
	 {6, UC_ "BIT1.0"}
     },
     "Scratch File", "TMP", "tmp",
     TMP_desc,
     TMP_load,
     TMP_dump,
     Null_next,
     TMP_wdefault,
     TMPdump_init,
     Built_in_display,
     Null_special,
     Bottom_to_top,
     Yes_text
    }

#ifndef NO_SUNRAS

 /****** Sun rasterfile *************/
    ,
    {T_FLEX,
     -1,
     {
	 {4, UC_ "\131\246\152\225"}
     },
     "Sun Rasterfile", "RAS", "ras",
     RAS_desc,
     RAS_load,
     RAS_dump,
     Null_next,
     Null_get_option,
     Null_set_option,
     Built_in_display,
     Null_special,
     Bottom_to_top,
     Yes_text
    }
#endif

#ifndef NO_COMPRESS

 /***** compressed file hack *************/
    ,
    {T_FLEX,
     -1,
     {
	 {2, UC_ "\037\235"}
     },
     "Compressed ", "CMPRS", "Z",
     Compress_desc,
     Compress_load,
     Null_dump,
     Null_next,
     Null_get_option,
     Null_set_option,
     Built_in_display,
     Null_special,
     0,
     No_text
    }
#endif

#ifndef NO_TIFF

 /******** TIFF files. **************/
    ,
    {T_FLEX,
     -1,
     {
	 {2, UC_ "MM"},
	 {2, UC_ "II"}
     },
     "TIFF File", "TIFF", "tif",
     TIFF_desc,
     TIFF_load,
     TIFF_dump,
     Null_next,
     TIFF_wdefault,
     TIFF_dumpinit,
     0,
     0,
     0,
     No_text
    }
#endif
};

IPTR imgptr;
int totalfmt = sizeof(img_io) / sizeof(img_io[0]);
rgba_t background_color = 0x008f8f8f;

/********************************************************
 * main windows id's and dimensions
 ********************************************************/

long win_id = -1;
long win_w, win_h;
long win_xo, win_yo;

/***********************************************************
 * Various flags that control the subtle things in the program
 *************************************************************/

int double_buf;			/* if double buffer. Unused currently */
int keepmisc;			/* keep text and sgf across images    */
int always_clear = 1;		/* refresh screen between each display */
int always_border;		/* force a border on forms            */
int report_mouse;		/* report mouse locations             */
int preserve_wm_colors = 1;	/* keep black & white                 */
int crop_fill = 1;		/* if fill crop region larger than im */
int always_pan = 1;		/* if auto pan: 1 for slidesonly      */
int always_delete;		/* if deletion w/o confirmation       */
int always_readscrn;		/* Cut buffer source                  */
int auto_remove = 1;		/* if remove bad files                */
int auto_ext = 1;		/* if add extension on write          */
int auto_position;		/* if ignore postion file             */
int halftone_method;		/* B&W dithering method               */
int quant_method;		/* qunatization method                */
int display_style;		/* special effects in display         */
int report_level = 3;		/* index into percentage arrays       */
int percent_report;		/* the percentage                     */
int verbose = 1;		/* warning and error only             */
int pezx = 15;			/* pixedit magnification              */
int pezy = 15;			/* pixedit magnification              */
int slideshow;			/* pause between frames               */
int slide_pause_index;		/* index into pause time              */
int showedit;			/* if show editing panel automatically */
int full_win;			/* if startup full window             */
int no_panel;			/* no initial control panels          */
int abort_change_ask;		/* if silently abort changes          */
int show_cut_buffer = 1;	/* draw cut buffers                   */
float g_zoomx = 1.0, g_zoomy = 1.0;
int simu_dbl_buffer;		/* if simulate double buffering        */
int simu_op_dblbuf;		/* if simulate dblbuf in overlay plane */

int form_single;		/* undocumented */

const char *hexdigits = "0123456789abcdef";

/* compiler won't merge strings, here we save some space */

const char *mfailed = "malloc failed";
const char *bugquit = "Internal error";
const char *outbound = "Out of bounds";

#endif /* BIT_GLOBAL_H */

