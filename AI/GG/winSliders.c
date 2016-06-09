/*****************************************************************************
 winSliders.c    (< cedit.c, Paul Haeberli - 1984)    S.R.Lines    93.11.10
 SGI: cc -float -prototypes -I/home/vu/spraxlo/c/h -DTEST_ME glc.c \
      -L/home/vu/spraxlo/bin/IRIS -limage -lgutil -lgl_s -lm -lc_s -g -o glc
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <gl/gl.h>
#include <gl/device.h>
#include <SGI/port.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <m/mj.h>
#include        <winLoop.h>
#include <winSliders.h>

#define ANY_MOVED	1
#define TWO_MOVED	2
#define ALL_MOVED	3
#define OK_BUTTON	(-1)

#define SX_RED (0.0)
#define SX_GRN (2.0)
#define SX_BLU (4.0)
#define TX_RED (1.5)
#define TX_GRN (3.5)
#define TX_BLU (5.5)

#define S_TOP	9.4F
#define S_BOT	1.6F

#define CLIP(lo,cv,hi) if (cv < lo) cv = lo; else if (cv > hi) cv = hi;

extern	Colorindex rgb(float r, float g, float b);	/* libgl */
extern	void	gamgetmcolor(int index, short *r, short *g, short *b); /* gamma.c */
extern	void	setcolorsys(int sys);	/* libgutil: colormod.c */
extern	void 	fromrgb(float r, float g, float b, float *tr, float *tg, float *tb);
extern	long	grey(float fg);		/* libgutil: vcolor.c */
extern	void	torgb(float r, float g, float b, float *tr, float *tg, float *tb);
extern	void	rgb_to_irgb(float r, float g, float b, int *ir, int *ig, int *ib);
extern	void	irgb_to_rgb(int ir, int ig, int ib, float *r, float *g, float *b);

static  double	tmpR, tmpG, tmpB, oldR, oldG, oldB, newR, newG, newB;
static	long	xorg, yorg;
static	long	xsize, ysize;
static	int	toggleInt;
static  int     chgR = 0, chgG = 0, chgB = 0, changed = 0;
#define SS_SIZE 256
static  char	ss[SS_SIZE];
#ifdef  	TEST_ME
#include	<stdlib.h>

void rgbCallBackEx(double red, double grn, double blu)
{
    fprintf(stderr, "rgbCallBackExample:  %.3f  %.3f  %.3f\n", *red, *grn, *blu);
}

void main(int argc, char **argv)
{
    double newR, newG, newB;
    if (argc > 1) setcolorsys(atoi(argv[1]));
    fprintf(stderr, "\n%s: testing gl_sliders_rgb\n", argv[0]);
    newR = newG = newB = 0.5;
    winSlidersRgb(&newR, &newG, &newB, "glSlideTest", rgbCallBackEx, NULL, 0);
}
#endif


void modgetrgb(double *dr, double *dg, double *db)
{
    short cr, cg, cb;
    float fr, fg, fb, r, g, b;
    gamgetmcolor(0, &cr, &cg, &cb);
    irgb_to_rgb(cr, cg, cb, &fr, &fg, &fb);
    fromrgb(fr, fg, fb, &r, &g, &b);
    *dr = r, *dg = g, *db = b;
}


void
drawOK(unsigned long clr)
{
    char	*txt = "OK";
    int 	pixlen;
    float	flen;

    cpack(clr);
    rectf(7.0F, S_BOT, 9.0F, S_TOP);
    grey(0.0F);
    pixlen = strwidth(txt);
    flen = (pixlen*10.0) / xsize;
    cmov2(8.0F - flen / 2.0F, 5.2F);
    charstr(txt);
    rect(7.0, S_BOT, 9.0, S_TOP);
}


