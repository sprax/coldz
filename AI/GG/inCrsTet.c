/*****************************************************************************
 inCrsTet.c    S.R.Lines    95.05.31
#include        <string.h>
 *****************************************************************************/
#include        <stdio.h>
#include        <math.h>
#include        <malloc.h>
#include        <assert.h>
#include        <gl/gl.h>
#include        <gl/device.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unFlags.h>
#include        <un/unMath.h>
#include        <un/unMacro.h>
#if 00
#include        <m/rowStat.h>
#include	<m/mjPack.h>
#include        <m/rowCpack.h>
#include        <m/rowZero.h>
#include        <m/rowMake.h>
#include        <m/rowCnvt.h>
#endif
#include	<m/mj.h>
#include        <m/rowDiff.h>
#include	<winLoop.h>
#include	<winDraw.h>
#include	"rowFlow.h"
#include	"inExtern.h"
#include	"inParm.h"
#include	"mob.h"
#include	"new.h"
#include	"blerp.h"

static int	Nvtx  = 0  ;
static float    rv0 =  11.0;
static float    rv1 =  10.0; 	/* diag   =  14.142136; */
static float    rv2 =   9.0;
static float    rv3 =   8.0;
static float    radSqr = 100.0;
static float	tetA[4][2], sttA[4][2];
static float	tetB[4][2], sttB[4][2];
static float	FloB[8][2];

#define MINAREA 200.0

