/*****************************************************************************
 winLoopSgi.c    S.R.Lines    93.8.17-9.21
 *****************************************************************************/
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <malloc.h>
#include        <assert.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unMacro.h>
#include        <un/unMem.h>
#include        <SGI/image.h>
#include        <SGI/dispimg.h>
#include        <SGI/miscgl.h>
#include        <SGI/row_gutil.h>
#include        <gl/gl.h>
#include        <gl/get.h>
#include        <gl/device.h>
#include	<m/mj.h>
#include	<m/mjPack.h>
#include        <m/rowCpack.h>
#include        <m/rgbMark.h>
#include	<winLoop.h>
#include	<winDraw.h>
#include        <un/unFlags.h>

void makeGridTet (Wimj *v, int ycells, int xcells, unt flags);
#define showSubCAST(S,xi,yi,xf,yf,wT,zX,zY,f) \
    showSub(S,(int)(xi),(int)(yi),(int)(xf),(int)(yf),wT,(dbl)(zX),(dbl)(zY),f)


#define DEBUG      1
#define SLEN     256

static int	WnNum=0;		/* Global number of allocated Wimj's */
static unt	WIDE = 1280;		/* should test in init_GL_config */
static unt	TALL = 1024;
static char	Ss[SLEN];

long     WLmenu    = 0;
long     WLmode    = MOUSEPOS;
long     WLmodeDef = MOUSEPOS;

pWmj		Wj[MWIND];	/* Wj[i] is a window (ID=i) w/ attached mj */
float		Zoom = 1.0;
short	gRED[3] = { 255,  0,  0 };
short	gGRN[3] = {   0,255,  0 };
short	gBLU[3] = {   0,  0,255 };
short	gYEL[3] = { 255,255,  0 };
short	gMAG[3] = { 255,  0,255 };
short	gCYN[3] = {   0,255,255 };
short  *gCLR[6] = { gRED,gGRN,gBLU,gYEL,gMAG,gCYN };

	/* Fully opaque packed colors (alpha = 128/255 or 0.5) */
unsigned long  FcCLR[6] = { FcRED,FcGRN,FcBLU,FcYEL,FcMAG,FcCYN };
unsigned long  HcCLR[6] = { HcRED,HcGRN,HcBLU,HcYEL,HcMAG,HcCYN };

	/* Half-transparent packed colors (alpha = 128/255 or 0.5) */

#define  MAX_OVER 2           /* valid values are 2,4,8 */
/* default colors in overlay */
static int overrgb[16][3] =
{	/* First 8 in same order as gl.h */
    {0  , 0  ,   0},  /* black = none	= erase	*/
    {255, 0  ,   0},  /* red    */
    {0  , 255,   0},  /* green	*/
    {255, 255,   0},  /* yellow	*/
    {  0,   0, 255},  /* blue	*/
    {255,   0, 255},  /* magenta*/
    {  0, 255, 255},  /* cyan	*/
    {255, 255, 255},  /* white	*/

    {128, 128, 128},  /* gray	*/
    {128, 128, 128},  /* gray	*/
    {128, 128, 128},  /* gray	*/
    {128, 128, 128},  /* gray	*/
    {128, 128, 128},  /* gray	*/
    {128, 128, 128},  /* gray	*/
    {128, 128, 128},  /* gray	*/
    {128, 128, 128}   /* gray	*/
};
extern int force_pupdraw;       /* undocumented */

static long otherfb;            /* hardwhere supported non regular FB */

/***************************************************************
 * check if overlay exists and in case it doesn't, pupdraw will be used
 ***************************************************************/

static void
init_overlay(void)
{
long over_plane, pup_plane, pup_colors, overlay_colors = 0;
int i;

  over_plane = getgdesc(GD_BITS_OVER_SNG_CMODE);

  if (over_plane > 0) {
    over_plane = MIN(over_plane, MAX_OVER);
    overlay(over_plane);
    /* doublebuffer();	@@ sprax */
    overlay_colors = 1 << over_plane;

    /* install colormap in overlay only, don't mess with pupdraw */
    drawmode(OVERDRAW);
#if 1
    for (i = 0; i < overlay_colors; i++)
      mapcolor((unsigned short)i, (short)overrgb[i][0], (short)overrgb[i][1], (short)overrgb[i][2]);
#endif
    /*cmode();*/
    /*onemap();*/
    gconfig();
    drawmode(NORMALDRAW);
  }

  if ((pup_plane = getgdesc(GD_BITS_PUP_SNG_CMODE)) <= 0 && !over_plane)
    die("No Bitplanes for OVER and PUP !!");

  pup_colors = (1 << pup_plane);

  /* the real harware framebuffer used */
  otherfb = (overlay_colors > 1 ? OVERDRAW : PUPDRAW);
  /**
  wrn("init_overlay using %d bitplanes from %s",
         (otherfb == PUPDRAW) ? pup_plane : over_plane,
         (otherfb == PUPDRAW) ? "Pupdraw" : "Overlay");
  **/
}


void
init_GL_config(void)
{
static int init;
  if (!init) { init = 1; 
    subpixel(TRUE);
    /*init_cursor();*/
    init_overlay();
  }
}


int
rubberCirc (float radius)
{
long		orgX, orgY;
static float	newX, newY, oldX, oldY;

    getorigin(&orgX,&orgY);
    /*getsize(&xsize,&ysize);*/
    cursoff();
    while(getbutton(LEFTMOUSE)) {
      newX = (flt)(getvaluator(MOUSEX) - orgX);
      newY = (flt)(getvaluator(MOUSEY) - orgY);
      if (newX != oldX || newY != oldY) {
        drawmode(OVERDRAW);		/* If no OVERDRAW, use PUPDRAW */
        color(BLACK);			/* black erases */
        circ(oldX, oldY, radius);	/* draw over old circle */
        color(RED);			/* set color of new circle */
        circ(newX, newY, radius);	/* draw new circle */
        drawmode(NORMALDRAW);		/* return to main framebuffer (in RGB mode) */
        oldX = newX, oldY = newY;
      }
    }
    curson();
  return 1;
}