void centernum(float val, float x, float y)
{
    char buf[128];
    int pixlen;
    float flen;

    grey(0.8F);
    rectf(x - 1.0F, y - 0.3F, x + 1.0F, y + 1.1F);
    grey(0.0);
    if (toggleInt)
        sprintf_s(buf, 128, "%d", ((int)(val * 255)));
    else
        sprintf_s(buf, 128, "%.2f", val);
    pixlen = strwidth(buf);
    flen = (pixlen*10.0F) / xsize;
    cmov2(x - flen / 2.0F, y);
    charstr(buf);
}


void drawknob(unsigned long clr, int mark, float tmp, float old, float new, float xpos)
{
    pushmatrix();
    translate(xpos, 2.0F + (7.0F*tmp), 0.0F);	/* (re)erase tmp (last drawn) knob location */
    grey(0.9F);
    rectf(1.13F, -0.12F, 1.92F, 0.12F);
    popmatrix();
    if (mark) {
        pushmatrix();
        translate(xpos, 2.0F + (7.0F*old), 0.0F);	/* (re)mark old (last used) knob location */
        grey(0.6F);
        rectf(1.13F, -0.12F, 1.92F, 0.12F);
        popmatrix();
    }
    pushmatrix();
    translate(xpos, 2.0F + (7.0F*new), 0.0F);	/* (re)draw new (provisional) knob location */
    cpack(clr);
    rectf(1.15F, -0.10F, 1.9F, 0.10F);
    grey(0.0);
    rect(1.15F, -0.10F, 1.9F, 0.10F);
    popmatrix();
}


void drawslider1(unsigned long clr, int mark, double tmp
    , double old, double new, double xpos, double tpos)
{
    drawknob(clr, mark, (float)tmp, (float)old, (float)new, (float)xpos);
    centernum((float)new, (float)tpos, 0.45F);  /* any higher occludes bottom of slider */
}


static void
drawsliders(int mark, int olds)
{ /* always erase old, sometimes remark old, and always draw new */
    unsigned long clr;
    drawslider1(FcRED, mark, oldR, oldR, newR, SX_RED, TX_RED);
    drawslider1(FcGRN, mark, oldG, oldG, newG, SX_GRN, TX_GRN);
    drawslider1(FcBLU, mark, oldB, oldB, newB, SX_BLU, TX_BLU);
    if (olds)
        clr = rgb((float)oldR, (float)oldG, (float)oldB);
    else
        clr = rgb((float)newR, (float)newG, (float)newB);
    drawOK(clr);
}

void makeback(float x)
{
    pushmatrix();
    translate(x, 0.0, 0.0);
    grey(0.9F);
    rectf(1.0F, (Coord)S_BOT, 2.0F, (Coord)S_TOP);
    grey(0.0F);
    rect(1.0F, (Coord)S_BOT, 2.0F, (Coord)S_TOP);
    popmatrix();
}



static void
winSlidersDraw(int mark, int olds)
{
    reshapeviewport();
    getorigin(&xorg, &yorg);
    grey(0.8F);
    clear();
    ortho2(0.0F, 10.0F, 0.0F, 10.0F);
    grey(0.0F);
    makeback(SX_RED);
    makeback(SX_GRN);
    makeback(SX_BLU);
    drawsliders(mark, olds); /* may (re)draw marks, even if they'll be covered */
}


static void
winSlidersDraw11(struct Wimj *v, int dev, int flags)
{
    /*
    if (dev == REDRAWICONIC)
      winset(InSlidersWid); prefsize(100,100); winconstraints();
    */
    winSlidersDraw(1, 1); 	/* @@ replace newR w/ global InSlideRed, etc */
}


void
winSlidersUpdate(double red, double grn, double blu, int mark, int olds)
{
    long	wid = winget();
    winset(InSlidersWid);
    newR = red, newG = grn, newB = blu;
    winSlidersDraw(0, 0);
    tmpR = oldR = newR, tmpG = oldG = newG, tmpB = oldB = newB;
    winset(wid);
}


