/************************************************************************
 *									*
 *  Forms is a library for building up dialogue and interaction boxes.  *
 *  It is based of the Silicon Graphics Graphical Library.		*
 *									*
 *  This software is public domain. It may not be resold.		*
 *									*
 *  Written by: Mark Overmars						*
 *              Department of Computer Science				*
 *              University of Utrecht					*
 *		P.O. Box 80.089						*
 *              3508 TB Utrecht, the Netherlands			*
 *		Email: markov@cs.ruu.nl					*
 *									*
 *  Version 2.2 d							*
 *  Date:  Jun 23, 1993							*
 ************************************************************************/

#ifndef __FORMS_H__
#define __FORMS_H__

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h>
#include "gl/gl.h"

/************   The type OBJECT            ************/

struct forms;

typedef struct objs {
   int objclass;        /* What type of object */
   int type;            /* The type within the class */
   int boxtype;         /* The type of the bounding box */
   float x,y,w,h;       /* The bounding box */
   int col1,col2;       /* Two possible colors */

   char *label;		/* The label */
   int align;		/* Label or text alignment */
   int lcol;		/* Color of the label */
   float lsize; 	/* Size of the label */
   int lstyle;		/* Style of the text label */

   char *shortcut;	/* The list of shortcuts */

   int (*handle)(struct objs *, int, float, float, char);
			/* Handling procedure. */

   int *spec;           /* pointer to special stuff for object */

   int pushed;		/* wheter pushed */
   int focus; 		/* wheter focussed */
   int belowmouse;	/* Whether currently below the mouse */

   int redraw;		/* How often the object must be redrawn (0,1,2) */

   int active;		/* Whether active */
   int input;		/* Whether receiving input */
   int wantall;		/* Whether the input object wants <Ret>, <Tab> */
   int visible;         /* Whether being displayed */
   int radio;           /* Whether a radio object */
   int automatic;       /* Whether this object gets timer events */
   void (*object_call_back)(struct objs *, long);
			/* The call-back routine */
   long argument;	/* Its argument */

   struct objs *next;	/* Next object in the form */
   struct objs *prev;	/* Previous object in the form */
   struct forms *form;	/* Form to which object belong */
} FL_OBJECT;

/************   The type FORM              ************/

typedef struct forms {
   long window;         /* Window of the form */
   float w,h;           /* The size of the form */
   long x,y;		/* Position of form on screen */
   float hotx, hoty;    /* hot spots added by TCZ     */

   int deactivated;	/* Whether deactivated */
   int visible;         /* Whether being displayed */
   int frozen;          /* When true no redraws are performed */

   int doublebuf;	/* Whether in double buffer mode */

   void (*form_call_back)(struct objs *);
			/* The call-back routine */

   struct objs *first;	/* First object in the form */
   struct objs *last;	/* Last object in the form */

   struct objs *focusobj; /* Object to which input is directed */
} FL_FORM;

/************   General Constants          ************/

#ifndef NULL
#define NULL                    0
#endif

#ifndef FALSE
#define FALSE                   0
#endif

#ifndef TRUE
#define TRUE                    1
#endif

/***** Placement *****/

#define FL_PLACE_FREE		0
#define FL_PLACE_SIZE		1
#define FL_PLACE_ASPECT		2
#define FL_PLACE_MOUSE		3
#define FL_PLACE_CENTER		4
#define FL_PLACE_POSITION	5
#define FL_PLACE_FULLSCREEN	6
#define FL_PLACE_HOTSPOT        7   /* TCZ */

/***** Finds *****/

#define FL_FIND_INPUT		0
#define FL_FIND_AUTOMATIC	1
#define FL_FIND_MOUSE		2

/***** Special Objects  *****/

#define FL_BEGIN_GROUP		10000
#define FL_END_GROUP		20000

/***** Alignments *****/

#define FL_ALIGN_TOP		0
#define FL_ALIGN_BOTTOM		1
#define FL_ALIGN_LEFT		2
#define FL_ALIGN_RIGHT		3
#define FL_ALIGN_CENTER		4

/***** Bounding boxes *****/

#define FL_NO_BOX		0
#define FL_UP_BOX		1
#define FL_DOWN_BOX		2
#define FL_FLAT_BOX		3
#define FL_BORDER_BOX		4
#define FL_SHADOW_BOX		5
#define FL_FRAME_BOX		6
#define FL_ROUNDED_BOX		7
#define FL_RFLAT_BOX		8
#define FL_RSHADOW_BOX		9

/***** Boundary Colors *****/

#define FL_TOP_BOUND_COL	51
#define FL_LEFT_BOUND_COL	55
#define FL_BOT_BOUND_COL	40
#define FL_RIGHT_BOUND_COL	35

