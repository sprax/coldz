/*
 * structures.h
 */

#ifndef _STRUCTURES_H
#define _STRUCTURES_H

#define NEWKEY      500
#define NEWCOLOR    501
#define UPDATE      502
#define ATTRIBUTES  503
#define DESELECT    504
#define SELECT      505
#define SWITCH_ICON 506
#define NEW_NUMBER  507
#define OK          508
#define CANCEL      509
#define CHANGECOLOR 510
#define CHANGETRANS 511
#define CHANGEFONT  512
#define CHANGESTRING 513
#define CHANGELEADING 5001
#define ASSIGN_PALETTE 514
#define SETWORDSPACE 515
#define SETCHARSPACE 516
#define NEWTRICOLOR  517
#define GETWORDCOORDS 518
#define SCROLLUP     519
#define SCROLLDOWN   520
#define CHANGECOLOR_NOINSTALL 521
#define FLIP_BEVEL   522	/* DKY */
#define GETCOLOR     523	/* DKY */
#define CHANGE_X_POS 524	/* DKY */
#define CHANGE_Y_POS 525	/* DKY */
#define SET_SIZE     526	/* DKY */
#define GET_STRINGWIN_COLOR 527	/* DKY */
#define PRINTSTRING  528
#define CHANGE_BAR_BORDER_COLOR     529 /* DKY */
#define CHANGE_BAR_COLOR            530	/* DKY */
#define CHANGE_BAR_AND_BORDER_COLOR 531	/* DKY */
#define CHANGE_CURVE 532	        /* DKY */

#define CHANGE_BASE_COLOR          533 /* DKY */
#define CHANGE_HEADLINE_COLOR      534 /* DKY */
#define TR_CHANGETRANS             535 /* MAF */
#define CHANGE_INPUTWIN_BASE_COLOR 545 /* DKY */
#define CHANGE_INPUTWIN_TEXT_COLOR 546 /* DKY */

#define STRINGWIN                  547 /* Tim */
#define PARAGRAPHWIN               548 /* Tim */

#define CHANGEDESTINATION          549 /* Tim */

#define FREESTRING                 550 /* DKY */
#define CHANGE_REGISTER_FONT_COLOR 551 /* BDD */

#define SWITCH_ICON_NODRAW         552 /* BS  */
#define ICON_NEWCOLOR              553 /* DKY */
#define SET_SHADOW                 554 /* DKY */
#define NEWBARCOLOR                555 /* DKY */

#define NEWVALUE                   556 /* DB */
#define ORDER                      5560 /* Tim  5/13/91, for TileChildren */
#define CHANGE_BORDER              557 /* Lindi */
#define SET_LIGHT                  558 /* *DKY-25Mar91* */

#define CHANGE_UL_ROUND 	570 /* cmk */
#define CHANGE_UR_ROUND 	571 /* cmk */
#define CHANGE_LR_ROUND 	572 /* cmk */
#define CHANGE_LL_ROUND 	573 /* cmk */

#define NEWKNOBS      559       /* Tim 3/26/91 */
#define SETKNOB       560       /* Tim 3/27/91 */
#define UNSETKNOB     561       /* Tim 5/11/91 */

#define DBUFFER_ON    562       /* SL 9/19/91 */
#define DBUFFER_OFF   563       /* SL 9/19/91 */

/* Messages accepted by TransShadowWin   SL  11/8/91 */
#define SHADOW_UPDATE        564
#define SHADOW_OFF           565
#define SHADOW_ON            566
#define CHANGE_SHADOW_TRANS  567
#define CHANGE_SHADOW_COLOR  568
#define CHANGE_SHADOW_OFFSET 569
#define CHANGE_SHADOW_POS    570

/* Messages accepted by Patternify       SL  2/14/92 */
#define PATTERN_RELATIVE     571

#define TML_WORDWRAP 572
#define GETSTRING    573                 /* SL 6/24/92 */

#define CENTERED    -9999
#define NOMAKE      -9998

#define redclear 2

#define BETWEEN(X,A,B) (( A <= X ) && ( X <= B ) )
#define FCON(COLOR) ((float)(COLOR)/255.0)
#define CHECKFILE(FILENAME) (fclose(fopen(FILENAME,"r")) != EOF)
#define STRING(x) (x ? x : "(NULL STRING)")
#define SWAP(A,B,T) (T = A, A = B, B = T)
#ifndef ABS
#define ABS(x) ((x)  > 0 ? (x) : - (x))
#endif


struct curstruct {
  int id, dx, dy, hotdx, hotdy, rule;
  unsigned char *source;
};

struct didierstruct {
  int id, dx, dy, hotdx, hotdy, rule;
  unsigned char *source; 
  unsigned char *mun;
  float hl, hh, vl, vh, cl, ch;
};