/* __________________________ NEW SLIDER FUNCS ____________________*/


typedef struct  sliD {
    double  R;
    double  G;
    double  B;
    void(*rgbCallBack)(double R, double G, double B);
    int     when;
    char    *wTitle;
}       sliD;


static void
slideMenuFunc(void *v, int device, int flags)
{
    pWmj    wS = (pWmj)v;
    pWmjMnu mS = wS->mnu;
    sliD   *dS = (sliD *)mS->data;
    char   *wTitle = dS->wTitle;
    long    pval;
    static  char   *mstrSlide =
        "WHEN: %t|Any Moved|Two Moved|All Moved|OK Button|Fixed Incr|Return|Quit";

#define RETITLE(S)      sprintf_s(ss, SS_SIZE, "%s: %s",wTitle,(S)); wintitle(ss)

    if (!mS->id) mS->id = defpup(mstrSlide);

    switch (pval = dopup(mS->id)) {
        case  1: dS->when = ANY_MOVED;  RETITLE("ANY_MOVED"); break;
        case  2: dS->when = TWO_MOVED;  RETITLE("TWO_MOVED"); break;
        case  3: dS->when = ALL_MOVED;  RETITLE("ALL_MOVED"); break;
        case  4: dS->when = OK_BUTTON;  RETITLE("OK_BUTTON"); break;
        case  5: toggleInt = !toggleInt;                  break;
        case  6: winclose(wS->id);  /* winset(mS->nA); */ break;
        case  7: gexit(); exit(1);                        break;
        case -1: /* no selection */                       break;
        default: fprintf(stderr, "slideMenuFunc: unknown option: pupval:%d\n", pval);
    }
    /* return dS->when; */
}



static void
slideActFunc(void *v, int device, int mouseDown)
{
    pWmj    wS = (pWmj)v;
    pWmjAct aS = wS->act;
    sliD   *dS = (sliD *)aS->data;
    int     when = dS->when;
    int     xx, yy;
    float   wx, wy;

    if (!mouseDown) return;

    getorigin(&xorg, &yorg);
    getsize(&xsize, &ysize);	/* necessary? @@  */


    switch (device) {
        case MIDDLEMOUSE:	wrn("slideActFunc: middle mouse same as left mouse");
        case LEFTMOUSE:
            xx = getvaluator(MOUSEX);
            yy = getvaluator(MOUSEY);
            wx = 10.0F*(xx - xorg) / xsize;
            wy = 10.0F*(yy - yorg) / ysize;
            if (wy > 0.0 && wy < 10.0) {
                if (wx > 1.0 && wx < 2.0) {
                    newR = (wy - 2.0) / 7.0;
                    CLIP(0.0, newR, 1.0);
                    if (newR != tmpR) {
                        chgR = 1, changed = chgR + chgG + chgB;
                        drawslider1(FcRED, (when != changed), tmpR, oldR, newR, SX_RED, TX_RED);
                        tmpR = newR;
                    }
                }
                else if (wx > 3.0 && wx < 4.0) {
                    newG = (wy - 2.0) / 7.0;
                    CLIP(0.0, newG, 1.0);
                    if (newG != tmpG) {
                        chgG = 1, changed = chgR + chgG + chgB;
                        drawslider1(FcGRN, (when != changed), tmpG, oldG, newG, SX_GRN, TX_GRN);
                        tmpG = newG;
                    }
                }
                else if (wx > 5.0 && wx < 6.0) {
                    newB = (wy - 2.0) / 7.0;
                    CLIP(0.0, newB, 1.0);
                    if (newB != tmpB) {
                        chgB = 1, changed = chgR + chgG + chgB;
                        drawslider1(FcBLU, (when != changed), tmpB, oldB, newB, SX_BLU, TX_BLU);
                        tmpB = newB;
                    }
                }
                else if (wx > 7.0 && wx < 9.0) {
                    if (wy > S_BOT && wy < S_TOP)
                        changed = OK_BUTTON;
                }
                if (changed == when && (chgR | chgG | chgB)) {
                    drawsliders(0, 0);
                    if (dS->rgbCallBack) dS->rgbCallBack(newR, newG, newB);
                    chgR = chgG = chgB = changed = 0;
                    dS->R = oldR = newR, dS->G = oldG = newG, dS->B = oldB = newB;
                }
            }
            break;

        default:            wrn("slideActFunc: NO ACTION for device %d", device); break;
    }
}