#define FL_COL1			47
#define FL_MCOL			49
#define FL_LCOL			0

#define FL_BOUND_WIDTH		3.0

/***** Events *****/

#define FL_DRAW			0
#define FL_PUSH			1
#define FL_RELEASE		2
#define FL_ENTER		3
#define FL_LEAVE		4
#define FL_MOUSE		5
#define FL_FOCUS		6
#define FL_UNFOCUS		7
#define FL_KEYBOARD		8
#define FL_STEP			9
#define FL_MOVE			10
#define FL_SHORTCUT		11
#define FL_FREEMEM		12

/***** Font *****/

#define FL_MAXFONT		32

#define FL_FONT_NAME_0		"Helvetica"
#define FL_FONT_NAME_1		"Helvetica-Bold"
#define FL_FONT_NAME_2		"Helvetica-Oblique"
#define FL_FONT_NAME_3		"Courier"
#define FL_FONT_NAME_4		"Courier-Bold"
#define FL_FONT_NAME_5		"Courier-Oblique"
#define FL_FONT_NAME_6		"Times-Roman"
#define FL_FONT_NAME_7		"Times-Bold"
#define FL_FONT_NAME_8		"Times-Italic"
#define FL_FONT_NAME_9		"Bookman-Light"
#define FL_FONT_NAME_10		"Bookman-Demi"
#define FL_FONT_NAME_11		"Bookman-LightItalic"
#define FL_FONT_NAME_12		"Symbol"
#define FL_FONT_NAME_13		"Type"
#define FL_FONT_NAME_14		"Iris"
#define FL_FONT_NAME_15		"ZapfDingbats"

#define FL_SMALL_FONT		8.0
#define FL_NORMAL_FONT		10.0
#define FL_LARGE_FONT		18.0

#define FL_NORMAL_STYLE		0
#define FL_BOLD_STYLE		1
#define FL_ITALIC_STYLE		2
#define FL_FIXED_STYLE		3

/* NEXT ONES WILL BE REMOVED */
#define FL_ENGRAVED_STYLE	4
#define FL_ICON_STYLE		5


/************   General Routines           ************/

/***** In objects.c *****/

typedef int (*FL_HANDLEPTR)(FL_OBJECT *, int , float, float, char);

FL_FORM		*fl_make_form(float,float); 
FL_OBJECT	*fl_make_object(int,int,float,float,float,float,const char *, FL_HANDLEPTR);

void	fl_free_object(FL_OBJECT *);
void	fl_free_form(FL_FORM *);

void	fl_add_object(FL_FORM *, FL_OBJECT*);
void	fl_insert_object(FL_OBJECT *, FL_OBJECT*);
void	fl_delete_object(FL_OBJECT *);

void	fl_set_object_align(FL_OBJECT *, int);
void	fl_set_object_boxtype(FL_OBJECT *, int);
void	fl_set_object_color(FL_OBJECT *, int, int);
void	fl_set_object_label(FL_OBJECT *, const char *);
void	fl_set_object_lcol(FL_OBJECT *, int);
void	fl_set_object_lsize(FL_OBJECT *, float );
void	fl_set_object_lstyle(FL_OBJECT *, int);

void	fl_show_object(FL_OBJECT *);
void	fl_hide_object(FL_OBJECT *);
void	fl_hide_object_only(FL_OBJECT *);

void	fl_activate_object(FL_OBJECT *);
void	fl_deactivate_object(FL_OBJECT *);

void 	fl_set_object_focus(FL_FORM *, FL_OBJECT *);

void	fl_set_object_shortcut(FL_OBJECT *, const char *);

FL_OBJECT	*fl_find_object(FL_OBJECT *, int, float, float);
FL_OBJECT	*fl_find_first(FL_FORM *, int, float, float);
FL_OBJECT	*fl_find_last(FL_FORM *, int, float, float);

void	fl_redraw_object(FL_OBJECT *);
void	fl_redraw_form(FL_FORM *);

void	fl_freeze_object(FL_OBJECT *);
void	fl_unfreeze_object(FL_OBJECT *);
void	fl_freeze_form(FL_FORM *);
void	fl_unfreeze_form(FL_FORM *);

void	fl_handle_object(FL_OBJECT *, int, float, float, char);
int	fl_handle_object_direct(FL_OBJECT *, int, float, float, char);

/***** In forms.c *****/

extern FL_FORM *fl_current_form;

FL_FORM		*fl_bgn_form(int, float, float);
void		fl_end_form(void);
void  		fl_addto_form(FL_FORM *);

FL_OBJECT	*fl_bgn_group(void);
FL_OBJECT	*fl_end_group(void);

