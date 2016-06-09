/*****************************************************************************
 winDraw.c    S.R.Lines    93.8.17-9.21
 *****************************************************************************/
#include        <stdio.h>
#include        <stdlib.h>
#include        <math.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <gl/gl.h>
#include        <gl/device.h>
#include	<m/mj.h>
#include	<winLoop.h>
#include	<winDraw.h>

#define X       0
#define Y       1
#define MAXVTC  1024


int
reordPoly2 ( flt rA[][2], flt rB[][2], flt oA[][2], flt oB[][2], int nvt)
{
int     j,n,nMin = 0;
flt     y,  yMin = oA[0][1];

  for (j = 1; j < nvt; j++) {
    y = oA[j][1];
    if (y < yMin)  yMin = y, nMin = j;
  }

  for (n = nMin, j = 0; j < nvt; j++, n++) {
    if (n == nvt) n = 0;
    rA[j][0] = oA[n][0];
    rA[j][1] = oA[n][1];
    rB[j][0] = oB[n][0];
    rB[j][1] = oB[n][1];
  }
return  nMin;
}


double polyDiff(float pA[][2], float pB[][2], int nvt)
{
double  dx,dy,dd = 0.0;

  while (--nvt > 0) {
    dx = pA[nvt][0] - pB[nvt][0];
    dy = pA[nvt][1] - pB[nvt][1];
    dd += sqrt(dx*dx + dy*dy);
  }
  return dd;
}

int
reorderPoly ( float rord[][2], float poln[][2], int nvt)
{
int     j,n,nMin = 0;
float   y,  yMin = poln[0][1];

  for (j = 1; j < nvt; j++) {
    y = poln[j][1];
    if (y < yMin)  yMin = y, nMin = j;
  }
  for (n = nMin, j = 0; j < nvt; j++, n++) {
    if (n == nvt) n = 0;
    rord[j][0] = poln[n][0];
    rord[j][1] = poln[n][1];
  }
return  nMin;
}

// Why does only this one take longs instead of floats for coords?
void drawExCtr (long xx, long yy, long rad)
{
  long xy[2];
  bgnline();
  xy[0] = xx - rad;   xy[1] = yy - rad;   v2i(xy);
  xy[0] = xx + rad;   xy[1] = yy + rad;   v2i(xy);
  endline();
  bgnline();
  xy[0] = xx - rad; /*xy[1] = yy + rad;*/ v2i(xy);
  xy[0] = xx + rad;   xy[1] = yy - rad;   v2i(xy);
  endline();
}


void drawTet (float vtx[ ][2])
{ /* fprintf(stderr,"drawTet: (%.1f , %.1f) (%.1f , %.1f) (%.1f , %.1f)\n"
        ,vtx[0][0],vtx[0][1],vtx[1][0],vtx[1][1],vtx[2][0],vtx[2][1]); */
#if 1
  poly2(4,vtx);
#else
  bgnclosedline();
  v2f(vtx[0]);
  v2f(vtx[1]);
  v2f(vtx[2]);
  v2f(vtx[3]);
  endclosedline();
#endif
}


void
scaleOffsetPoly2 (float dstp[][2], float srcp[][2], unt npts
	 , double xfac, double yfac, double xoff, double yoff)
{
  while (npts--) {
    dstp[npts][0] = xoff + xfac * srcp[npts][0];
    dstp[npts][1] = yoff + yfac * srcp[npts][1];
  }
}



void
scalePoly2 (flt dstp[][2], flt srcp[][2], unt npts, dbl xfac, dbl yfac)
{
  while (npts--) {
    dstp[npts][0] = xfac * srcp[npts][0];
    dstp[npts][1] = yfac * srcp[npts][1];
  }
}




double
rectBoundsFromPoly (float rectp[4], float polyp[][2], int npts)
{
register float ff;
float   xL = polyp[0][0], yL = polyp[0][1];
float   xU = xL, yU = yL;
int     j = npts;

  for (j = 1; j < npts; j++) {
    ff = polyp[j][0];
    if       (ff < xL)  xL = ff;
    else if  (ff > xU)  xU = ff;
    ff = polyp[j][1];
    if       (ff < yL)  yL = ff;
    else if  (ff > yU)  yU = ff;
  }
  rectp[0] = xL;
  rectp[1] = yL;
  rectp[2] = xU;
  rectp[3] = yU;
  return (xU - xL)*(yU - yL);   /* area of bounding rectangle */
}


#if 00
static int drawVlst (void *vp)
{
pWmj    W = (pWmj)vp;
pVlst   V = W->V;
short  *vlst = V->vlst;
long    j,nv;
short   *sxy;

  if ((nv = V->nvts) > 0) {

    /* SHOULD DELETE FOLLOWING TEST: */
    if (nv > MAXVTC) {
       wrn("V->nvts:%d > MAXVTC:%d",nv,MAXVTC);
       nv = MAXVTC;
    }

/****
    reshapeviewport();
    if (W->P) lrectwrite(0,0,D->w-1,D->h-1,W->P);
****/
    color(WHITE);               /* prepare to draw white lines */

    bgnline();
      for (j = 0; j < nv; j += 2) {
        sxy = &vlst[j];
        if (sxy[X] < 0)  { endline(); bgnline(); }
        else             v2s(sxy);
      }
    endline();
  }
  return (nv);
}
#endif



#if 000
void
addpt(pWmj W, short mval[], short lastval[], long leftMouseDown)
{
pVlst   V  =  W->V;
short  *v;

  if (V) {
    v = V->vlst;
    if (leftMouseDown) {
      v[V->nvts++] = mval[X];
      v[V->nvts++] = mval[Y];
      bgnline();
        v2s(lastval);
        v2s(mval);
      endline();
    }
    else if (v[V->nvts-2] >= 0) {
      /** wrn("addpt: Ending line %d, vt %d,  ORG %4d %4d    PNT %4d %4d"
                         ,k,V->nvts,Xo,Yo,mval[X],mval[Y]); k++; **/
      v[V->nvts++] = -4096;
      v[V->nvts++] = -4096;
    }
    if (V->nvts > MAXVTC)  {
      int       j;
      for (j = 0; j < MAXVTC; j += 4)
        printf("%4d:\t%4d %4d\t\t%4d %4d\n", j, v[j], v[j+1], v[j+2], v[j+3]);
      die("Woops!  MAXVTC=%d exceeded!\n",MAXVTC);
    }
  }
  else {
    V = W->V = (pVlst)calloc(1,sizeof(Vlst));
    V->vlst  = v = (short *)calloc(MAXVTC+2,sizeof(short));
    V->glfn  = &drawVlst;
    V->stat  = 1;
    v[0]     = mval[X];
    v[1]     = mval[Y];
    V->nvts  = 2;
  }
  /**************************
  lastval[X] = mval[X];
  lastval[Y] = mval[Y];
  **************************/
  return;
}
#endif