int
rubberRectZoom(void *v, int device, int izoom)
{
pWmj    wA = (pWmj)v;
/* WmjAct *Ap = (device == LEFTMOUSE ? wA->lefMouse : wA->midMouse); */
flt     zX = wA->zX, zY = wA->zY;
long    xb, yb, xn, yn, xo,yo, wx,ww,wy,wh;
long    widA = winget();

  /* wrn("BEGIN rubberRectZoom:   mouseDown:%d",mouseDown); */

  getorigin(&wx,&wy);
  getsize(  &ww,&wh); ww -= 1; wh -= 1;

  xn = xo = xb = (getvaluator(MOUSEX) - wx);
  yn = yo = yb = (getvaluator(MOUSEY) - wy);

  cursoff();
  drawmode(OVERDRAW);

  while(getbutton((Device)device)) {
    xn = getvaluator(MOUSEX)-wx; if (xn < 0) xn = 0;  else if (xn > ww) xn = ww;
    yn = getvaluator(MOUSEY)-wy; if (yn < 0) yn = 0;  else if (yn > wh) yn = wh;
    if (xn != xo || yn != yo) {
      color(BLACK);                   /* black erases */
      sboxi(xb,yb,xo,yo);
      color(RED);                     /* set color of new circle */
      sboxi(xb,yb,xn,yn);
      xo = xn,  yo = yn;
    }
  }
  color(BLACK);
  sboxi(xb,yb,xo,yo);
  if (xb > xn)  xn = xb, xb = xo;
  if (yb > yn)  yn = yb, yb = yo;
  xo = (long)((xn - xb)*izoom*zX);
  yo = (long)((yn - yb)*izoom*zY);
  xo = MIN(xo,yo);
  drawmode(NORMALDRAW);
  curson();

#define     MIN_BOX_LEN  100
  if (xo < MIN_BOX_LEN) {
    wrn("rubberRectZoom: box size: %d < MIN: %d", xo, MIN_BOX_LEN);
    return 0;
  }
  cpack(FcGRN);
  sboxi(xb,yb,xn,yn);
  /* wrn("B4 showSub: %4d  %4d  %4d  %4d   zX:%.2f  zY:%.2f",xb,yb,xn,yn, zX, zY); */
  wA->next = showSubCAST(wA, (xb/zX), (yb/zY), (xn/zX), (yn/zY), "subA", zX*izoom, zY*izoom,5);
  wA->next->lefMouse = wA->lefMouse;
  wA->next->prev = wA;
  winset(widA);
return wA->next->id;
}

#define CRAD 5
void
labelFace (void *v, int device, int nlabs)
{
FILE 	*ofp;
static  int 	nl = 0, xl[8], yl[8];
static  dbl 	dx,dy;
pWmj    wA = (pWmj)v;
flt     zX = wA->zX, zY = wA->zY;
long    nn, xn, yn, xo = -999, yo = -999, wx,ww,wy,wh;
short crs;
Colorindex clr, wtm;
Boolean b;

  getcursor(&crs, &clr, &wtm, &b);
  getorigin(&wx,&wy);
  getsize(  &ww,&wh); ww -= 1; wh -= 1;
  /* cursoff(); */
  drawmode(OVERDRAW);
  curstype(CCROSS);
  defcursor(2,NULL);
  setcursor(2,0,0);

  nn = getgdesc(GD_CROSSHAIR_CINDEX);
  /* wrn("GD_CROSSHAIR_CINDEX: %d, crs: %d, b: %d", nn,crs,b); */

  /* decide whether this is a new label */
  xn = getvaluator(MOUSEX)-wx; if (xn < 0) xn = 0; else if (xn > ww) xn = ww;
  yn = getvaluator(MOUSEY)-wy; if (yn < 0) yn = 0; else if (yn > wh) yn = wh;

  nl++; 
  for (nn = 1; nn < nl; nn++) {
    dx = xn - xl[nn];
    dy = yn - yl[nn];
    if (dx*dx + dy*dy < 200.0) {
      xo = xl[nn], yo = yl[nn]; 
      color(BLACK);
      circi(xo,yo,CRAD*2);
      nl--;
      break;
    }
  }
  if (nl > nlabs)  {
    sprintf(Ss,"%s.lbl",wA->T);
    if ( ! (ofp = fopen(Ss,"w")))
      die("fopen failed on <%s>\n",Ss);
    fprintf(stderr, "\nl_eye \t%.8f \t%.8f\n"  , (dbl)xl[1]/ww, (dbl)(yl[1])/wh);
    fprintf(stderr,   "r_eye \t%.8f \t%.8f\n"  , (dbl)xl[2]/ww, (dbl)(yl[2])/wh);
    fprintf(stderr,  "center \t%.8f \t%.8f\n"  , (dbl)xl[3]/ww, (dbl)(yl[3])/wh);
    fprintf(ofp   , "# winLooper LABEL_FACE mode on %s (%d x %d x %d)\n"
	,wA->T,wA->D.w,wA->D.h,wA->D.d);
    fprintf(ofp   , "\nl_eye \t%.8f \t%.8f\n"  , (dbl)xl[1]/ww, (dbl)(yl[1])/wh);
    fprintf(ofp   ,   "r_eye \t%.8f \t%.8f\n"  , (dbl)xl[2]/ww, (dbl)(yl[2])/wh);
    fprintf(ofp   ,  "center \t%.8f \t%.8f\n\n", (dbl)xl[3]/ww, (dbl)(yl[3])/wh);
    fclose(ofp);
    nl = 0;
    drawmode(NORMALDRAW);
    /* curson(); */
    return;
  }
  while(getbutton((Device)device)) {
    xn = getvaluator(MOUSEX)-wx; if (xn < 0) xn = 0; else if (xn > ww) xn = ww;
    yn = getvaluator(MOUSEY)-wy; if (yn < 0) yn = 0; else if (yn > wh) yn = wh;
    if (xn != xo || yn != yo) {
      color(BLACK);
      circi(xo,yo,CRAD);
      color(GREEN);
      circi(xn,yn,CRAD);
      sprintf(Ss,"%s label %ld  xy: %.1f %.1f  (%.7f  %.7f)"
	,wA->T, nn, xn/zX, yn/zY,  (dbl)xn/ww,  (dbl)(yn)/wh);
      wintitle(Ss);
      xo = xn,  yo = yn;
    }
  }
  color(RED);
  circi(xn,yn,CRAD*2);
  drawmode(NORMALDRAW);
  /*setcursor(crs,0,0);*/
  /*curson();*/
  xl[nn]  = xn;
  yl[nn]  = yn;
}