void		fl_scale_form(FL_FORM *, float, float);
void		fl_set_form_position(FL_FORM *, long, long);
void		fl_set_form_size(FL_FORM *, long, long);
long		fl_show_form(FL_FORM *, int, int, const char *); /*TCZ*/
void            fl_set_form_hotspot(FL_FORM *, float, float);    /*TCZ*/
void		fl_hide_form(FL_FORM *);	

void		fl_treat_interaction_events(int);
FL_OBJECT	*fl_do_forms(void);
FL_OBJECT	*fl_check_forms(void);
FL_OBJECT	*fl_do_only_forms(void);
FL_OBJECT	*fl_check_only_forms(void);

void		fl_activate_form(FL_FORM *);
void		fl_deactivate_form(FL_FORM *);
void		fl_activate_all_forms();
void		fl_deactivate_all_forms();

/***** In events.c *****/

extern FL_OBJECT *FL_EVENT;

typedef void (*FL_CALLBACKPTR)(FL_OBJECT *, long);
typedef void (*FL_FORMCALLBACKPTR)(FL_OBJECT *);
typedef void (*FL_EVENTCALLBACKPTR)(short, short);

void	fl_init_events();

void	fl_set_call_back(FL_OBJECT *, FL_CALLBACKPTR, long);
void	fl_set_event_call_back(FL_EVENTCALLBACKPTR);
void	fl_set_form_call_back(FL_FORM *, FL_FORMCALLBACKPTR);

void 	fl_qdevice(Device);
void 	fl_unqdevice(Device);
int 	fl_isqueued(Device);
long 	fl_qtest(void);
long 	fl_qread(short *);
long 	fl_blkqread(short *, short);
void 	fl_qreset(void);
void 	fl_qenter(short,short);
void 	fl_tie(Device, Device, Device);

void	fl_treat_user_events(void);

FL_OBJECT 	*fl_object_qread(void);
void 		fl_object_qenter(FL_OBJECT *);

/***** In goodies.c *****/

long 	fl_show_buttonbox(void);
void 	fl_hide_buttonbox(void);
void 	fl_set_buttonbox_label(int, char []);

void 	fl_show_message(char [], char[], char []);
int 	fl_show_question(char [], char[], char []);
int 	fl_show_choice(char [], char[], char [], int,char [], char[], char []);
const char 	*fl_show_input(char[], char[]);

int 	fl_show_colormap(int);

/***** In fselect.c *****/

char 	*fl_show_file_selector(char [], char [], char [], char []);

char 	*fl_get_directory();
char 	*fl_get_pattern();
char 	*fl_get_filename();

/***** In draw.c *****/

void	fl_get_mouse(float *, float *);

void	fl_set_clipping(float, float, float, float);
void	fl_unset_clipping(void);

void	fl_init_fonts();
void	fl_set_font_name(int, char []);
void	fl_set_font(char [], char [], char [], char[]); /* REMOVED SOON */
float	fl_get_char_height(float, int);
float	fl_get_char_width(float, int, char);
float	fl_get_string_width(float, int, char []);

void	fl_init_colormap();
void	fl_color(int);
void	fl_mapcolor(int, short, short, short);
void	fl_getmcolor(int, short *, short *, short *);

void    fl_line(float, float, float, float, int);
void    fl_rect(float, float, float, float, int);
void    fl_bound(float, float, float, float, int);
void    fl_rectbound(float, float, float, float, int);

void	fl_drw_box(int ,float, float, float, float, int,float);
void	fl_drw_string(int, int, float, float, float, float,
			int, int, int, int, float, int,
			int, int, int, char []);
int	fl_get_pos_in_string(int, int, float, float, float, float,
			float, int, float, float, char []);
			
void	fl_drw_text(int, float, float, float, float, int, float, int, char[]);
void	fl_drw_text_beside(int, float, float, float, float, int, float, int, char[]);
void	fl_drw_text_cursor(int, float, float, float, float, int, float, int, char[], int, int);

/***** In symbols.c *****/

typedef void (*FL_DRAWPTR)(int);

void 	fl_init_symbols();

int 	fl_add_symbol(char [], FL_DRAWPTR, int);
int 	fl_draw_symbol(char [], float, float, float,float, int);

/***** In support.c *****/

extern int fl_rgbmode;
extern int fl_doublebuf;

void	fl_init();
void	fl_set_graphics_mode(int, int);

void	fl_show_errors(int);
void	fl_error(char [], char []);

void	*fl_malloc(size_t);

void	fl_save_user_window();
void	fl_restore_user_window();
void	fl_set_forms_window(FL_FORM *);

/************************* The Classes ***********************/

/************   Object Class: Bitmap       ************/

/***** Class    *****/

