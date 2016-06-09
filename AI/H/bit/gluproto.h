/*
 * $Id: gluproto.h,v 0.87 1993/10/11 00:00:00 zhao Pre-Release $
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
 * Prototypes for gl(mouse,overlay,cursor) utilities
 *
 */

#ifndef BIT_GLUPROTO_H
#define BIT_GLUPROTO_H

#include "utype.h"

extern void glu_init(void);
extern int double_buffer_capable(void);

/************ cursors   ****************/
extern void set_busy_cursor(long);
extern void set_cross_cursor(long);
extern void set_square_cursor(long);
extern void set_hand_cursor(long);
extern void set_default_cursor(long, int);
extern int get_cursor_index(void);
extern void set_cursor_byindex(long, int);
extern void rotate_circle_cursor(long);
extern void reset_cursor(long);
extern void set_smallcross_cursor(long);
extern void set_cursor_color(int[]);
extern void get_cursor_color(int[]);

/* mouse:                    */
extern void get_mouse(int *, int *);
extern void get_mouse_r2w(int *, int *);
extern void set_mouse(int, int);
extern void set_mouse_bounds(int, int, int, int);
extern void reset_mouse_bounds(void);

/* other */

#define control_down (getbutton(CTRLKEY) || getbutton(RIGHTCTRLKEY))
#define altkey_down  (getbutton(LEFTALTKEY) || getbutton(RIGHTALTKEY))

#define lmouse_down   getbutton(LEFTMOUSE)
#define mmouse_down   getbutton(MIDDLEMOUSE)
#define rmouse_down   getbutton(RIGHTMOUSE)
#define mouse_down   (lmouse_down || mmouse_down)

/* overlay stuff           */
extern void switch_frame_buffer(void);
extern void clear_over_pup(void);
extern int over_pup_colors;

/* colormap               */
extern void set_colmap(int, pc_t *, pc_t *, pc_t *);
extern void get_colmap(int, pc_t *, pc_t *, pc_t *);
/* routines that draw simple geometric figures */
extern void draw_line(float, float, float, float);
extern void gl_rect(int, int, int, int, int, int);
extern void gl_line(int, int, int, int, int, int);
extern void gl_circ(int, int, int, int, int, int);
extern void gl_tri(int, int, int, int, int, int);
extern void gl_plus(int, int, int, int, int, int);
extern void gl_hdist(int, int, int, int, int, int);
extern void gl_star(int, int, int, int, int, int);
extern void gl_arrow(int, int, int, int, int, int);

/* special routine that does polygons */
extern void gl_poly(const int *, const int *, int, int);
extern void gl_line_poly(const int *, const int *, int);

/* wrappers for GL routines that deals with different  pixel sizes */
extern void clear_screen(long, int);
extern void rect_clear(long, int, int, int, int);
extern void Rectwrite(int, int, int, int, void *);
extern void Rectcopy(int, int, int, int, int, int);
extern void set_rectcp_ras(int);
extern long Rectread(int, int, int, int, void *);
extern void Color(rgba_t, int);
extern void Color4(int[]);
extern void set_color_bk(void);

/****** moving rubbers bands etc *******************/
extern void set_rubber_bounds(int, int, int, int, int);
extern void set_rubber_center(int);
extern void set_rubber_change_rate(int);
extern void set_rubber_obj(int);
extern void rubber_moveto(int *, int *, int *, int *);
extern void rubber_hide(void);
extern void rubber_finish(void);
extern void rubber_show(int);
extern void rubber_new(long, int, int, int, int, int);
extern int get_max_rubber_obj(void);
extern int rubber_on_screen(long *, int *);

extern long rubber_cursor(long, short *, int *, int *, int, int, int);
extern long rubber_info(long, short *, int *, int *, int *, int *, int, int);


/******* infomation about the rubber objects *****/
extern void show_rect_ori(int, int);
extern void show_rect_size(int, int);
extern void show_rect_speed(int);
extern void show_rect_all(int, int, int, int, int);
extern void hide_rect_all(void);

/* misc. external routines */
extern int is_valid_win(long);
/*
 * Macros related to graphics
 */
#include "mac.h"
#ifndef RT_GMAC_H
#define RT_GMAC_H
#define WHERE_R2W(x,y,xo,yo) x = getvaluator(MOUSEX) - xo;\
                             y = getvaluator(MOUSEY) - yo
#endif
#endif