pWmj Wclone(pWmj W, int flags)
{
pWmj    Wn;  /* @@ shouldn't is be: Wimj Wm = *W; ?? */
  sprintf(Ss,"%s - cloned",W->T);
  Wn = W->next = showPm(W->P,&W->D,Ss,W->zX,W->zY,flags);
  Wn->prev = W;
return Wn;
}

pWmj cloneGrid(pWmj W, int flags)
{
pWmj    Wn = NULL;
pDmj	D = &W->D;
flt 	rgb[3];

  wrn("cloneGrid, W->F is %lu", (unl)W->F);

  if (W->F) {
    rgb[0] = 200.0;
    rgb[1] = 000.0;
    rgb[2] = 250.0;
    rgbMarkGridRect(W->F,D->x,D->y,D->w,D->h, 10, 15, rgb);
    sprintf(Ss,"%s - cloneGrid",W->T);
    Wn = W->next = showFm(W->F,D,Ss,2.0,flags);
    Wn->prev = W;
  }
return Wn;
}


int drawThis (void *vp) /* Example function for G->glfn */
{
pGlst   G = (pGlst) vp;
int     j = G->glen/3;
punl    rgba = G->rgba;
flt     *fxy = G->glxy;
#if 0
     sfactr   is a symbolic constant taken from the list below.  It identifies
              the blending factor by which to scale contribution from source
              pixel RGBA (red, green, blue, alpha) values.
              BF_ZERO        0
              BF_ONE         1
              BF_DC          (destination RGBA)/255
              BF_MDC         1 - (destination RGBA)/255
              BF_SA          (source alpha)/255
              BF_MSA         1 - (source alpha)/255
              BF_DA          (destination alpha)/255
              BF_MDA         1 - (destination alpha)/255
              BF_MIN_SA_MDA  min(BF_SA, BF_MDA)
              BF_CC          (constant RGBA)/255
              BF_MCC         1 - (constant RGBA)/255
              BF_CA          (constant alpha)/255
              BF_MCA         1 - (constant alpha)/255
              BF_MIN         min(1, destination RGBA/source RGBA)
              BF_MAX         max(1, destination RGBA/source RGBA)
     dfactr   is a symbolic constant taken from the list below.  It identifies
              BF_ZERO        0
              BF_ONE         1
              BF_SC          (source RGBA)/255
              BF_MSC         1 - (source RGBA)/255
              BF_SA          (source alpha)/255
              BF_MSA         1 - (source alpha)/255
              BF_DA          (destination alpha)/255
              BF_MDA         1 - (destination alpha)/255
              BF_CC          (constant RGBA)/255
              BF_MCC         1 - (constant RGBA)/255
              BF_CA          (constant alpha)/255
              BF_MCA         1 - (constant alpha)/255
#endif

long	sfactr=BF_SA, dfactr=BF_MSA;  /* USABLE: dfactr=BF_SA; */

  while (j--) {
    if (j % 2) {
      bgnclosedline();
        cpack(rgba[0]); v2f(&fxy[0]);  
        cpack(rgba[1]); v2f(&fxy[2]); 
        cpack(rgba[2]); v2f(&fxy[4]);
      endclosedline();
    } else {
      blendfunction(sfactr,dfactr);
      bgnpolygon();
        cpack(rgba[0] & 0x7FFFFFFF); v2f(&fxy[0]);  
        cpack(rgba[1] & 0x7FFFFFFF); v2f(&fxy[2]);
        cpack(rgba[2] & 0x7FFFFFFF); v2f(&fxy[4]);
      endpolygon();
      blendfunction(BF_ONE,BF_ZERO);
    }
    cpack(rgba[0]); circ(fxy[0],fxy[1],2.0);
    cpack(rgba[1]); circ(fxy[2],fxy[3],2.0);
    cpack(rgba[2]); circ(fxy[4],fxy[5],2.0);
    fxy += 6, rgba += 3;
  }
  return G->glen/3;
}

void
showThisGL(pWmj W)
{
static  unl cA[64], *cp;
long    j, ww,wh;
flt     *dp,*sp;
pGlst   gA=NULL;
static 	long	numPxy = 21;
static  flt     fA[64], fxy[] = {
	.10F, .10F,   .30F, .40F,   .50F, .10F,
	.10F, .90F,   .10F, .70F,   .60F, .80F,
        .90F, .90F,   .60F, .90F,   .75F, .60F,
	.45F, .55F,   .50F, .45F,   .55F, .55F,
	.30F, .40F,   .70F, .25F,   .50F, .70F,
	.60F, .10F,   .75F, .25F,   .90F, .05F,
	.80F, .10F,   .85F, .95F,   .90F, .10F };

  if (! gA) {
    gA = (pGlst)calloc(1,sizeof(Glst));
    gA->glfn = &drawThis;
    gA->glen = numPxy;
    dp = gA->glxy = fA; cp = gA->rgba = cA;
    for (sp = fxy, j  = numPxy/3;  j-- ; cp += 3)
      cp[0] = FcRED, cp[1] = FcGRN, cp[2] = FcBLU;
  }
  getsize(&ww,&wh);
  for (sp = fxy, j  = numPxy/3;  j-- ; dp += 6, sp += 6) {
    dp[0] = sp[0]*ww;  dp[1] = sp[1]*wh;
    dp[2] = sp[2]*ww;  dp[3] = sp[3]*wh;
    dp[4] = sp[4]*ww;  dp[5] = sp[5]*wh;
  }
  if (! W->G)            { W->G = gA; }
  else if (! W->G->glfn) { W->G->glfn = gA->glfn; }  /* Shouldn't happen? */
  gA->stat = gA->glfn(gA);
}


