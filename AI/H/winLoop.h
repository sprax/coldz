/*****************************************************************************
 winLoop.h    S.R.Lines    94.8.29
#include        "mj.h"	-- for Dmj struct (dimensions of an mj image) 
 *****************************************************************************/
#ifndef winLoop_h__
#define winLoop_h__

extern	float	Zoom;
extern	int	winLoopSgi(unt flags);

#ifdef  SGI
#define winLoop	winLoopSgi
#define WL_RESOURCE_NAME	"win loop"
#endif

#ifdef  WIN32   // hack fixme @@
#define winLoop	winLoopSgi
#define WL_RESOURCE_NAME	"win loop"
#endif

#define MCHAN    4
#define TLEN   128
#define STOPPED         1
#define LOOPING         2
#define SWINGING        3
#define FORBACK         4
#define MAPCHAIN        5
#define SLIDERS         6
#define ZOOMER          7
#define MOUSEPOS        8
#define MOUSE_X         9
#define MOUSE_XY       10
#define MOUSE_XYZ      11
#define LABEL_FACE     12

#define FORWARD         0
#define BACK            1

/*_____ pseudodevices: user-defined range: 0x1000 - 0x7FFF (man qenter) */
#define	WL_RETURN	0x1002	/* queue this to break from winLooper */
#define	WL_NOTHING	0x1004	/* queue this to kick qread out of waiting */

typedef struct  WmjAct {
        void  (*func)(void *gwp, int dev, int flags);	/*gwp:graph win ptr*/
        void   *data;	/* typically shared data, eg, the App struct */
	int	nA;	/* eg, array indices in (App*)data */
	int	nB;
	int	nC;
}       WmjAct, *pWmjAct;

typedef struct  WmjMnu {
        void  (*func)(void *gwp, int dev, int flags);   /*gwp:graph win ptr*/
        void   *data;   /* typically shared data, eg, the App struct */
	int	id;	/* menu id returned by defpup (never 0 ?)    */
        int     nA;     /* eg, array indices in (App*)data */
        int     nB;
        int     nC;
}       WmjMnu, *pWmjMnu;

typedef struct  Vlst { /* Vertex List */
    int     stat;      /* Current state or whatever; init to 0 (calloc)  */
    int     nvts;      /* Length of glxy, ie, number of xy-pairs.        */
    short  *vlst;      /* OR: Coord **glxy; or even Coord glxy[][2];     */
    punl    rgba;      /* Could be cpacked color for each xy, texel, etc */
    int   (*glfn)(void *vl); /* Thus:  vl->stat = vl->glfn(W|vl);     */
    struct Vlst *next;
}   Vlst,  *pVlst;

typedef struct  Glst {  /* Graphics List: ptrs to functions & data for GL */
	int	stat;	/* Current state or whatever; init to 0 (calloc)  */
	int	glen;	/* Length of glxy, ie, number of xy-pairs.        */
	pfl	glxy;	/* OR: Coord **glxy; or even Coord glxy[][2];	  */
	punl	rgba;	/* Could be cpacked color for each xy, texel, etc */
	int	(*glfn)(void *gp); /* Thus:  gp->stat = gp->glfn(gp);     */
}       Glst, *pGlst;

typedef struct  Wimj {
        long    id;	/* window id returned by winopen */
	long	in;	/* image number, or index in app data array */
        long    wx,wy;  /* window coord positions for GL prefposition x1,y1*/
        int     x,y;
        unt     w,h;
        Dmj     D;
        ppu     U[MCHAN];
        ppf     F;
        punl    P;      /* unsigned long *p; for 4-byte/pixel cpacked data */
        char   *T;	/* window title */
        char   *ti;     /* window icon title -- usually accept default */
        flt	zX, zY;	/* X and Y zoom factors (for rectzoom)  */
	pGlst	G;
/*	pVlst	V; 	*/
	WmjAct *lefMouse;
	WmjAct *midMouse;
	WmjAct *rigMouse;	/* usually menuMouse */
	WmjAct *act;
	WmjMnu *mnu;
struct	Wimj   *prev;
struct	Wimj   *next;
        void  (*draw)(struct Wimj *v, int dev, int flags);
}       Wimj,  *pWmj;