#define FL_BITMAP		3

/***** Types    *****/

#define FL_NORMAL_BITMAP	0

/***** Defaults *****/

#define FL_BITMAP_BOXTYPE	FL_NO_BOX
#define FL_BITMAP_COL1		0
#define FL_BITMAP_COL2		FL_COL1
#define FL_BITMAP_LCOL		FL_LCOL
#define FL_BITMAP_ALIGN		FL_ALIGN_BOTTOM

/***** Others   *****/

#define FL_BITMAP_MAXSIZE	128*128
#define FL_BITMAP_BW		FL_BOUND_WIDTH

/***** Routines *****/

FL_OBJECT 	*fl_create_bitmap(int, float, float, float, float, char []);
FL_OBJECT 	*fl_add_bitmap(int, float, float, float, float, char []);

void		fl_set_bitmap(FL_OBJECT *, int, int, char *);


/************   Object Class: Box          ************/

/***** Class    *****/

#define FL_BOX			1

/***** Types    *****/

/* See the bouding boxes */

/***** Defaults *****/

#define FL_BOX_BOXTYPE		FL_UP_BOX
#define FL_BOX_COL1		FL_COL1
#define FL_BOX_LCOL		FL_LCOL
#define FL_BOX_ALIGN		FL_ALIGN_CENTER

/***** Others   *****/

#define FL_BOX_BW		FL_BOUND_WIDTH

/***** Routines *****/

FL_OBJECT	*fl_create_box(int, float, float, float, float, const char *);
FL_OBJECT	*fl_add_box(int, float, float, float, float, const char *);

/************   Object Class: Browser      ************/

/***** Class    *****/

#define FL_BROWSER		71

/***** Types    *****/

#define FL_NORMAL_BROWSER	0
#define FL_SELECT_BROWSER	1
#define FL_HOLD_BROWSER		2
#define FL_MULTI_BROWSER	3

/***** Defaults *****/

#define FL_BROWSER_BOXTYPE	FL_DOWN_BOX
#define FL_BROWSER_COL1		FL_COL1
#define FL_BROWSER_COL2		3
#define FL_BROWSER_LCOL		FL_LCOL
#define FL_BROWSER_ALIGN	FL_ALIGN_BOTTOM

/***** Others   *****/

#define FL_BROWSER_SLCOL	FL_COL1
#define FL_BROWSER_BW		FL_BOUND_WIDTH
#define FL_BROWSER_LINELENGTH	128
#define FL_BROWSER_MAXLINE	512

/***** Routines *****/

FL_OBJECT *fl_create_browser(int, float, float, float, float, const char *);
FL_OBJECT *fl_add_browser(int, float, float, float, float, const char *);

void 	fl_set_browser_topline(FL_OBJECT *, int);
int 	fl_get_browser_topline(FL_OBJECT *);
void 	fl_clear_browser(FL_OBJECT *);
void 	fl_add_browser_line(FL_OBJECT *, const char *);
void 	fl_addto_browser(FL_OBJECT *, const char *);
void 	fl_insert_browser_line(FL_OBJECT *, int , const char *);
void 	fl_delete_browser_line(FL_OBJECT *, int );
void 	fl_replace_browser_line(FL_OBJECT *, int , const char *);
const char	*fl_get_browser_line(FL_OBJECT *, int);
int 	fl_load_browser(FL_OBJECT *, char []);
int	fl_get_browser_maxline(FL_OBJECT *);
void 	fl_select_browser_line(FL_OBJECT *, int );
void 	fl_deselect_browser_line(FL_OBJECT *, int );
void 	fl_deselect_browser(FL_OBJECT *);
int 	fl_isselected_browser_line(FL_OBJECT *, int );
int 	fl_get_browser(FL_OBJECT *);
void 	fl_set_browser_fontsize(FL_OBJECT *, float);
void 	fl_set_browser_fontstyle(FL_OBJECT *, int);
void 	fl_set_browser_specialkey(FL_OBJECT *, char);
/************   Object Class: Button       ************/

/***** Class    *****/

#define FL_BUTTON			11

/***** Types    *****/

#define FL_NORMAL_BUTTON	0
#define FL_PUSH_BUTTON		1
#define FL_RADIO_BUTTON		2
#define FL_HIDDEN_BUTTON	3
#define FL_TOUCH_BUTTON		4
#define FL_INOUT_BUTTON		5
#define FL_RETURN_BUTTON	6
#define FL_HIDDEN_RET_BUTTON	7

/***** Defaults *****/

#define FL_BUTTON_BOXTYPE	FL_UP_BOX
#define FL_BUTTON_COL1		FL_COL1
#define FL_BUTTON_COL2		FL_COL1
#define FL_BUTTON_LCOL		FL_LCOL
#define FL_BUTTON_ALIGN		FL_ALIGN_CENTER