int checkScreenSize(int xsize, int ysize)
{
  int k = getgdesc(GD_XPMAX);
  int j = getgdesc(GD_YPMAX);
  if( xsize > k || ysize > j) {
    warn("checkScrnSize: %d x %d image too big for %d x %d screen"
        ,xsize,ysize,k,j);
    return -1;
  }
  return 0;
}



#if 00
static void mjPackFlt (punl ulp, ppu src[], pDmj D);
void mjPackFlt (punl ulp, ppu src[], pDmj D)
{
unt     d = D->d;
  switch(d) {
    case  3:  mjPackFltRGBP(ulp, src,  D);              break;
    case  4:  mjPackFltRGBA(ulp, src,  D);              break;
    default: die("mjPackFlt <%s>: NOIMP for depth %d",__FILE__,d);
  }
}
#endif /*00*/



void mjPackFltI ( punl ulp, ppf row, pDmj D)
{
  switch(D->d) {
    case  1: mjPackFltGray(ulp,row,D);   break;
    case  3: mjPackFltRGBI(ulp,row,D);   break;
    default: die("mjPackFltI [%s]: NOIMP for d==%d",__FILE__,D->d);
  }
}


void mjPackUnc (punl ulp, ppu src[], pDmj D)
{
  switch(D->d) {
    case  1:  mjPackUncGray(ulp, *src,  D);  break;
    case  3:  mjPackUncRGBP(ulp,  src,  D);  break;
    case  4:/*mjPackUncRGBA(ulp,  src,  D);  break;*/
              mjPackUncRGBP(ulp,  src,  D);  break;
    default: die("mjPackUnc <%s>: NOIMP for depth %d",__FILE__,D->d);
  }
}




int nextPos(pWmj new, pWmj old)
{ /** Use to assign new to the position just right of old  **/
  if (! old || !new)  return -1;
  new->wx = old->wx + old->D.w + 16;
  new->wy = old->wy;
  return 0;
}


int
Wpos(pWmj W, long cx, long cy, unt nw, unt nh, unt flags)
{
static int on,ox,oy,oh;

  oh = nh;

  /*wrn("Wpos: [ %d %d %d ]%d %d %u %u   %u",on,ox,oy,cx,cy,nw,nh,flags);*/

  checkScreenSize(nw,nh);

#if 0
  if (flags == 3) {	/* @@ Set by resource file WinLoop: */
    prefsize(nw,nh);	/* winLoop.gl.winLoop.origin 400 400 pixels */
    /*wrn("Wpos set size %d %d, relying on WinLoop for origin...",nw,nh);*/
    return 0;
  } 
#endif

  on++;
  if (cx || cy)		W->wx = cx, W->wy = cy;
  else switch (flags) {
    case  8: case  9: W->wx = WIDE-nw-12;     W->wy = TALL-nh-38;     break;
    case  6: case  7: W->wx = (WIDE-nw)/2-12; W->wy = TALL-nh-38;     break;
    case  5:          W->wx = 12;             W->wy = TALL-nh-38;     break;
    case  4:          W->wx = (WIDE-nw)/2-12; W->wy = (TALL-nh)/2-38; break;
    case  1: case  2: case  3:
      if      (on == 1) 	W->wx = 12,  W->wy = TALL-nh-38; 
      else if (ox + nw > WIDE)	W->wy = oy + oh;
      else 			W->wx = ox+12,  W->wy = oy;
      break;
    case  0:
    default: 	/*wrn("Wpos: default: flags == %u",flags);*/
      if (on == 1)	W->wx = 5, W->wy = 60;
      else {
        W->wy = oy;
        if (ox+nw > WIDE)  W->wx = ox =  8,  W->wy = oy+nh;
        else               W->wx = ox + 14;
        if (oy+nh > TALL)  W->wy = TALL-nh-23;
      }
  }
  ox = W->wx, oy = W->wy;
  prefposition(ox,ox+nw-1,oy,oy+nh-1);
  ox += nw;
  return oy;
}


#define NO_STRUCT_ASSIGN 1

pWmj
Wnew (pDmj D, char *winTitle, float zX, float zY, unt flags)
{
  unt zW,zH;
  pWmj W = calloc(1,sizeof(Wimj));
  W->D = *D;

  W->zX = zX;
  W->zY = zY;
  zW = (unt)(D->w*W->zX + 0.5F);
  zH = (unt)(D->h*W->zY + 0.5F);
  Wpos(W,0,0,zW,zH,flags);
  W->T  = strdup(winTitle);

  if (++WnNum == 1) {	/* 1st window named to get GL Resources. */ 
    W->id = winopen(WL_RESOURCE_NAME);	/* defined in winLoop.h. */
    wintitle(winTitle); /* switch to caller-specified title.     */
    init_GL_config();
  }
  else if (WnNum >= MWIND)  warn("Wnew: too many (%d) windows!",WnNum);
  else {
    /*wrn("Wnew: making WnNum %d", WnNum);*/
    W->id = winopen(winTitle);
  }
  if (W->id < 0) {
    warn("Wnew: winopen failed on %d: No more windows!",WnNum);
    return NULL;
  }
  Wj[W->id] = W;
  iconsize(100,100);	/** Not clear why, but this seems needed. **/

  /*
   prn("Wnew[%s] addr: %lu	id: %d	WnNum: %d",winTitle,(unl)W,W->id,WnNum);
   */
return W;
}


