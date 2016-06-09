/*
 * $Id: gui.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
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
 * graphics interface utility defines and macros, most related to forms
 */

#ifndef BIT_GUI_H
#define BIT_GUI_H

#include "gl/gl.h"
#include "gl/device.h"
#include "forms.h"
#include "utype.h"

/* FORM_CLASS XYPLOT */
#ifndef FORM_XYPLOT_H_
#define FORM_XYPLOT_H_

#define    FL_XYPLOT     63

/***********   type ***************/
#define FL_LINE_XYPLOT          0
#define FL_DASHED_XYPLOT        1
#define FL_DOTTED_XYPLOT        2
#define FL_POINTS_XYPLOT        3
#define FL_CIRCLE_XYPLOT        4
#define FL_SQUARE_XYPLOT        5
#define FL_LINEPOINT_XYPLOT     6
#define FL_LINECIRCLE_XYPLOT    7
#define FL_LINESQUARE_XYPLOT    8
#define FL_IMPULSE_XYPLOT       9
#define FL_IMPULSELINE_XYPLOT   10
#define FL_LINEFILL_XYPLOT      11
#define FL_ACTIVE_XYPLOT        12
#define FL_XYPLOT_MAXTYPE       13

/***** Defaults *****/

#define FL_XYPLOT_BOXTYPE       FL_BORDER_BOX
#define FL_XYPLOT_COL1          FL_COL1
#define FL_XYPLOT_LCOL          FL_LCOL
#define FL_XYPLOT_ALIGN         FL_ALIGN_BOTTOM

/***** Others   *****/

#define FL_XYPLOT_BW            FL_BOUND_WIDTH

/* prototypes */
extern FL_OBJECT *fl_create_xyplot(int, float, float, float, float, const
				   char *);
extern FL_OBJECT *fl_add_xyplot(int, float, float, float, float, const char *);
extern void fl_clear_xyplot(FL_OBJECT *);
extern void fl_clear_xyplot_text(FL_OBJECT *);
extern void fl_set_xyplot(FL_OBJECT *, float *, float *, int,
			  const char *, const char *, const char *);
extern void fl_set_xyplot_only(FL_OBJECT *, float *, float *, int,
			       const char *, const char *, const char *);
extern void fl_set_xyplot_xbounds(FL_OBJECT *, float, float);
extern void fl_set_xyplot_ybounds(FL_OBJECT *, float, float);
extern void fl_set_xyplot_point(FL_OBJECT *, float, float, int);
extern void fl_set_xyplot_autobounds(FL_OBJECT *, int, int);
extern void fl_set_xyplot_xscale(FL_OBJECT *, int, int);
extern void fl_set_xyplot_yscale(FL_OBJECT *, int, int);
extern void fl_set_xyplot_colors(FL_OBJECT *, int, int);
extern void fl_set_xyplot_legend(FL_OBJECT *, int);
extern void fl_set_xyplot_lstyle(FL_OBJECT *, int);
extern void fl_set_xyplot_lsize(FL_OBJECT *, float);
extern void fl_set_xyplot_type(FL_OBJECT *, int);
extern void fl_get_xyplot(FL_OBJECT *, float *, float *, int *);
extern void fl_get_active_xyplot(FL_OBJECT *, float *, float *, int *);
extern void fl_set_xyplot_return(FL_OBJECT *, int);
extern void fl_set_xyplot_text(FL_OBJECT *, float x, float y, int,
			       const char *, int, int, float, int);
extern void fl_add_xyplot_text(FL_OBJECT *, float x, float y, int,
			       const char *, int, int, float, int);
#endif /* FORM_XYPLOT_H */

/* RGB_ICON class */

#define FL_RGB_ICON      64
#define FL_NORMAL_ICON   0
#define FL_RADIO_ICON    1
#define FL_PUSH_ICON     2
#define FL_TOUCH_ICON    3
#define FL_DBL_CLK_ICON  4

/* some default */
#define FL_ICON_BOXTYPE  FL_FLAT_BOX
#define FL_ICON_LCOL     FL_BLUE
#define FL_ICON_LSIZE    12.0
#define FL_ICON_ALIGN    FL_ALIGN_CENTER
#define FL_ICON_LSTYLE   FL_BOLD_STYLE

extern FL_OBJECT *fl_add_icon(int, float, float, float, float, const char *);
extern FL_OBJECT *fl_set_icon_file(FL_OBJECT *, const char *);
extern FL_OBJECT *fl_set_icon_bitmap(FL_OBJECT *, int, int, rgba_t **, int);

extern void fl_set_icon_info(FL_OBJECT *, const char *);
extern const char *fl_get_icon_info(FL_OBJECT *);

extern int fl_get_icon(FL_OBJECT *);
extern int fl_get_icon_dblclk(FL_OBJECT *);
extern void fl_set_icon(FL_OBJECT *, int);

/* END of RGB_ICON */


#ifndef TC_NORMAL_SIZE
#define TC_SMALL_SIZE    10.0
#define TC_NORMAL_SIZE   11.0
#define TC_MEDIUM_SIZE   14.0
#define TC_LARGE_SIZE    16.0
#endif

/*
 * get the forms index and remap the color to the correct one for some X
 * software left the colormap screwed-up
 */
#ifndef FL_COLOR_RE_DEF
#define FL_COLOR_RE_DEF