/***** Others   *****/

#define FL_BUTTON_MCOL1		FL_MCOL
#define FL_BUTTON_MCOL2		FL_MCOL
#define FL_BUTTON_BW		FL_BOUND_WIDTH

/***** Routines *****/

FL_OBJECT	*fl_create_button(int, float, float, float, float, const char *);
FL_OBJECT	*fl_add_button(int, float, float, float, float, const char *);

int	fl_get_button(FL_OBJECT *);
void	fl_set_button(FL_OBJECT *, int);
int 	fl_get_button_numb(FL_OBJECT *);


void	fl_set_button_shortcut(FL_OBJECT *, const char *);

/************   Object Class: Chart         ************/

/***** Class    *****/

#define FL_CHART		4

/***** Types    *****/

#define FL_BAR_CHART		0
#define FL_HORBAR_CHART		1
#define FL_LINE_CHART		2
#define FL_FILLED_CHART		3
#define FL_SPIKE_CHART		4
#define FL_PIE_CHART		5
#define FL_SPECIALPIE_CHART	6

/***** Defaults *****/

#define FL_CHART_BOXTYPE	FL_BORDER_BOX
#define FL_CHART_COL1		FL_COL1
#define FL_CHART_LCOL		FL_LCOL
#define FL_CHART_ALIGN		FL_ALIGN_BOTTOM

/***** Others   *****/

#define FL_CHART_BW		FL_BOUND_WIDTH
#define FL_CHART_MAX		512

/***** Routines *****/

FL_OBJECT *fl_create_chart(int, float, float, float, float, const char *);
FL_OBJECT *fl_add_chart(int, float, float, float, float, const char * );

void	fl_clear_chart(FL_OBJECT *);
void	fl_add_chart_value(FL_OBJECT *, float, char [], int);
void	fl_insert_chart_value(FL_OBJECT *, int, float, char [], int);
void	fl_replace_chart_value(FL_OBJECT *, int, float, char [], int);
void	fl_set_chart_bounds(FL_OBJECT *, float, float);
void	fl_set_chart_maxnumb(FL_OBJECT *, int);
void	fl_set_chart_autosize(FL_OBJECT *, int);
/************   Object Class: Choice       ************/

/***** Class    *****/

#define FL_CHOICE		42

/***** Types    *****/

#define FL_NORMAL_CHOICE	0

/***** Defaults *****/

#define FL_CHOICE_BOXTYPE	FL_DOWN_BOX
#define FL_CHOICE_COL1		FL_COL1
#define FL_CHOICE_COL2		FL_LCOL
#define FL_CHOICE_LCOL		FL_LCOL
#define FL_CHOICE_ALIGN		FL_ALIGN_LEFT

/***** Others   *****/

#define FL_CHOICE_BW		FL_BOUND_WIDTH
#define FL_CHOICE_MCOL		FL_MCOL
#define FL_CHOICE_MAXITEMS	128
#define FL_CHOICE_MAXSTR	64

/***** Routines *****/

FL_OBJECT 	*fl_create_choice(int ,float ,float ,float ,float ,const char *);
FL_OBJECT 	*fl_add_choice(int ,float ,float ,float ,float, const char *);

void 	fl_clear_choice(FL_OBJECT *);
void 	fl_addto_choice(FL_OBJECT *, const char *);
void 	fl_replace_choice(FL_OBJECT *, int , const char *);
void 	fl_delete_choice(FL_OBJECT *, int );
void 	fl_set_choice(FL_OBJECT *, int );
int 	fl_get_choice(FL_OBJECT *);
char	*fl_get_choice_text(FL_OBJECT *);
void	fl_set_choice_fontsize(FL_OBJECT *, float);
void	fl_set_choice_fontstyle(FL_OBJECT *, int);


/************   Object Class: Clock        ************/

/***** Class    *****/

#define FL_CLOCK		61

/***** Types    *****/

#define FL_SQUARE_CLOCK		0
#define FL_ROUND_CLOCK		1

/***** Defaults *****/

#define FL_CLOCK_BOXTYPE	FL_UP_BOX
#define FL_CLOCK_COL1		37
#define FL_CLOCK_COL2		42
#define FL_CLOCK_LCOL		FL_LCOL
#define FL_CLOCK_ALIGN		FL_ALIGN_BOTTOM

/***** Others   *****/

#define FL_CLOCK_TOPCOL		FL_COL1
#define FL_CLOCK_BW		FL_BOUND_WIDTH

/***** Routines *****/