pWmj
Wreg (long wid, pDmj D, char *winTitle, float zX, float zY, unt flags)
{
pWmj	W = calloc(1,sizeof(Wimj));
  W->id   = wid;
  W->D    = *D;
  W->zX   = zX;
  W->zY   = zY;
  W->T    = strdup(winTitle);

  /* if (wid < 0)  { warn("Wreg: can't use window id %d",wid);  return NULL; } */

  if (++WnNum == 1) {   
    ;  /* W->id = winopen(WL_RESOURCE_NAME); Too late to get GL Resources? */
  } 
  else if (WnNum >= MWIND)  warn("Wreg: too many (%d) windows!",WnNum);

  Wj[W->id] = W;
  iconsize(100,100);    /** Not clear why, but this seems needed. **/
return W;
}

pWmj
showSub( pWmj S, int xi, int yi, int xf, int yf, char *winTitle
        , dbl zoomX, dbl zoomY, unt flags)
{
pWmj    W;
Dmj     *sD = &S->D, Dm;
punl    P, dp, sp;
int     j, k, sw = sD->w;
  Dm.x = 0;		Dm.l = xi;
  Dm.y = 0;		Dm.b = yi;
  Dm.w = xf - xi + 1;	Dm.r = (sD->w - xf);	/* is r or t ever used? */  
  Dm.h = yf - yi + 1;	Dm.t = (sD->h - yf);
  Dm.d = sD->d;
/*
  wrn("wL:showSub: %4d  %4d  %4d  %4d  ->  Dm.lbrt: %4d  %4d  %4d  %4d"
	,xi,yi,xf,yf,Dm.l,Dm.b,Dm.r,Dm.t);
*/
#if 1
  mallocAss(P,unl,Dm.w*Dm.h);        /* Show only [ltrb] instead? */
  for (dp = P, sp = S->P + yi*sw + xi, j = Dm.h; j-- ; sp += (sw - Dm.w))
    for (k = Dm.w; k-- ; )
      *dp++ = *sp++;
  W = showPm(P,&Dm,winTitle,(flt)zoomX,(flt)zoomY,flags);
#endif
return  W;
}

void 
WdrawP (pWmj W)
{
long	dmode;
  winset(W->id);		/* added 95.07.16 */
  if (W->P) {
    RGBmode();                          /** Must precede gconfig? **/
    rectzoom(W->zX,W->zY);      /** Zoom is user global, default 1.0F **/
    gconfig();                          /** doublebuffer(); **/
    lrectwrite((Screencoord)0, (Screencoord)0, (Screencoord)(W->D.w-1), (Screencoord)(W->D.h-1), (W->P));
    switch (dmode = getdisplaymode()) {
      case DMRGBDOUBLE: swapbuffers();  break;
    }
  }
  else      clear();                    /* Set background color before clear? */
}

void
Wdraw (pWmj W, int dev, int flags)
{ 
pGlst   G  =  W->G;

  /** W may not already the current graphics window (e.g. expose events): **/
  winset(W->id);
  getorigin(&W->wx,&W->wy);

  if (W->draw)  { W->draw(W,dev,flags); return; }

  /** Draw image or clear background for graphics: **/
  WdrawP (W);

  /** Now execute any attached graphics drawing function(s): **/

  if (G) {
    if (G->stat) G->stat = G->glfn(G);
    else wrn("Wdraw: NO DRAW %d CUZ G:%lu   stat:%d",W->id,(unl)G,(G?G->stat:0));
  }
}


pWmj
showMjU ( ppu U[], pDmj D, pGlst G, char *winTitle, unt flags)
{
unt 	j = 0;
pWmj	W;
  if (!(W = Wnew(D,winTitle,Zoom,Zoom,flags)))
    return 0;	/* valid win id's begin at 1 on Indigo, 257 Indy */
  mallocAss(W->P,unl,D->w*D->h);	/* Show only [ltrb] instead? */
  mjPackUnc(W->P,U,&W->D);
  do { W->U[j] = U[j]; } while (++j < D->d);
  /** winset(W->id); 			Redundant? **/
  if (G)  W->G = G;
  Wdraw(W,0,flags);		/** Execute any GL draw function(s) **/
  return W;
}


pWmj
showU ( ppu U[4], int sX, int sY, unt sW, unt sH, unt sD, pGlst G
      , char *winTitle, unt flags)
{
pWmj W;
Dmj Dm;  unt j=0;
  Dm.x =     Dm.l = sX;
  Dm.y =     Dm.t = sY;
  Dm.w = sW, Dm.r = sW-1;
  Dm.h = sH, Dm.b = sH-1;
  Dm.d = sD;

  if (! (W = Wnew(&Dm,winTitle,Zoom,Zoom,flags)))
    return 0;
  mallocAss(W->P,unl,sW*sH);
  mjPackUnc(W->P,U,&W->D);
  do { W->U[j] = U[j]; } while (++j < sD);
  if (G)  W->G = G;
  Wdraw(W,0,0);
  return W;
}



pWmj
showF ( ppf F, int sX, int sY, unt sW, unt sH, unt sD, pGlst G
      , char *winTitle, double zoom, unt flags)
{
pWmj W;
Dmj Dm;
  Dm.x =     Dm.l = sX;
  Dm.y =     Dm.t = sY;
  Dm.w = sW, Dm.r = sW-1;
  Dm.h = sH, Dm.b = sH-1;
  Dm.d = sD;
  if (! (W = Wnew(&Dm,winTitle,(flt)zoom,(flt)zoom,flags)))
    return 0;
  mallocAss(W->P,unl,sW*sH);
  /* if (flags)  warn("showF: treating src as interleaved regardless of flags"); */
  W->F = F;
  mjPackFltI(W->P,F,&W->D);
  if (G)  W->G = G;
  Wdraw(W,0,0);
  return W;
}