#define MWIND   512	/* @@ KLUGE: Should be more like < 64 */
extern pWmj     Wj[];	/* @@ really want to export this? */
/* static  pWmj    Wj[MWIND]; -- not exported */
/* static  int     WnNum; current number of allocated Wimj's */

extern  long WLmenu;
extern  long WLmode;
extern  long WLmodeDef;

extern  void (*WLstep)(void);
extern  void (*WLswing)(void);
extern  void (*WLmouseX)(pWmj W, int dev, int val);
extern  void (*showSomeGL)(pWmj W);


extern short   gRED[3];	/* { 255,  0,  0 }; */
extern short   gGRN[3];	/* {   0,255,  0 }; */
extern short   gBLU[3];	/* {   0,  0,255 }; */
extern short   gYEL[3];	/* { 255,255,  0 }; */
extern short   gCYN[3];	/* {   0,255,255 }; */
extern short   gMAG[3];	/* { 255,  0,255 }; */
extern short  *gCLR[6]; /* { gRED,gGRN,gBLU,gYEL,gMAG,gCYN }; */


        /* Opaque Packed Colors (alpha = 128/255 or 0.5) */
#define FcBLK	0xFF000000
#define FcRED	0xFF0000FF
#define FcGRN	0xFF00FF00
#define FcBLU	0xFFFF0000
#define FcYEL	0xFF00FFFF
#define FcMAG	0xFFFF00FF
#define FcCYN	0xFFFFFF00
#define FcWHT	0xFFFFFFFF

        /* Half Transparent Packed Colors (alpha = 128/255 or 0.5) */
#define HcBLK	0x7F000000
#define HcRED	0x7F0000FF
#define HcGRN	0x7F00FF00
#define HcBLU	0x7FFF0000
#define HcYEL	0x7F00FFFF
#define HcMAG	0x7FFF00FF
#define HcCYN	0x7FFFFF00
#define HcWHT	0x7FFFFFFF

extern unsigned long  FcCLR[6]; /* { FcRED,FcGRN,FcBLU,FcYEL,FcMAG,FcCYN }; */
extern unsigned long  HcCLR[6]; /* { FcRED,FcGRN,FcBLU,FcYEL,FcMAG,FcCYN }; */


extern  pWmj Wnew (pDmj D, char *winTitle, float zoomx, float zoomy, unt flags);
extern	pWmj Wclone(pWmj W, int flags);
extern	pWmj Wreg (long wid, pDmj D, char *winTitle, float zX, float zY
			, unt flags);

extern  void WdrawP (pWmj W);
extern  void Wdraw (struct Wimj *W, int dev, int flags);

extern	void saveRgbGL(pWmj W, unt flags);

extern  void mjPackUnc (punl ulp, ppu src[], pDmj D);
extern  void mjPackFltI(punl ulp, ppf row,   pDmj D);

extern	int drawList(void *p); /*Example function for G->glfn in winLoopSgi.c*/
                               /*1st line: pGlst G = (pGlst)p; */
extern  int drawThis(void *p); /* Example function for G->glfn */

pWmj showMjU ( ppu U[], pDmj D, pGlst G
             , char *winTitle, unt flags);

pWmj showU ( ppu U[], int sX, int sY, unt sW, unt sH, unt sD
           , pGlst G, char *winTitle, unt flags);
pWmj showF ( ppf F, int sX, int sY, unt sW, unt sH, unt sD
           , pGlst G, char *winTitle, double zoom, unt flags);
pWmj showFm( ppf F, pDmj Dm, char *winTitle, flt zoom, unt flags);
pWmj showSub (pWmj S, int xi, int xf, int yi, int yf
	, char *winTitle, dbl zoomX, dbl zoomY, unt flags);
pWmj
showPm(unl *P, pDmj Dm, char *winTitle, float zoomX, float zoomY, unt flags);

extern int checkScreenSize(int xsize, int ysize);

extern int rubberCirc (float radius);
extern int rubberRectMag (void *v, int device, int flags);


void drawCrossCtr (double xx, double yy, double rad);
void drawTet (float vtx[][2]);

int followFlow (void *v, int device, int flags);

#endif	/*winLoop_h__*/