FL_OBJECT	*fl_create_clock(int, float, float, float, float, char []);
FL_OBJECT	*fl_add_clock(int, float, float, float, float, char []);

void	fl_get_clock(FL_OBJECT *,int * ,int *, int *);

/************   Object Class: Counter       ************/

/***** Class    *****/

#define FL_COUNTER		25

/***** Types    *****/

#define FL_NORMAL_COUNTER	0
#define FL_SIMPLE_COUNTER	1

/***** Defaults *****/

#define FL_COUNTER_BOXTYPE	FL_UP_BOX
#define FL_COUNTER_COL1		FL_COL1
#define FL_COUNTER_COL2		4
#define FL_COUNTER_LCOL		FL_LCOL
#define FL_COUNTER_ALIGN	FL_ALIGN_BOTTOM

/***** Others   *****/

#define FL_COUNTER_BW		FL_BOUND_WIDTH

/***** Routines *****/

FL_OBJECT	*fl_create_counter(int, float, float, float, float, char []);
FL_OBJECT	*fl_add_counter(int, float, float, float, float, char []);

void 	fl_set_counter_value(FL_OBJECT *, float);
void 	fl_set_counter_bounds(FL_OBJECT *, float, float);
void 	fl_set_counter_step(FL_OBJECT *, float, float);
void	fl_set_counter_precision(FL_OBJECT *, int);
float 	fl_get_counter_value(FL_OBJECT *);

void	fl_set_counter_return(FL_OBJECT *, int);
/************   Object Class: Dial         ************/

/***** Class    *****/

#define FL_DIAL			22

/***** Types    *****/

#define FL_NORMAL_DIAL		0
#define FL_LINE_DIAL		1

/***** Defaults *****/

#define FL_DIAL_BOXTYPE		FL_NO_BOX
#define FL_DIAL_COL1		FL_COL1
#define FL_DIAL_COL2		37
#define FL_DIAL_LCOL		FL_LCOL
#define FL_DIAL_ALIGN		FL_ALIGN_BOTTOM

/***** Others   *****/

#define FL_DIAL_TOPCOL		FL_COL1
#define FL_DIAL_BW		FL_BOUND_WIDTH

/***** Routines *****/

FL_OBJECT	*fl_create_dial(int ,float, float, float, float, char []);
FL_OBJECT	*fl_add_dial(int ,float, float, float, float, char []);

void 	fl_set_dial_value(FL_OBJECT *, float);
float 	fl_get_dial_value(FL_OBJECT *);
void 	fl_set_dial_bounds(FL_OBJECT *, float, float);
void 	fl_get_dial_bounds(FL_OBJECT *, float *, float*);

void    fl_set_dial_step(FL_OBJECT *, float);
void 	fl_set_dial_return(FL_OBJECT *, int);
/************   Object Class: Free         ************/

/***** Class    *****/

#define FL_FREE			101

/***** Types    *****/

#define FL_NORMAL_FREE		1
#define FL_SLEEPING_FREE	2
#define FL_INPUT_FREE		3
#define FL_CONTINUOUS_FREE	4
#define FL_ALL_FREE		5

/***** Defaults *****/

/***** Others   *****/

/***** Routines *****/

FL_OBJECT 	*fl_create_free(int, float, float, float, float, char [], FL_HANDLEPTR);
FL_OBJECT 	*fl_add_free(int, float, float, float, float, char [], FL_HANDLEPTR);

/************   Object Class: Input        ************/

/***** Class    *****/

#define FL_INPUT		31

/***** Types    *****/

#define FL_NORMAL_INPUT		0
#define FL_FLOAT_INPUT		1
#define FL_INT_INPUT		2
#define FL_HIDDEN_INPUT		3
#define FL_MULTILINE_INPUT	4
#define FL_SECRET_INPUT		5

/***** Defaults *****/

#define FL_INPUT_BOXTYPE	FL_DOWN_BOX
#define FL_INPUT_COL1		13
#define FL_INPUT_COL2		5
#define FL_INPUT_LCOL		FL_LCOL
#define FL_INPUT_ALIGN		FL_ALIGN_LEFT

/***** Others   *****/

#define FL_INPUT_TCOL		FL_LCOL
#define FL_INPUT_CCOL		4
#define FL_INPUT_BW		FL_BOUND_WIDTH

/***** Routines *****/

FL_OBJECT	*fl_create_input(int, float, float, float, float, const char *);
FL_OBJECT	*fl_add_input(int, float, float, float, float, const char *);

void	fl_set_input(FL_OBJECT *, const char *);
void	fl_set_input_color(FL_OBJECT *, int, int);
const char	*fl_get_input(FL_OBJECT *);
void	fl_set_input_return(FL_OBJECT *, int);