pWmj
showFm( ppf F, pDmj Dm, char *winTitle, flt zoom, unt flags)
{
pWmj W;
  if (! (W = Wnew(Dm,winTitle,zoom,zoom,flags)))  return 0;
  mallocAss(W->P,unl,Dm->w*Dm->h);
  W->F = F;
  mjPackFltI(W->P,F,&W->D);
  Wdraw(W,0,0);
return  W;
}

pWmj
showPm(unl *P, pDmj Dm, char *winTitle, float zoomX, float zoomY, unt flags)
{
pWmj W;
  if (! (W = Wnew(Dm,winTitle,zoomX,zoomY,flags)))  return 0;
  W->P = P;
  Wdraw(W,0,0);
return  W;
}



long Wnext (long wid)  /* wid should be a valid Wn index (0 <= wid < MWIND) */
{		       /* If wid == 0, return with lowest current window ID */
  long j;              /* (window ID's are actually always >= 1) */
  if (wid < 0 || wid >= MWIND)  wid = 1;
  j = wid;
  do { if (++j >= MWIND)  j = 0; if (Wj[j])  break; } while (j != wid);
  if (j == wid) {
    j = -1;
    /*wrn("Wnext: Couldn't find a non-null window after %d",wid);*/
  }
  return j;
}


int Wdestroy(long iw)
{
pWmj    W = Wj[iw];
  winclose(iw);
  WnNum--;
  if (W->P)	free(W->P);
  /** Kill only what Wnew or show* created, not W->F or W->U **/
  free(Wj[iw]);
  Wj[iw] = NULL;
  return Wnext(iw);  /* Return the id of a non-NULL window, tho it shouldn't matter. */
}


int drawList (void *vp)	/* Example function for G->glfn */
{
pGlst	G = (pGlst) vp;
int     j = G->glen;
unl 	centClr = 0xFF606060, *rgba = G->rgba;
flt     xx,yy, *f = G->glxy;

  while (j--) {
    xx = *f++;
    yy = *f++;
    cpack(centClr);
    circf(xx,yy,1.0);
    cpack(*rgba++);
    circ( xx,yy,2.0);
  }
  return G->glen;
}

void putRowsPackedToRGB(IMAGE *sgim, punl src, unt dW, unt dH)
{
ush     red[1280], grn[1280], blu[1280];        /* @@ Captain Kludge */
unt j;
  for (j = 0; j < dH; src += dW, j++) {
      cpacktorgb(src,red,grn,blu,dW);
      putrow( sgim,  red, j, 0);
      putrow( sgim,  grn, j, 1);
      putrow( sgim,  blu, j, 2);
  }
}

void
saveRgbGL(pWmj W, unt flags)
{ /* Uses lrectread to get W->P mixed with (GL) drawn overlays */
pDmj    D  = &W->D;
int     dW = D->w, dH = D->h, j;
size_t	nread, sz;
punl	rectP = W->P;
char    ss[SLEN], path[TLEN], *tag="RGB";
IMAGE   *sgim;
Screencoord wd = (ush)(dW*W->zX + 0.5F);
Screencoord ht = (ush)(dH*W->zY + 0.5F);

  if (flags)	{
    tag = "RgbGL";
    dW  = wd;
    dH  = ht;
  }
  if (! W->ti)  { 
    sprintf(path,"%s.%03ld",tag,W->id); /* Would set to default icon title...   */
    W->ti = strdup(path);              /* but can't find equiv. of getwintitle */
  }                                    /* set W->ti so we see where file went. */
  else sprintf(path,"%s.%s",tag,W->ti);
  sprintf(ss,"%s: saving: %s ...",W->T,path);
  wintitle(ss);
  if (! (sgim = iopen(path,"w",RLE(1),3,dW,dH,3))) {
    warn("save%s: iopen(%s) failed",tag,path);
    return;
  }

  if (flags || !rectP) {
    sz    = (unl)dW*dH;
    mallocAss(rectP,unl,sz);
    if ((nread = lrectread((Screencoord)0,(Screencoord)0,(Screencoord)(wd-1),(Screencoord)(ht-1), rectP)) != sz)
      warn("lrectread returned %lu instead of %lu",nread,sz);
    putRowsPackedToRGB(sgim,rectP,dW,dH);
    free(rectP);
  }
  else putRowsPackedToRGB(sgim,rectP,dW,dH);

  j = iclose(sgim);/* 0:OK, -1:fclose error (check errno), EOF:libgutil err */
  if (j) sprintf(ss,"%s: ERROR %d saving %s",W->T,j,path);
  else {
    sprintf(ss,"%s: saved as %s",W->T,path);
  }
  wintitle(ss);
}



#if 0000		/* Not used */
static int
saveRGB(long wid)
{
pWmj    W  = Wj[wid];
pDmj    D  = &W->D;
int  	dW = D->w, dH = D->h, j;
char	ss[SLEN], *path = W->ti;
IMAGE   *sgim;

  if (! path)  {
    sprintf(ss,"RGB.%03d",wid);  /* Would set to default icon title...   */
    W->ti = path = strdup(ss);	 /* but can's find equiv. of getwintitle */
  }				 /* set W->ti so we see where file went. */
  sprintf(ss,"%s: saving %s ...",W->T,path);
  wintitle(ss);
  if (! (sgim = iopen(path,"w",RLE(1),3,dW,dH,3))) {
    warn("saveRGB: iopen(%s) failed",path);
    return -1;
  }

  putRowsPackedToRGB(sgim,W->P,dW,dH);

  j = iclose(sgim);/* 0:OK, -1:fclose error (check errno), EOF:libgutil err */
  if (j) sprintf(ss,"%s: ERROR %d saving %s",W->T,j,path);
  else {
    #ifdef DEBUG
    sprintf(ss,"rectimg %s",path); sys(ss);
    #endif
    sprintf(ss,"%s: saved as %s",W->T,path);
  }
  wintitle(ss);
  return j;
}
#endif /*0000*/