void
markCrsTet2 (void *v, int device, int flags)
{
static  int     xMin,yMin, xMax,yMax;
static  flt	xAo,yAo, xBo,yBo;
static  flt	x20, y10, y20, y21, xovery20, xovery21, xovery10;
pWmj    wA   = (pWmj)v, wB = wA->next;
WmjAct *ap   = wA->act;
pMD	wwD  = (pMD)ap->data;
int 	nA   = ap->nA,       nB = ap->nB;
pMOB	mA   = wwD->mob[nA], mB = wwD->mob[nB];
pDmj	fD   = mA->fD;
ppf 	floX = mA->foX;
ppf 	floY = mA->foY;
flt 	ff, mx10, mx21, my20, fxy[2], zX = wA->zX, zY = wA->zY;
int 	j, k, km = fD->x, jm = fD->y, kM = km + fD->w - 1, jM = jm + fD->h - 1;
long	xAn=9999,yAn=-9999, xBn,yBn, orgX, orgY, wd, ht;
long	widB = wB->id, widA = winget();

  yBn = -99999;		/* change flag */
  getorigin(&orgX,&orgY);	
  getsize(&wd,&ht);	wd -= 1, ht -= 1;
  cursoff();

  drawmode(OVERDRAW);    	/* If no OVERDRAW, use PUPDRAW */
  winset(widB);
  drawmode(OVERDRAW);

  if (Nvtx >= 4)  Nvtx = 0;
  while(getbutton(device)) {
    xAn = (getvaluator(MOUSEX) - orgX); if(xAn < 0) xAn=0; else if(xAn > wd) xAn=wd;
    yAn = (getvaluator(MOUSEY) - orgY); if(yAn < 0) yAn=0; else if(yAn > ht) yAn=ht;
    if (xAn != xAo || yAn != yAo) {

      xMin =   0;
      xMax =  wd;
      yMin =   0;
      yMax =  ht;
      if (Nvtx == 3) {

        ff   = xAn - tetA[0][0];
        my20 = tetA[0][1] + ff*y20/x20;
        if      (x20 > 0.0F)   
            yMin = MAX(yMin,my20);
        else if (x20 < 0.0F)   
            yMax = MIN(yMax,my20);
        if      (yAn < yMin)    
            yAn = yMin;	/* redo x-clipping */
        else if (yAn > yMax)    
            yAn = yMax;

        ff   = yAn - tetA[1][1];
        mx10 = tetA[1][0] + ff*xovery10;
        mx21 = tetA[1][0] + ff*xovery21;
        if      (y10 > 0.0F)  xMax =  MIN(xMax,mx10);
        else if (y10 < 0.0F)  xMin =  MAX(xMin,mx10);
	if      (y21 > 0.0F)  xMax =  MIN(xMax,mx21);
        else if (y21 < 0.0F)  xMin =  MAX(xMin,mx21);
        if (xAn < xMin)   
            xAn = xMin;
        else if (xAn > xMax)   
            xAn = xMax;

#if 1		 /**** enforce convexity in source quadrilateral ****/
        if (tetA[2][0] >= tetA[0][0])  {
          yMin = my20;
          if (yAn < yMin) {
            yAn = yMin;  
            xMin = tetA[0][0];
            xMax = tetA[2][0];
          }
        }
        else  {
          yMax = my20;
          if (yAn > yMax) {
            yAn = yMax;
            xMin = MAX(xMin,tetA[2][0]);
            xMax = MIN(xMax,tetA[0][0]);
          }
        }
        if      (xAn < xMin)   xAn = xMin;	/* redo x-clipping */
        else if (xAn > xMax)   xAn = xMax;
        /* NB: Change y after x, then might need to recompute x-limits! */
/*
        if (xAn < tetA[0][0])
          yMax = tetA[0][1] + (xAn - tetA[0][0])*yoverx01;
        else
          yMax = tetA[0][1] + (xAn - tetA[0][0])/xovery20;
        if (yAn >= yMax)  yAn = yMax;
*/
      }
#endif

      k = xAn/zX, j = yAn/zY;	/* @@ The flow is not y-inverted? */
      if (k < km)  k = km;  else if (k > kM)  k = kM;
      if (j < jm)  j = jm;  else if (j > jM)  j = jM;
      xBn = xAn - zX * floX[j][k];
      yBn = yAn - zY * floY[j][k];
      if (xBn < 0)  xBn = 0;  else if (xBn > wd)  xBn = wd;
      if (yBn < 0)  yBn = 0;  else if (yBn > ht)  yBn = ht;

      winset(widA);
      color(BLACK);          		/* black erases */
      drawExCtr(xAo, yAo, rv0); /* erase old circle */

      color(BLACK);
      switch (Nvtx) {

        case  1:	/* rubber line from vertices 0 to 1 */
          bgnline(); 			/* erase old line */
          v2f(tetA[0]);
          fxy[0] = xAo, fxy[1] = yAo;
          v2f(fxy);
          endline();
          color(RED);
          circ(tetA[0][0],tetA[0][1], rv0);
          color(GREEN);
          bgnline(); 			/* draw new line */
          v2f(tetA[0]);
          fxy[0] = xAn, fxy[1] = yAn;
          v2f(fxy);
          endline();

          winset(widB);
          color(BLACK);
          bgnline();                    /* erase old line */
          v2f(tetB[0]);
          fxy[0] = xBo, fxy[1] = yBo;
          v2f(fxy);
          endline();
          color(RED);
          circ(tetB[0][0],tetB[0][1], rv0);
          color(GREEN);
          bgnline();                    /* draw new line */
          v2f(tetB[0]);
          fxy[0] = (flt)xBn, fxy[1] = (flt)yBn;
          v2f(fxy);
          endline();
          break;

        case  2:	/* rubber line from vertices 0 to 1 to 2 */
          bgnline(); 			/* erase old line */
          v2f(tetA[0]);
          v2f(tetA[1]);
          fxy[0] = xAo, fxy[1] = yAo;
          v2f(fxy);
          endline();
          color(RED);
          circ(tetA[0][0],tetA[0][1], rv1);
          color(YELLOW);
          circ(tetA[1][0],tetA[1][1], rv1);
          color(GREEN);
          bgnline(); 			/* draw new line */
          v2f(tetA[0]);
          v2f(tetA[1]);
          fxy[0] = xAn, fxy[1] = yAn;
          v2f(fxy);
          endline();

          winset(widB);
          color(BLACK);
          bgnline();                    /* draw old line */
          v2f(tetB[0]);
          v2f(tetB[1]);
          fxy[0] = xBo, fxy[1] = yBo;
          v2f(fxy);
          endline();
          color(RED);
          circ(tetB[0][0],tetB[0][1], rv1);
          color(YELLOW);
          circ(tetB[1][0],tetB[1][1], rv1);
          color(GREEN);
          bgnline();                    /* draw new line */
          v2f(tetB[0]);
          v2f(tetB[1]);
          fxy[0] = (flt)xBn, fxy[1] = (flt)yBn;
          v2f(fxy);
          endline();
          break;

        case  3:        /* rubber line from vertices 1 to 2 & 2 to 0 */
          poly2(4,tetA);
          tetA[3][0] = xAn, tetA[3][1] = yAn;
          color(GREEN);
          poly2(4,tetA);

          winset(widB);
          color(BLACK);
          poly2(4,tetB);
          tetB[3][0] = (flt)xBn, tetB[3][1] = (flt)yBn;
          color(GREEN);
          poly2(4,tetB);
          break;

        case  0:
        default:  winset(widB);  break;
      }

      winset(widA);
      color(RED);             		/* set color of new X */
      drawExCtr(xAn, yAn, rv0); /* draw new X */

      winset(widB);
      color(BLACK);     
      drawExCtr(xAo, yAo, rv0);
      drawExCtr(xBo, yBo, rv0);
      color(RED);
      drawExCtr(xAn, yAn, rv0);
      color(GREEN);
      drawExCtr(xBn, yBn, rv0);

      xAo = xAn,  yAo = yAn;
      xBo = (flt)xBn,  yBo = (flt)yBn;
    }
  }
#define TINY 0.05
  if (yBo == (flt)yBn)  {	/* i.e., we got a new mouse location */
    winset(widA);
    switch(Nvtx) {
    case  0:  /* erase old triangle vertex markers & draw new 1 */
      color(RED);
      circ(tetA[0][0] = xAn, tetA[0][1] = yAn, rv0);
      winset(widB);
      color(RED);	/* necessary to re-color in new window B */
      circ(FloB[0][0]=tetB[0][0]=(flt)xBn,FloB[0][1]=tetB[0][1]=(flt)yBn,rv0);
      Nvtx = 1;
      break;
    case  1:  /* just add vertex 2 */
      y10 = yAn - tetA[0][1];
      xovery10  = (ABS(y10) > TINY) ? (xAn - tetA[0][0])/y10 : 0.0F;
      color(YELLOW);
      circ(tetA[1][0] = xAn, tetA[1][1] = yAn, rv1);
      winset(widB);
      color(YELLOW);
      circ(FloB[1][0]=tetB[1][0]=(flt)xBn,FloB[1][1]=tetB[1][1]=(flt)yBn,rv1);
      Nvtx = 2;
      break;
    case  2:  /* just add vertex 2 */
      y21 = yAn - tetA[1][1];  		/* y2 - y1 in tetra A */
      xovery21 = (ABS(y21) > TINY) ? (xAn - tetA[1][0])/y21 : 0.0F;
      /** wrn(" ABS(y21): %f   xAn, tetA[1][0]:  %d  %f    so xovery21:  %f"
	 ,ABS(y21),xAn,tetA[1][0],xovery21);  **/
      x20 = (xAn - tetA[0][0]);
      y20 = (yAn - tetA[0][1]);  xovery20 = (ABS(y20) > TINY) ? x20/y20 : 0.0F;
      color(GREEN);
      circ(           tetA[2][0]=xAn,           tetA[2][1]=yAn, rv2);
      winset(widB);
      color(GREEN);
      circ(FloB[2][0]=tetB[2][0]=(flt)xBn,FloB[2][1]=tetB[2][1]=(flt)yBn, rv2);
      Nvtx = 3;
      break;
    case  3:  /* add vertex 3 and draw the quadrilateral */
      color(BLACK);
      poly2(4,tetA);
      drawmode(NORMALDRAW);
      cpack(FcRED);
      circ( tetA[0][0], tetA[0][1], rv0);
      cpack(FcYEL);
      circ( tetA[1][0], tetA[1][1], rv1);
      cpack(FcGRN);
      circ( tetA[2][0], tetA[2][1], rv2);
      cpack(FcMAG);
      circ( xAn, yAn, rv3);
      cpack(FcCYN);
      poly2(4,tetA);
      winset(widB);
      circ(FloB[3][0]=tetB[3][0]=(flt)xBn,FloB[3][1]=tetB[3][1]=(flt)yBn,rv1);
      /****
      drawmode(NORMALDRAW);
      cpack(FcCYN);
      poly2(4,tetB);
      ****/
      Nvtx = 4;
      break;
    }
  }
  drawmode(NORMALDRAW);
  winset(widA);
  drawmode(NORMALDRAW);
  curson();

  if   (Nvtx == 4)  {
    scalePoly2(sttA,tetA,4,1.0/zX,1.0/zY);
    scalePoly2(sttB,tetB,4,1.0/zX,1.0/zY);
    j = mB->ntet;
    mB->tetA[j][0][0] = sttA[0][0], mB->tetA[j][0][1] = sttA[0][1];
    mB->tetA[j][1][0] = sttA[1][0], mB->tetA[j][1][1] = sttA[1][1];
    mB->tetA[j][2][0] = sttA[2][0], mB->tetA[j][2][1] = sttA[2][1];
    mB->tetA[j][3][0] = sttA[3][0], mB->tetA[j][3][1] = sttA[3][1];
  }
}