/************   Object Class: Lightbutton  ************/

/***** Class    *****/

#define FL_LIGHTBUTTON		12

/***** Types    *****/

    /* Same types as for buttons */

/***** Defaults *****/

#define FL_LIGHTBUTTON_BOXTYPE	FL_UP_BOX
#define FL_LIGHTBUTTON_COL1	39
#define FL_LIGHTBUTTON_COL2	3
#define FL_LIGHTBUTTON_LCOL	FL_LCOL
#define FL_LIGHTBUTTON_ALIGN	FL_ALIGN_CENTER

/***** Others   *****/

#define FL_LIGHTBUTTON_TOPCOL	FL_COL1
#define FL_LIGHTBUTTON_MCOL	FL_MCOL
#define FL_LIGHTBUTTON_BW1	FL_BOUND_WIDTH
#define FL_LIGHTBUTTON_BW2	2.0
#define FL_LIGHTBUTTON_MINSIZE	12.0

/***** Routines *****/

FL_OBJECT  *fl_create_lightbutton(int, float, float, float, float, const char *);
FL_OBJECT  *fl_add_lightbutton(int, float, float, float, float, const char *);

/************   Object Class: Menu         ************/

/***** Class    *****/

#define FL_MENU			41

/***** Types    *****/

#define FL_TOUCH_MENU		0
#define FL_PUSH_MENU		1

/***** Defaults *****/

#define FL_MENU_BOXTYPE		FL_BORDER_BOX
#define FL_MENU_COL1		55
#define FL_MENU_COL2		37
#define FL_MENU_LCOL		FL_LCOL
#define FL_MENU_ALIGN		FL_ALIGN_CENTER

/***** Others   *****/

#define FL_MENU_BW		FL_BOUND_WIDTH
#define FL_MENU_MAXITEMS	128
#define FL_MENU_MAXSTR		64

/***** Routines *****/

FL_OBJECT	*fl_create_menu(int, float, float, float, float, char []);
FL_OBJECT	*fl_add_menu(int, float, float, float, float, char []);

void	fl_clear_menu(FL_OBJECT *);
void	fl_set_menu(FL_OBJECT *, char []);
void	fl_addto_menu(FL_OBJECT *, char []);
void	fl_replace_menu_item(FL_OBJECT *, int, char []);
void	fl_delete_menu_item(FL_OBJECT *, int);

void	fl_set_menu_item_shortcut(FL_OBJECT *, int, char []);
void	fl_set_menu_item_mode(FL_OBJECT *, int, long);
void	fl_show_menu_symbol(FL_OBJECT *, int);

int	fl_get_menu(FL_OBJECT *);
char	*fl_get_menu_text(FL_OBJECT *);

/************   Object Class: Positioner   ************/

/***** Class    *****/

#define FL_POSITIONER		23

/***** Types    *****/

#define FL_NORMAL_POSITIONER	0

/***** Defaults *****/

#define FL_POSITIONER_BOXTYPE	FL_DOWN_BOX
#define FL_POSITIONER_COL1	FL_COL1
#define FL_POSITIONER_COL2	1
#define FL_POSITIONER_LCOL	FL_LCOL
#define FL_POSITIONER_ALIGN	FL_ALIGN_BOTTOM

/***** Others   *****/

#define FL_POSITIONER_BW	FL_BOUND_WIDTH

/***** Routines *****/

FL_OBJECT	*fl_create_positioner(int, float, float, float, float, char []);
FL_OBJECT	*fl_add_positioner(int, float, float, float, float, char []);

void 	fl_set_positioner_xvalue(FL_OBJECT *, float);
float 	fl_get_positioner_xvalue(FL_OBJECT *);
void 	fl_set_positioner_xbounds(FL_OBJECT *, float, float);
void 	fl_get_positioner_xbounds(FL_OBJECT *, float *, float *);
void 	fl_set_positioner_yvalue(FL_OBJECT *, float);
float 	fl_get_positioner_yvalue(FL_OBJECT *);
void 	fl_set_positioner_ybounds(FL_OBJECT *, float, float);
void 	fl_get_positioner_ybounds(FL_OBJECT *, float *, float *);
void    fl_set_positioner_xstep(FL_OBJECT *, float);
void    fl_set_positioner_ystep(FL_OBJECT *, float);


void 	fl_set_positioner_return(FL_OBJECT *, int);
/************   Object Class: Roundbutton  ************/

/***** Class    *****/

#define FL_ROUNDBUTTON		13

/***** Types    *****/

    /* Same types as for buttons */

/***** Defaults *****/