void Wicon(pWmj W, int dev, int flags)
{
#if 0

char    *cp = W->T; /* May have value 0, but should never be NULL */ 
unl     uclr;

  winset(W->id);

#endif

  reshapeviewport();    /* This is the minimum required functionality.    */

  if (W->draw)  { W->draw(W,dev,flags); return; }

  /** Here is an easy addition: icon color from 1st 5 chars of title: **/
/*
  uclr = (unl)(cp[0]|(cp[1]<<4)|(cp[2]<<8)|(cp[3]<<12)|(cp[4]<<16)|((unl)0xFF<<24));
  cpack(uclr); clear();
*/
  /************************* I've never gotten icontitle to work...
  if (! Wj[iw])   wrn("redraw on Null window iw = %d, skipping",iw);
  else if (Wj[iw]->ti)    icontitle(Wj[iw]->ti);
  else                    icontitle(Wj[iw]->ti="snoopy");
  *********************************************************************/
}


#define BUFSIZE 50

void drawit()
{
    loadname(1);
    color(BLUE);
    writemask(BLUE);
    sboxfi(20, 20, 100, 100);

    loadname(2);
    pushname(21);
        color(GREEN);
        writemask(GREEN);
        circfi(200, 200, 50);
    popname();
    pushname(22);
        color(RED);
        writemask(RED);
        circi(200, 230, 60);
    popname();
    writemask(0xfff);
}


void printhits(buffer, hits)
short buffer[];
long hits;
{
    int indx, items, h, i;

    sprintf(Ss, "%ld hit", hits);
    charstr(Ss);
    if (hits != 1)
        charstr("s");
    if (hits > 0)
        charstr(": ");
    indx = 0;
    for (h = 0; h < hits; h++) {
        items = buffer[indx++];
        charstr("(");
        for (i = 0; i < items; i++) {
            if (i != 0)
                charstr(" ");
            sprintf(Ss, "%d", buffer[indx++]);
            charstr(Ss);
        }
        charstr(") ");
    }
}

int doThePick(pWmj W)
{
long hits;
long xsize, ysize;
short buffer[BUFSIZE];
int j = WnNum;

  do { if (Wj[j] == W) break; } while(j--);
  wrn("doThePick: WnNum is %d & we think W == Wj[%d]",WnNum,j);

  getsize(&xsize,&ysize);
  pick(buffer, BUFSIZE);
    ortho2(-0.5F, xsize - 0.5F, -0.5F, ysize - 0.5F);
    drawit();   /* no actual drawing takes place */
  hits = endpick(buffer);
  ortho2(-0.5F, xsize - 0.5F, -0.5F, ysize - 0.5F);
  color(BLACK);
  sboxfi(150, 20 - getdescender(), xsize - 1, 20 + getheight());
  color(WHITE);
  cmov2i(150, 20);
  printhits(buffer, hits);


return j;
}


void (*WLstep) (void) = NULL;
void (*WLswing)(void) = NULL;
void (*WLmouseX)(pWmj W, int dev, int val) = NULL;
void (*showSomeGL)(pWmj W) = showThisGL;