#define FL_BLACK       0
#define FL_RED         1
#define FL_GREEN       2
#define FL_YELLOW      3
#define FL_BLUE        4
#define FL_MAGENTA     5
#define FL_CYAN        6
#define FL_WHITE       7
#define FL_INACTIVE    8
#define FL_INDIANRED   9
#define FL_SLATEBLUE  12
#define FL_MAGIC1     47
#define FL_MAGIC2     49
#define FL_GRAY68     48
#define FL_GRAY76     50
#define FL_GRAY91     52
#define FL_GRAY92     54
#define FL_GRAY12     34
#define FL_ORANGERED  91
#define FL_DODGERBLUE 226
#endif /* FL_COLOR_RE_DEF */

/* some short hand */
#ifndef FL_SHORT_HAND_DEF
#define FL_SHORT_HAND_DEF

#define FL_NB    FL_NORMAL_BUTTON
#define FL_HB    FL_HIDDEN_BUTTON
#define FL_TB    FL_TOUCH_BUTTON
#define FL_RB    FL_RADIO_BUTTON
#define FL_PB    FL_PUSH_BUTTON
#define FL_RETB  FL_RETURN_BUTTON

#define FL_NC    FL_NORMAL_COUNTER
#define FL_NT    FL_NORMAL_TEXT
#define FL_NI    FL_NORMAL_INPUT
#define FL_FI    FL_FLOAT_INPUT
#define FL_NBM   FL_NORMAL_BITMAP

#define FL_HBR   FL_HOLD_BROWSER
#define FL_NBR   FL_NORMAL_BROWSER
#define FL_MBR   FL_MULTI_BROWSER

#define FL_HFS   FL_HOR_FILL_SLIDER
#define FL_HNS   FL_HOR_NICE_SLIDER

#define FL_NCH   FL_NORMAL_CHOICE
#endif

/* prototypes for basic utilites */
extern FILE *get_BITfile_fp(const char *, const char *);
extern FILE *get_HELPfile_fp(const char *, const char *);
extern char *get_TMPfile(const char *);
extern FILE *get_TMPfile_fp(const char *, const char *);
extern FILE *msg_fopen(const char *, const char *);
extern void check_emergency(void);
extern const char *get_HELPFile(const char *);

/**************************************************************
 * Wrappers for FORMS
 *************************************************************/
extern long bit_show_form(FL_FORM *, int, int, const char *);
extern void bit_hide_form(FL_FORM *);
extern long bit_qread(short *);
extern long bit_check_Q(short *);
extern long bit_handle_event(long, short);
extern long handle_const_q(long, short);

/* prototypes for functions */

extern void gui_init(void);
extern int yes_no(const char *, const char *, const char *, int);
extern int get_orientation(const char *, int *, int);
extern void hide_orientation(void);
extern void TC_continue(const char *, const char *, const char *, int);
extern int getint(const char *, int *, int, int, int);
extern int get2int(const char *, int *, int, int,
		   const char *, int *, int, int,
		   const char *, int *, int, int);
extern int getfloat(const char *, float *, float, float, int, int);

/****** Read a string from keyboard *********/
extern const char *getstring(const char *, const char *, int);
extern void set_getstring_cb(void (*f) (const char *));
extern void getstring_finish(void);

extern void set_message_color(int, int, int);
extern void get_message_color(int *, int *, int *);

/* four charts for histograms */
extern void show_chart4(const char *, float *, float *[], int);
extern void set_chart4_help(int);
extern void set_chart4_ybounds(float *, float *);
extern void set_chart4_text(const char *, const char *, const char *,
			    const
			    char *);
extern void set_chart4_ylabels(const char *, const char *, const char *,
			       const
			       char *);
extern void set_chart4_style(long);

/* actions */
extern int define_action(const char *, const char *);
extern int addto_action(int, const char *, VLfptr);
extern int do_action(int);
extern void free_action(int);
extern void free_all_actions(void);

/* options     */
typedef int (*OptFunc) (int);
extern int def_option(const char *, int, const char *);
extern int set_option_act(int, const char *, OptFunc);
extern int getopt_index(const char *);
extern int addto_option(int, const char *, const char *, const char *, int *, const char *, VIIpfptr);
extern int replace_option(int, int, int *, const char *);
extern int do_option(int);
extern void free_option(int);
extern void free_all_options(void);
extern int load_optfile(const char *, int);

extern void deactivate_all_forms(void);

extern void set_hand_cursor(long);
extern void rubber_hide(void);
extern void rubber_show(int);

extern void show_help(int, int, int, int, int);
extern void add_to_help(const char *);
extern void insert_help(const char *, int);
extern void clear_help(void);
extern void load_help(const char *, const char *);
extern void help_cb(FL_OBJECT *, long);
extern void print_help(const char *, const char *, int, int, int, int);
extern void set_help_title(const char *);

extern int is_valid_win(long win);
extern long myqread(short *, long, long);
extern void show_busy(const char *);
extern void end_busy(void);
extern void rotate_cursor(void);
extern void remove_rotate_cursor(void);
extern void fl_set_form_hotspot(FL_FORM *, float, float);
extern const char *getfilename(const char *, const char *,
			       const char *, const char *, int);

/* from file control.c */
extern long get_control_wid(void);
extern long get_info_wid(void);
extern void show_misc_info2(const char *);
extern void hide_misc_info2(void);

/* from gl_util.c */
#include "ulib.h"
#include "uproto.h"
#include "gluproto.h"

extern int form_single;
extern int always_border;
extern char bitpath[], helppath[];
extern long win_id, win_xo, win_yo, win_w, win_h;

#endif /* GUI_H_ */