/* This is the old wrapstruct 

  struct wrapstruct {
     struct curstruct *echo;
     int (**functions)();
     char **arguments;
     int numfuncs, whenmask, now, trapflag;
  };

*/

struct wrapstruct {
  struct curstruct *echo;
  int whenmask, now, trapflag;
};

struct messagestruct {
   struct Window *W;
   int id, mask;
   char *data;
};

struct stackstruct {
  char **top, **max;
  char **stack;
};

struct stepperstruct
       {
       struct Window *pad,*slider;
       int line,max,min,area;
       };

struct suckstruct {
  struct Window *closewin, *openwin;
};

struct function_struct {
  int (*function)(), mask;
  char *argument;
};

struct m_function_struct {	/* for MFunctionCaller() - *DKY-13Feb91* */
  int (*function)(), mask;
  int numArgs;
  char *args[10];
};


struct rectstruct {
  int red, green, blue;
  int bred, bgreen, bblue;
  int border, shadow;
};

struct HatchRectStruct {	/* *DKY-17Feb91* */
  int type, hatchR, hatchG, hatchB, shadow, border, spacing, xvector, yvector;
};

struct bevelstruct
{
  int red,green,blue,border,bred,bgreen,bblue,shadow;
  int lred,lgreen,lblue,rred,rgreen,rblue,bevel;
  int light;			/* *DKY-25Apr90* */
};


struct transrectstruct {
  int red, green, blue;
  int bred, bgreen, bblue;
  int border, shadow,trans;
};

struct roundrectstruct {
  int red, green, blue, shadow, shadowred, shadowgreen, shadowblue;
  float curvature;
};

struct cornerrectstruct 
{
  int red, green, blue, shadow, shadowred, shadowgreen, shadowblue;
  int ul_curve, ur_curve, lr_curve, ll_curve;
};

struct washrectstruct {
  int ulred, ulgreen, ulblue, llred, llblue, llgreen, urred, urgreen, urblue, lrred, lrgreen, lrblue;
  int shadow, shadowred, shadowgreen, shadowblue, border;
};

struct grad_data {   /*-DB-1/5/91-*/
  float *redvals, *greenvals, *bluevals;
  /* refers to 3 arrays of 255 float numbers between 0 and 255 inclusive */
};

struct gradrectstruct {  /*-DB-1/5/91-*/
  struct rectstruct *rect;
  struct grad_data *grad;
};

struct trirectstruct {
  int red, green, blue;
  int bred, bgreen, bblue;
  int trired, trigreen, triblue;
  int direction;
  int border, shadow;
};

struct stringstruct {
  int x,y;
  char *string;
  char *font;
  int size;
  int bits;
  int red, green, blue, t;
  int char_space, word_space;
};

struct rstringstruct {   
  /*** same as above but use font characteristics of another window ***/
  char *string;
  struct stringstruct *parms;
  int x,y;
};

struct adaptive_stringstruct {   /*DB*/
  struct stringstruct *parms;
  float contrast;
  int munsellflag;
};

struct adaptive2_stringstruct {   /*DB*/
  struct stringstruct *parms;
  char *color_name;
};

				/* moved from popup.c *DKY- 8Mar91* */
struct popup_struct {           /* structure with info for a popup                      */
  int mask;                     /* mask that tells which sides of the window are active */
  int line_height;              /* height of menu lines in pixels                       */
  int x1, y1, x2, y2;           /* control rect. x1 = -1 for no control rect            */
  struct stringstruct parms;    /* font information                                     */
  struct curstruct *echo;
};


struct pop_trigger {		/* moved from popup.c *DKY-11Mar91* */
  struct Window *menu;
  int x, y;
  int callFunction;		/* new *DKY-11Mar91* -- do we call the item's function when justup? */
};

struct ezslidestruct {
  int red, green, blue, shadow, shadowred, shadowgreen, shadowblue;
  int barred, bargreen, barblue, kind, orient;
  char *max, *number, *min, *area;
  int oldIntValue;
  float oldFloatValue;
};

struct indentedezslidestruct {	/* *DKY-24Jul90* */
  int red, green, blue, shadow, shadowred, shadowgreen, shadowblue;
  int barred, bargreen, barblue, kind, orient;
  int barborderred, barbordergreen, barborderblue;
  int indent;
  char *max, *number, *min, *area;
  int oldIntValue;
  float oldFloatValue;
};

struct equilibriumstruct {
  int num_sliders, kind, orient;
  int red, green, blue, shadow, shadowred, shadowgreen, shadowblue;
  int barred, bargreen, barblue;
  char *max, *min, *number, *total;
  struct Window *controller;
};

struct snapstruct {
  struct Window *movewin;
  int hgrid;
  int vgrid;
};

struct domovestruct {
  struct curstruct *cursor;
  struct Window *movewin;
  char *fname;
};

struct dosnapmovestruct {
  struct curstruct *cursor;
  struct Window *movewin;
  char *fname;
  int hgrid, vgrid;
};