int
winLoopSgi(unt flags)
{
static  long	iw, pval, menuSave,menuMark;
static  char 	*mstrMain, *mstrSave, *mstrMark;
short	qval;
Device	qdev;
WmjAct* wa;
WmjMnu* wm;

/* wrn("winLoopSgi: BEGIN: WnNum: %d   Wj[1]: %lu ", WnNum, (unl)Wj[1]); */

  if (WnNum < 1) {
    /* warn("winLoopSgi: No windows or Wj[1] is NULL -- Bye-Bye!\n"); */
    return WnNum;
    /* exit(0); */
  }
  /*
  else prn("winLoopSgi: WnNum: %d,  Wj[1]: %lu\n",WnNum,(unl)Wj[1]);
  */

  if (! iw) {			/* should be 0 only on 1st call */
    qdevice(LEFTMOUSE);
    qdevice(MIDDLEMOUSE);
    qdevice(MENUBUTTON);
    qdevice(LEFTARROWKEY);
    qdevice(RIGHTARROWKEY);
    qdevice(PADMINUS);
    qdevice(PADPLUSKEY);
    qdevice(ESCKEY);
    qdevice(F1KEY);
    qdevice(F2KEY);
    qdevice(WINSHUT);
    qdevice(WINFREEZE);
    qdevice(WINTHAW);
    qdevice(PADENTER);
    qdevice(REDRAWICONIC);
    qdevice(WINCLOSE);	/* Queued, but seems never to happen. */
    /* qdevice(INPUTCHANGE); qdevice(REDRAW); --done by winopen */


    mstrMark  = "Marks %t|1st menu %x31|GridTet %x32|What? %x33";
    menuMark  =  defpup(mstrMark);
    mstrSave  = "SAVE %t|Clone %x61|FrameSrc %x62|Frame+GL %x63|CloneGRID %x64";
    menuSave  =  defpup(mstrSave);
    mstrMain  = "WinLoop %t|LabelFace|Zoomer|Mark %m|Edit %l|Save %m|Return|Quit";
    WLmenu    =  defpup(mstrMain,menuMark,menuSave);
  }

#define retitle(S)  	sprintf(Ss,"%s: %s",Wj[iw]->T,(S)); wintitle(Ss)
#define WLmodeTil(M,S)	WLmode = M; retitle(S)

  for (;;) {    
    if      (WLmode == SWINGING && WLswing != NULL)  while (!qtest())  WLswing();
    else if (WLmode == LOOPING  && WLstep  != NULL)  while (!qtest())  WLstep();
    /* else if (ICCV)  while (!qtest())  readICCVparams(); */

    switch (qdev = (ush)qread(&qval)) {

      case MENUBUTTON:
        if(qval) {
          if ((wm=Wj[iw]->mnu)) wm->func(Wj[iw],qdev,flags); /* try window's own menu */
          else switch(pval = dopup(WLmenu)) {		     /* else use default menu */

            case -1: /* no selection */		  		break;
            case  1: WLmodeTil(LABEL_FACE,"Label Face");  	break;
            case  2: if   (WLmode == ZOOMER) {WLmodeTil(WLmodeDef,"");}
                     else {WLmodeTil(ZOOMER,"Zoomer");}		break;

            case  3: 	/* GL submenu (never the case) */	break;
            case 31: WLmodeTil(STOPPED,"Show GL"); showSomeGL(Wj[iw]);	break;
            case 32: WLmodeTil(STOPPED,"GridTet"); makeGridTet(Wj[iw],16,16,0);  break;

            case 4 : wrn("WL: No default Edit submenu"); 	break;

            case 5 : 	/* MOVIE submenu */			break;
            case 51: WLmodeTil(SWINGING,"SWING");		break;
            case 52: WLmodeTil(LOOPING,"LOOP");			break;
            case 53: WLmodeTil(MOUSE_X,"mouse-X");		break;
            case 54: WLmodeTil(STOPPED,"STOP");			break;

            case 6 : 	/* SAVE submenu */			break;
            case 61: WLmodeTil(STOPPED,"CLONED");          Wclone(Wj[iw],0);	break; 
            case 62: WLmodeTil(STOPPED,"RGB Saved");    saveRgbGL(Wj[iw],0);	break;
            case 63: WLmodeTil(STOPPED,"RGB+GL Saved"); saveRgbGL(Wj[iw],1);	break;
            case 64: WLmodeTil(STOPPED,"cloned+GRID");  cloneGrid(Wj[iw],1);	break;

            case  7: goto RETURN;
            case  8: goto DEATH;

            default: warn("winLoopSgi: Unknown menu option: qval:%d, pupval:%d",qval,pval);
	    /** Menu can pop up and go away again with no selected item. **/
          }
        }
        break;

      case LEFTMOUSE:		/* qval = 1 for mouseDown, 0 for mouseUp */
        if      (qval && WLmode == MOUSE_X && WLmouseX)  WLmouseX(Wj[iw],LEFTMOUSE,qval);
        else if (qval && WLmode == LABEL_FACE)  labelFace(Wj[iw],LEFTMOUSE,3);
        else if (qval && WLmode == ZOOMER)  rubberRectZoom(Wj[iw],LEFTMOUSE,2);
        else if ((wa=Wj[iw]->lefMouse))	wa->func(Wj[iw],LEFTMOUSE,qval);
        else if ((wa=Wj[iw]->act))	wa->func(Wj[iw],LEFTMOUSE,qval);
        break;

      case MIDDLEMOUSE:
        if      ((wa=Wj[iw]->midMouse))	wa->func(Wj[iw],MIDDLEMOUSE,qval);
        else if ((wa=Wj[iw]->act)) 	wa->func(Wj[iw],MIDDLEMOUSE,qval);
        break;

      case INPUTCHANGE:		/** 0 == outside app's wins **/
        if (qval > 0)		/** @@ THIS IS SOMETHING OF A KLUGE! **/
          winset(iw=qval);     
#if 0 	/* VERBOSE */
        { long Xo,Yo;
          getorigin(&Xo,&Yo);
          iw = winget();
          wrn("INPUTCHANGE %d %d ORG %4d %4d",qval,iw,Xo,Yo);
        }
#endif
        break;

      case WINTHAW:		/* Easy, cuz here winget() always == qval */
	/*wrn("WL WINTHAW %d", qval);*/
        reshapeviewport();	/* no break; */
      case REDRAW:
	/* wrn("WL WINTHAW OR REDRAW: %d    qval: %d", qdev, qval); */
        Wdraw(Wj[iw=qval],qdev,qval);	/* Wdraw does the winset(iw) (on expose, etc)*/
        break;

      case REDRAWICONIC:   /* qval is the correct window id, NOT iw=winget() */
        Wicon(Wj[iw=qval],REDRAWICONIC,qval);
        break;	/* let continue on to WINFREEZE? */

      case WINFREEZE:	/* Queued automatically after "1st" REDRAWICONIC */
        /* Stop ongoing rendering or frame-swapping; otherwise ignore.   */
        /* wrn("WINFREEZE qval:%d  old_iw:%d  tw=winget:%d",qval,iw,tw); */
	break;

      case WINCLOSE:  wrn("WINCLOSE (Is this an Indy?) -- treat as WINSHUT");
      case WINSHUT: /*wrn("WINSHUT -- destroying window %d",iw);*/
      case F1KEY:
      case PADPLUSKEY:
        if ((iw = Wdestroy(winget())) > 0)  break;
        wrn("Wdestroy left no more windows, so going to return");

      case WL_NOTHING:	/*prn("WL_NOTHING with qval:%d", qval);*/  break;

      case PADENTER:
      case F2KEY:
      case WL_RETURN:
        goto RETURN;

      case ESCKEY:
      case PADMINUS:
        goto DEATH;

      /*
      case LEFTARROWKEY:
        if(qval) { direction = BACK;    step(); }
        break;
      case RIGHTARROWKEY:
        if(qval) { direction = FORWARD; step(); }
        break;
      */
      default:	warn("defaultus:    qdev: %3d    qval: %3d", qdev, qval);
    }
  }

DEATH:	/* broken loop default is to die */
  gexit(); 
  /*wrn("winLoopSgi DEATH: WnNum: %d  qdev: %3d  qval: %3d",WnNum,qdev,qval);*/
  exit(0);

RETURN:	/* why live with dead windows? */
  if (WnNum > 0)
    warn("winLoopSgi RETURN: %d windows left with no update loop",WnNum);
  return WnNum;
}