void
winSlidersRgb(double *pR, double *pG, double *pB, char *wTitle
    , void(*rgbCallBack)(double R, double G, double B)
    , void(*redrawMain)(long wid), long widMain, int get_rgb)
{
    static  WmjAct  slideAct;
    static  WmjMnu  slideMnu;
    static  sliD    slideData;
    pWmj    wS;
    long    wid;
    Dmj     cD;


    wid = winget();
    cD.x = cD.y = 0;  cD.w = cD.h = 250;  cD.d = 0;
    keepaspect(1, 1);
    prefsize(250, 250);
    iconsize(100, 100);
    /* noborder(); */
    InSlidersWid = winopen(wTitle);
    RGBmode();
    gconfig();
    if (get_rgb)
        modgetrgb(pR, pG, pB);
    getsize(&xsize, &ysize);

    cD.w = xsize, cD.h = ysize;
    wS = Wreg(InSlidersWid, &cD, wTitle, 1.0, 1.0, 0);
    wS->draw = winSlidersDraw11;

    tmpR = oldR = newR = *pR, tmpG = oldG = newG = *pG, tmpB = oldB = newB = *pB;
    winSlidersDraw(0, 0);

    slideData.R = newR;
    slideData.G = newG;
    slideData.B = newB;
    slideData.when = OK_BUTTON;
    slideData.wTitle = wTitle;
    slideData.rgbCallBack = rgbCallBack;

    slideMnu.data = &slideData;
    slideMnu.func = &slideMenuFunc;
    slideMnu.nA = widMain;
    wS->mnu = &slideMnu;
    slideAct.data = &slideData;
    slideAct.func = &slideActFunc;
    wS->act = &slideAct;

    winset(wid);
}



/*__________________ THE OLD WAY _________________________________*/


static int
winSlidersMenu(char *wTitle, long widMain, int when, int flags)
{
    long    pval;
    static  long    midSlide;
    static  char   *mstSlide =
        "WHEN: %t|Any Moved|Two Moved|All Moved|OK Button|Fixed Incr|Return|Quit";

    if (!midSlide) midSlide = defpup(mstSlide);

    switch (pval = dopup(midSlide)) {
        case  1: when = ANY_MOVED;  RETITLE("ANY_MOVED");	break;
        case  2: when = TWO_MOVED;  RETITLE("TWO_MOVED");	break;
        case  3: when = ALL_MOVED;  RETITLE("ALL_MOVED");	break;
        case  4: when = OK_BUTTON;  RETITLE("OK_BUTTON");	break;
        case  5: toggleInt = !toggleInt;                  	break;
        case  6: winclose(InSlidersWid); InSlidersWid = 0;  winset(widMain);  break;
        case  7: gexit(); exit(1);                       	break;
        case -1: /* no selection */                      	break;
        default: fprintf(stderr, "winSlidersMenu: unknown option: pupval:%d\n", pval);
    }
    return when;
}



