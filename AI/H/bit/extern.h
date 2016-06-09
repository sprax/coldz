/*
 * $Id: extern.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
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
 * Externals mostly defined in global.h
 *
 */
#ifndef BIT_EXTERN_H
#define BIT_EXTERN_H
/*
 * how to identify images
 */
extern IMG_IO img_io[];
extern int totalfmt;
extern IPTR imgptr;
extern unsigned long background_color;
extern long win_id;

/*
 * window size
 */
extern long win_w, win_h;
extern long win_xo, win_yo;
extern int double_buf;

/* Version info */
extern const char *p_version[];
extern const char *sver;

/*
 * various flags
 */
extern int report_level;
extern int percent_report;
extern int always_pan;
extern int always_delete;
extern int always_readscrn;
extern int preserve_wm_colors;
extern int always_border;
extern int always_clear;
extern int report_mouse;
extern int auto_ext;
extern int auto_position;
extern int crop_fill;
extern int auto_remove;
extern int form_single;
extern int halftone_method;
extern int quant_method;
extern int showedit;
extern float g_zoomx, g_zoomy;
extern int display_style;
extern int over_pup_colors;
extern int verbose;
extern int slideshow;
extern int full_win;
extern int no_panel;
extern int abort_change_ask;
extern int simu_dbl_buffer;
extern int simu_op_dblbuf;
extern int show_cut_buffer;
extern int pezx, pezy;		/* magfactor in pixel edit */
extern int slide_pause_index;
extern const char *hexdigits;
extern char helppath[], bitpath[], startdir[];

extern const char *mfailed, *bugquit, *badfarg, *outbound;

#endif