void
moveCrsTet2 (void *v, int device, int flags)
{
pWmj    wB   = (pWmj)v, wA = wB->prev;
WmjAct *bp   = wA->act;
pMD     wwD  = (pMD)bp->data;
int     nB   = bp->nB;
pMOB    mB   = wwD->mob[nB];
/*
int     nA   = bp->nA;
pMOB    mA   = wwD->mob[nA];
*/
int     nvtx = Nvtx-1;
long    xBn,yBn, xBo,yBo, orgX, orgY, wd,ht;
float	dx, dy, dist, fxy[2];

  /* wrn("BEG moveCrsTet2: Nvtx %d, flags %d", Nvtx, flags); */

  if (Nvtx < 1)  return;



  /* widB = winget(); */
  getorigin(&orgX,&orgY);       /*getsize(&xsize,&ysize);*/
  getsize(&wd,&ht);     wd -= 1, ht -= 1;
  xBn  = (getvaluator(MOUSEX) - orgX);
  yBn  = (getvaluator(MOUSEY) - orgY);
  xBo  = (long)tetB[nvtx][0];
  yBo  = (long)tetB[nvtx][1];
  dx   = (xBn - xBo);
  dy   = (yBn - yBo);
  dist = dx*dx + dy*dy;
  if (dist > radSqr)   {
    wrn("moveCrsTet2: return cuz dist %f > %f",dist,radSqr); return;
  }

  cursoff();
  drawmode(OVERDRAW);        /* If no OVERDRAW, use PUPDRAW */
  while(getbutton(device)) {
    xBn = getvaluator(MOUSEX) - orgX; if (xBn < 0) xBn=0; else if (xBn > wd) xBn=wd;
    yBn = getvaluator(MOUSEY) - orgY; if (yBn < 0) yBn=0; else if (yBn > ht) yBn=ht;

    if (xBn != xBo || yBn != yBo) {

      color(BLACK);
      drawExCtr(xBo, yBo, rv0);
      color(RED);
      drawExCtr((long)tetA[nvtx][0], (long)tetA[nvtx][1], (long)rv0);   /* redraw X from A */
      color(GREEN);
      drawExCtr((long)FloB[nvtx][0], (long)FloB[nvtx][1], (long)rv0);   /* redraw orig flow X */
      drawExCtr(xBn, yBn, rv0);

      color(BLACK);
      switch (nvtx) {
        case  1:        /* rubber line from vertices 0 to 1 */
          bgnline();                    /* erase old line */
          v2f(tetB[0]);
          fxy[0] = xBo, fxy[1] = yBo;
          v2f(fxy);
          endline();
          color(RED);
          circ(tetB[0][0], tetB[0][1], rv0);	/* redraw original circle */
          color(GREEN);
          bgnline();                    	/* draw new line */
          v2f(tetB[0]);
          fxy[0] = xBn, fxy[1] = yBn;
          v2f(fxy);
          endline();
          break;

        case  2:        /* rubber line from vertices 0 to 1 to 2 */
          bgnline();
          v2f(tetB[0]);
          v2f(tetB[1]);
          fxy[0] = (float)xBo;
          fxy[1] = (float)yBo;
          v2f(fxy);
          endline();
          color(RED);
          circ(tetB[0][0], tetB[0][1], rv0);    /* redraw original circle */
          color(YELLOW);
          circ(tetB[1][0], tetB[1][1], rv1);    /* redraw original circle */
          color(GREEN);
          bgnline();                            /* draw new line */
          v2f(tetB[0]);
          v2f(tetB[1]);
          fxy[0] = (float)xBn;
          fxy[1] = (float)yBn;
          v2f(fxy);
          endline();
          break;

        case  3:
          poly2(4,tetB);
          tetB[3][0] = (flt)xBn, tetB[3][1] = (flt)yBn;
          color(GREEN);
          poly2(4,tetB);
          break;
      }
      xBo = xBn, yBo = yBn;
    }
  }

  tetB[nvtx][0] = (flt)xBn;
  tetB[nvtx][1] = (flt)yBn;

  dx   = (xBn - FloB[nvtx][0]);
  dy   = (yBn - FloB[nvtx][1]);
  dist = dx*dx + dy*dy;
  if (dist < 2.5) {
    /* wrn("moveFlowTet: HARDLY MOVED: (%.2f, %.2f) -> (%.2f, %.2f)"
	,FloB[nvtx][0],FloB[nvtx][1],tetB[nvtx][0],tetB[nvtx][1]); */
    tetB[nvtx][0] = FloB[nvtx][0];
    tetB[nvtx][1] = FloB[nvtx][1];
  }

  if (nvtx == 3) {
    color(BLACK);
    poly2(4,tetB);
    drawmode(NORMALDRAW);
    cpack(FcRED);
    circ( tetB[0][0], tetB[0][1], rv0);
    cpack(FcYEL);
    circ( tetB[1][0], tetB[1][1], rv1);
    cpack(FcGRN);
    circ( tetB[2][0], tetB[2][1], rv2);
    cpack(FcMAG);
    circ( tetB[3][0], tetB[3][1], rv3);
    cpack(FcCYN);
    poly2(4,tetB);
    scalePoly2(sttB, tetB, 4, (1.0/wB->zX) , (1.0/wB->zY) );
    xBn = mB->ntet;
    mB->tetB[xBn][0][0] = sttB[0][0], mB->tetB[xBn][0][1] = sttB[0][1];
    mB->tetB[xBn][1][0] = sttB[1][0], mB->tetB[xBn][1][1] = sttB[1][1];
    mB->tetB[xBn][2][0] = sttB[2][0], mB->tetB[xBn][2][1] = sttB[2][1];
    mB->tetB[xBn][3][0] = sttB[3][0], mB->tetB[xBn][3][1] = sttB[3][1];
    mB->ntet++;
  }
  else drawmode(NORMALDRAW);
  curson();
}