void
winSlidersRgbO(double *pR, double *pG, double *pB, char *wTitle
    , void(*rgbCallBack)(double R, double G, double B)
    , void(*redrawMain)(long wid), long widMain, int get_rgb)
{
    short	rdev, qval;
    int 	xx, yy;
    int 	when = ANY_MOVED, chgR = 0, chgG = 0, chgB = 0, changed = 0;
    float	wx, wy;
    unsigned long clr;
    double	tmpR, tmpG, tmpB, oldR, oldG, oldB, newR, newG, newB;

    keepaspect(1, 1);
    prefsize(250, 250);
    iconsize(100, 100);
    /* noborder(); */
    InSlidersWid = winopen(wTitle);
    qdevice(MENUBUTTON);
    qdevice(MIDDLEMOUSE);
    qdevice(LEFTMOUSE);
    qdevice(WINFREEZE);
    qdevice(WINTHAW);
    qdevice(REDRAWICONIC); 	/* Done by winopen: qdevice(REDRAW); */
    RGBmode();
    gconfig();
    if (get_rgb) modgetrgb(pR, pG, pB);
    tmpR = oldR = newR = *pR, tmpG = oldG = newG = *pG, tmpB = oldB = newB = *pB;
    getsize(&xsize, &ysize);
    winSlidersDraw(0, 0);
    while (1) {
        /*************************
        if(mode == LOOPING)
          while(!qtest()) step();
        **************************/
        switch (rdev = (short)qread(&qval)) {
            case MENUBUTTON:
                if (qval) when = winSlidersMenu(wTitle, widMain, when, 0); break;
            case MIDDLEMOUSE:
            case LEFTMOUSE: if (qval) {
                xx = getvaluator(MOUSEX);
                yy = getvaluator(MOUSEY);
                wx = 10.0F*(xx - xorg) / xsize;
                wy = 10.0F*(yy - yorg) / ysize;
                if (wy > 0.0 && wy < 10.0) {
                    if (wx > 1.0 && wx < 2.0) {
                        newR = (wy - 2.0) / 7.0;
                        CLIP(0.0, newR, 1.0);
                        if (newR != tmpR) {
                            chgR = 1, changed = chgR + chgG + chgB;
                            drawslider1(FcRED, (when != changed), tmpR, oldR, newR, SX_RED, TX_RED);
                            tmpR = newR;
                        }
                    }
                    else if (wx > 3.0 && wx < 4.0) {
                        newG = (wy - 2.0) / 7.0;
                        CLIP(0.0, newG, 1.0);
                        if (newG != tmpG) {
                            chgG = 1, changed = chgR + chgG + chgB;
                            drawslider1(FcGRN, (when != changed), tmpG, oldG, newG, SX_GRN, TX_GRN);
                            tmpG = newG;
                        }
                    }
                    else if (wx > 5.0 && wx < 6.0) {
                        newB = (wy - 2.0) / 7.0;
                        CLIP(0.0, newB, 1.0);
                        if (newB != tmpB) {
                            chgB = 1, changed = chgR + chgG + chgB;
                            drawslider1(FcBLU, (when != changed), tmpB, oldB, newB, SX_BLU, TX_BLU);
                            tmpB = newB;
                        }
                    }
                    else if (wx > 7.0 && wx < 9.0) {
                        if (wy > S_BOT && wy < S_TOP)
                            changed = OK_BUTTON;
                    }
                    if (changed == when && (chgR | chgG | chgB)) {
                        drawsliders(0, 0); /* clear old, redraw new */
                        if (rgbCallBack) rgbCallBack(newR, newG, newB);
                        *pR = oldR = newR, *pG = oldG = newG, *pB = oldB = newB;
                        chgR = chgG = chgB = changed = 0;
                    }
                }
            } break;
            case WINTHAW:             /* Easy, cuz here winget() always == qval */
                reshapeviewport();
                /* no break; */
            case REDRAW:		/* redraw marks (all old) and new */
                if (qval == InSlidersWid)
                    winSlidersDraw(1, 1);
                if (redrawMain)
                    redrawMain(widMain);
                break;
            case REDRAWICONIC:
                /* fprintf(stderr,"REDRAW ICONIC\n"); */
                reshapeviewport();
                clr = rgb((float)newR, (float)newG, (float)newB);
                cpack(clr);
                clear();
                break;
        }
    }
}