#define FL_ROUNDBUTTON_BOXTYPE	FL_NO_BOX
#define FL_ROUNDBUTTON_COL1	7
#define FL_ROUNDBUTTON_COL2	3
#define FL_ROUNDBUTTON_LCOL	FL_LCOL
#define FL_ROUNDBUTTON_ALIGN	FL_ALIGN_CENTER

/***** Others   *****/

#define FL_ROUNDBUTTON_TOPCOL	FL_COL1
#define FL_ROUNDBUTTON_MCOL	FL_MCOL
#define FL_ROUNDBUTTON_BW	FL_BOUND_WIDTH

/***** Routines *****/

FL_OBJECT  *fl_create_roundbutton(int, float, float, float, float, const char *);
FL_OBJECT  *fl_add_roundbutton(int, float, float, float, float, const char *);


/************   Object Class: Slider       ************/

/***** Class    *****/

#define FL_SLIDER		21
#define FL_VALSLIDER		24

/***** Types    *****/

#define FL_VERT_SLIDER		0
#define FL_HOR_SLIDER		1
#define FL_VERT_FILL_SLIDER	2
#define FL_HOR_FILL_SLIDER	3
#define FL_VERT_NICE_SLIDER	4
#define FL_HOR_NICE_SLIDER	5

/***** Defaults *****/

#define FL_SLIDER_BOXTYPE	FL_DOWN_BOX
#define FL_SLIDER_COL1		FL_COL1
#define FL_SLIDER_COL2		FL_COL1
#define FL_SLIDER_LCOL		FL_LCOL
#define FL_SLIDER_ALIGN		FL_ALIGN_BOTTOM

/***** Others   *****/

#define FL_SLIDER_BW1		FL_BOUND_WIDTH
#define FL_SLIDER_BW2		2.0

#define FL_SLIDER_FINE		0.05
#define FL_SLIDER_WIDTH		0.08

/***** Routines *****/

FL_OBJECT	*fl_create_slider(int, float, float, float, float, char []);
FL_OBJECT	*fl_add_slider(int, float, float, float, float, char []);

FL_OBJECT	*fl_create_valslider(int, float, float, float, float, char []);
FL_OBJECT	*fl_add_valslider(int, float, float, float, float, char []);

void 	fl_set_slider_value(FL_OBJECT *, float);
float 	fl_get_slider_value(FL_OBJECT *);
void 	fl_set_slider_bounds(FL_OBJECT *, float, float);
void 	fl_get_slider_bounds(FL_OBJECT *, float *, float *);

void	fl_set_slider_return(FL_OBJECT *, int);

void	fl_set_slider_step(FL_OBJECT *, float);
void	fl_set_slider_size(FL_OBJECT *, float);
void	fl_set_slider_precision(FL_OBJECT *, int);

/***** Routines in sldraw.c *****/

void	fl_drw_slider(int, float, float, float, float, int, int, int,
			float, float, char[]);

int	fl_get_pos_in_slider(float, float, float, float, int,
			float, float, float, float, float *);
/************   Object Class: Text         ************/

/***** Class    *****/

#define FL_TEXT			2

/***** Types    *****/

#define FL_NORMAL_TEXT		0

/***** Defaults *****/

#define FL_TEXT_BOXTYPE		FL_NO_BOX
#define FL_TEXT_COL1		FL_COL1
#define FL_TEXT_LCOL		FL_LCOL
#define FL_TEXT_ALIGN		FL_ALIGN_LEFT

/***** Others   *****/

#define FL_TEXT_BW		FL_BOUND_WIDTH

/***** Routines *****/

FL_OBJECT 	*fl_create_text(int, float, float, float, float, const char *);
FL_OBJECT 	*fl_add_text(int, float, float, float, float, const char *);

/************   Object Class: Timer        ************/

/***** Class    *****/

#define FL_TIMER		62

/***** Types    *****/

#define FL_NORMAL_TIMER		0
#define FL_VALUE_TIMER		1
#define FL_HIDDEN_TIMER		2

/***** Defaults *****/

#define FL_TIMER_BOXTYPE	FL_DOWN_BOX
#define FL_TIMER_COL1		FL_COL1
#define FL_TIMER_COL2		1
#define FL_TIMER_LCOL		FL_LCOL
#define FL_TIMER_ALIGN		FL_ALIGN_CENTER

/***** Others   *****/

#define FL_TIMER_BW		FL_BOUND_WIDTH
#define FL_TIMER_BLINKRATE	0.2

/***** Routines *****/

FL_OBJECT	*fl_create_timer(int, float, float, float, float, char []);
FL_OBJECT	*fl_add_timer(int, float, float, float, float, char []);

void	fl_set_timer(FL_OBJECT *, float);
float	fl_get_timer(FL_OBJECT *);


#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
}
#endif

#endif	/* define __FORMS_H__ */