struct GracefulMoverStruct {	/* *DKY-13Sep91* */
  struct Window *windowToMove;
  int lastX, lastY;
};

struct resizestruct {
  struct Window *movewin;
  int xmin, ymin, xmax, ymax;
};

struct toggle_struct {
  /*** used in Toggler ***/
  int mask, *num;
};

struct borderstruct		/* *DKY-17Feb91* */
{
  int red, green, blue, indent, thickness;
};

struct makestruct {            
  /*** used in maker ***/
  int x1, y1, x2, y2;
};

struct permakestruct {
  /*** used in permaker ***/
  float x1, y1, x2, y2;
};

struct funcstruct {
  /*** each line in a pop-up has one o' these ***/
  int (*function)();
  char *arg;
  int highlightMenuLine;	/* if true, then the line is highlighted when the pen is over it.  *DKY-12Mar91* */
};

struct m_funcstruct {	     /* for AddMPopupLine() - JRG 6/3/92 */
  int (*function)();
  int numArgs;
  char *args[10];
  int highlightMenuLine;
};



struct StuffStruct {
  /*** something Dave made ***/
  struct Window *root;
  struct curstruct *echo;
};


struct transhadowStruct {          /* SL  11/9/91  */
  struct Window *vertShadow,
                *horizShadow;
  int isShadowOn,      /* TRUE or FALSE */
      transparency,    /* 0 (opaque) to 255 (invisible) */
      offset,          /* 1 to the minimum of the shadowed window's x and y dimensions */
      position;        /* UP_AND_RIGHT, UP_AND_LEFT, DOWN_AND_RIGHT or DOWN_AND_LEFT */
  int lastX1, lastY1,
      lastX2, lastY2;  /* Used to check if a window has moved or resized */
};




struct DoPatternStruct
{
  struct curstruct *pattern;
  int isRelative;             /* SL 2/14/92 */
};

/*****************************************************
  This had been commented out.  I'm not sure why,
I think I did it.  I uncommented it.  I hope this is OK.
-dave july 25, 1990
******************************************************/
/*
struct NoteDrawstuffStruct {
  struct Window *FixW, *pallette; 
  struct curstruct *echo, *pencil, *marker, *brush;
  struct curstruct *eraser, *spray, *hand, *mini;
  int  trans;
  int  back_red, back_green, back_blue;
  float  thick;
  int  wd, ht;
  int  draw_mode, curs_type;
  unsigned char *r_bank, *g_bank, *b_bank;
  unsigned char *r_paper, *g_paper, *b_paper;
  int pattern_wd, pattern_ht;
  unsigned char *r_pattern, *g_pattern, *b_pattern;
  FILE *dribble;
};
*/
/*play button's dostuff*/
struct dynastruct
        {
        struct curstruct *cursor;
	struct List *port;
        };

/* for dynamic objects */
struct infostruct
        {
        struct curstruct *cursor;
	int current_int;
	char *data_file;
        };


struct IndicatorStruct {
int stat, r, g, b, thick;
};

struct listwinstruct {
  struct Window *Root;
  struct curstruct *echo;

  char **list;
  char *listbase;
  
  int no_of_items;

  char *displayfont;
  int size;
  int bits;
  
  int x,y;
  int c1,c2,c3,b1,b2,b3,t1,t2,t3,h1,h2,h3;
};


struct parastruct {
  int offx, offy;
  char *para;
  char *font;
  int size;
  int bits;
  int top;
  int leading;
  int red, green, blue, t;
  int height;
};

struct picfilestruct {
  char *fileR, *fileG, *fileB;
  int fw;
  int picx, picy;
  int picw, pich;
  int posx, posy;
};

struct picfilerectstruct {
  char *fileR, *fileG, *fileB;
  int fw;
  int picx, picy;
  int picw, pich;
  int posx, posy;
  unsigned char r, g, b;
  int bred, bgreen, bblue;
  int border, shadow;
};

struct picturefilestruct {
  char *file;
  int fw, fh;
  int picx, picy;
  int picw, pich;
  int posx, posy;
  int format;
  int t;
};

struct transp_icon_struct
{
  struct curstruct *icon;
  int transp;
};

/*** globals for regular window system, found in globals.c ***/

extern struct Window *CurrentTextWindow;         
/* current window to receive keyboard input */

extern struct Window *screen_toggle;
extern struct Window *Root1,*Root2;
extern struct OutDev *Output1, *Output2;
extern struct InDev *Input1, *Input2;

extern char *query_return;

extern float CurrentHue, CurrentSat, CurrentVal;

extern int CurrentRed,CurrentGreen,CurrentBlue;

extern int E_lastx,E_lasty;  /* last x and y mouse positions */
extern int e_value;
extern struct Window *E_lastRoot;

extern int (*set_new_color)();
/* this function gets called whenever a new color is chosen */


#endif

