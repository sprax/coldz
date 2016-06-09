/*****************************************************************************
 winNew.c    S.R.Lines    95.5.14
 NB: Keep the SWAP_0_AND_1 case around to add multiple chain features later.
 *****************************************************************************/
#include        <stdio.h>
#include        <math.h>
#include        <un/unBug.h>
#include        <un/unMacro.h>
#include        <un/unTypes.h>
#include        <gl/gl.h>
#include        <gl/device.h>
#include	<m/mj.h>
#include	<winLoop.h>
#include	<winDraw.h>

#define TINY 0.05
#define REP_0_WITH_2  0
#define REP_1_WITH_2  1
#define REPLACE_NONE  2
#define SWAP_0_AND_1  4		/* @@ get rid of this case */

static long     rv0 =  11;
static long     rv1 =  10; 	/* diag   =  14.142136; */
static long     rv2 =   9;
/*
static long     rv3 =   8;
static char	str[128];
*/
static	long	xA0,yA0, xA1,yA1, xA2,yA2;
static	dbl	the10, cos10, sin10, lic10;
static	dbl	the21, cos21, sin21, lic21;
static	dbl	the32, cos32, sin32, lic32;
static void drawTanLines(dbl length);

void
testChain (void *v, int device, int flags)
{
static  int     mouseUp=1, nextRep = REPLACE_NONE;
static	long	xAo,yAo;
static  long	triA[4][2];
pWmj    cW  = (pWmj)v;
pWmj    pW  = (cW->prev ? cW->prev : cW);
WmjAct *Ap  = (device == LEFTMOUSE ? cW->lefMouse : cW->midMouse);
#ifdef II
Tetra  *trp = Ap->data;
#else
FloXY  *flo = Ap->data;
Tetra  *trp = flo->tetra;
#endif
int 	ntr = trp->ntr, nvt = trp->nvt;
pDmj	cD  = &cW->D;
float	spol[3][2];
double	xfac = 1.0/cW->zX;
double	yfac = 1.0/cW->zY;
double	xoff = cD->l;
double  yoff = cD->b;
long    widP = pW->id, widN = cW->next->id, widA = winget();

int 	j; /* km = fD->x, jm = fD->y, kM = km + fD->w - 1, jM = jm + fD->h - 1; */
long	xAn,yAn= -9999, orgX, orgY, wd, ht, ixy[2];

  if ((mouseUp = !mouseUp))  return;

  if (!nvt) wrn("\ntestChain: lb: %d %d, czXY: %.1f %.1f  pzXY: %.1f %.1f"
	,cD->l,cD->b,cW->zX,cW->zY,pW->zX,pW->zY);

  getorigin(&orgX,&orgY);	
  getsize(&wd,&ht);	wd -= 1, ht -= 1;
  cursoff();
  drawmode(OVERDRAW);
  while(getbutton(device)) {
    xAn = getvaluator(MOUSEX) - orgX; if (xAn < 0) xAn=0; else if (xAn > wd) xAn=wd;
    yAn = getvaluator(MOUSEY) - orgY; if (yAn < 0) yAn=0; else if (yAn > ht) yAn=ht;
    if (xAn != xAo || yAn != yAo) {

      color(BLACK);
      drawExCtr( xAo, yAo, rv0);
      switch (nvt) {
        case  0: break;
        case  1:	/* rubber line from vertices 0 to 1 */
          bgnline();
            v2i(triA[0]); ixy[0] = xAo, ixy[1] = yAo;
            v2i(ixy);
          endline();
          color(GREEN);
          bgnline();
            v2i(triA[0]); ixy[0] = xAn, ixy[1] = yAn;
            v2i(ixy);
          endline();
          break;

        case  2:        	/* triangle: rubber from vert 1 to 2 & 2 to 0 */
          poly2i(3,triA);	/* always erase overdraw: normal drawn tri remains */
          triA[2][0] = xAn, triA[2][1] = yAn;
          color(GREEN);
          poly2i(3,triA);
          break;

        default:  		/* make new triangle by replacing third vertex */
          poly2i(3,triA);
          switch (nextRep)  {	/* prefer to stay on current side */
            case REP_0_WITH_2:
              if (yAn*cos21 - xAn*sin21 >= lic21)             nextRep = REPLACE_NONE; break;
            case REP_1_WITH_2:
              if (yAn*cos32 - xAn*sin32 >= lic32)             nextRep = REPLACE_NONE; break;
            default:				               /* was:  REPLACE_NONE: */
              if (yAn*cos21 - xAn*sin21 <  lic21) {
                nextRep = REP_0_WITH_2;
                triA[0][0] = xA2, triA[0][1] = yA2;
                triA[1][0] = xA1, triA[1][1] = yA1;
              } else if (yAn*cos32 - xAn*sin32 < lic32) {
                nextRep = REP_1_WITH_2;
                triA[0][0] = xA0, triA[0][1] = yA0;
                triA[1][0] = xA2, triA[1][1] = yA2;
              } else if (trp->ntr == 1 && yAn*cos10 - xAn*sin10 < lic10) {
                nextRep = SWAP_0_AND_1;     /** case REPLACE_NONE -OR- case SWAP_0_AND_1: **/
                triA[0][0] = xA1, triA[0][1] = yA1;
                triA[1][0] = xA0, triA[1][1] = yA0;
              } else
                nextRep = REPLACE_NONE;
              break;
          } /** end of switch (nextRep) **/
          if (nextRep == REPLACE_NONE)
            curson();
          else	{
            cursoff();
            color(GREEN); triA[2][0] = xAn, triA[2][1] = yAn;
            poly2i(3,triA);
          }
          break;
      }
      color(RED);             		/* set color of new X */
      drawExCtr( xAn,  yAn, rv0);
      xAo = xAn, yAo = yAn;
    }
  }
  if (yAo != yAn)  /* i.e.mouse didn't move */
    goto RETURN;

  switch(nvt) {
    case  0:  /* erase old triangle vertex markers & draw new 1 */
      triA[0][0] = xAn, triA[0][1] = yAn;		/* @@ necessary ? */
      /* drawmode(NORMALDRAW); cpack(FcGRN); */
      color(GREEN);
      circi( xA0 = xAn,   yA0      = yAn, rv0);
      break;
    case  1:  			/* just add second vertex */
      triA[1][0] = xAn, triA[1][1] = yAn;		/* @@ necessary ? */
      /* drawmode(NORMALDRAW); cpack(FcYEL); */
      color(YELLOW);
      circi( xA1 = xAn,   yA1      = yAn, rv1);
      break;

    default:  			/* add third vertex and draw the triangle */
      color(BLACK);		/* @@ would like to eliminate this & turn on NORMALDRAW 1st */
      poly2i(3,triA);
      drawmode(NORMALDRAW);
      if (trp->ntr) switch (nextRep) {
        case REP_0_WITH_2: xA0 = xA2, yA0 = yA2; 		   break;  /* New 10 is old -21 */
        case REP_1_WITH_2: xA1 = xA2, yA1 = yA2;		   break;
        case REPLACE_NONE: fprintf(stderr,"~");  goto RETURN;     break;  /* could just return @@ */
        case SWAP_0_AND_1: SWAP(xA0,xA1,yA2);  SWAP(yA0,yA1,xA2);  break;  /* get rid of this case */
        default: die("what's replace case %d?",nextRep);
      }
      else {	/* Make sure first tringle is positively oriented (counterclockwise). */
        if ((j = (xA1-xA0)*(yAn-yA0) - (yA1-yA0)*(xAn-xA0)) < 0) { /* neg-orient tri */ 
          SWAP(xA1,xAn,yA2);
          SWAP(yA1,yAn,xA2);
        }
        /** fprintf(stderr,"\nFIRST TRI nextRep: %d,  z: %d", nextRep,j); **/
        cpack(FcGRN);
        circi(xA0, yA0, rv0);
        cpack(FcYEL);
        circi(xA1, yA1, rv1);
        cpack(FcRED);
        circi(xAn, yAn, rv2);
      }
      /** fprintf(stderr,"\nREPLACED: %d", nextRep); **/
      nextRep = REPLACE_NONE;

      xA2 = xAn, yA2 = yAn;
      triA[0][0] = xA0;  trp->tetA[ntr][0][0] = xA0*xfac + xoff;
      triA[0][1] = yA0;  trp->tetA[ntr][0][1] = yA0*yfac + yoff;
      triA[1][0] = xA1;  trp->tetA[ntr][1][0] = xA1*xfac + xoff;
      triA[1][1] = yA1;  trp->tetA[ntr][1][1] = yA1*yfac + yoff;
      triA[2][0] = xA2;  trp->tetA[ntr][2][0] = xA2*xfac + xoff;
      triA[2][1] = yA2;  trp->tetA[ntr][2][1] = yA2*yfac + yoff;

      /* Recompute all angles, though in the default/generic case, only 2 change: */

      the10 = atan2f((yA1 - yA0), (xA1 - xA0));	
      cos10 = cos(the10); sin10 =  sin(the10);
      lic10 = yA0*cos10         -  xA0*sin10;

      the21 = atan2f((yA2 - yA1), (xA2 - xA1));
      cos21 = cos(the21); sin21 =  sin(the21);
      lic21 = yA1*cos21         -  xA1*sin21;

      the32 = atan2f((yA0 - yA2), (xA0 - xA2));
      cos32 = cos(the32); sin32 =  sin(the32);
      lic32 = yA2*cos32         -  xA2*sin32;
      /** 
      wrn("xy0 xy1 lic10:  %5.1f %5.1f   %5.1f %5.1f   % 9.3f",  xA0,yA0,xA1,yA1,lic10);
      wrn("xy1 xy2 lic21:  %5.1f %5.1f   %5.1f %5.1f   % 9.3f",  xA1,yA1,xA2,yA2,lic21);
      wrn("xy2 xy3 lic32:  %5.1f %5.1f   %5.1f %5.1f   % 9.3f\n",xA2,yA2,xA0,yA0,lic32);
      **/
      cpack(FcCYN);
      poly2i(3,triA);

winset(widN);
cpack(FcCLR[nvt % 6]);
poly2i(3,triA);
winset(widA);

#if 0
      drawTanLines(50.0);
      cpack(FcYEL);
      drawBisector(triA, 50.0);
      cpack(FcWHT);
      drawMedLine(triA);
#endif
      ntr++;
      break;
  }
  nvt++;

winset(widP);
  /* drawmode(OVERDRAW); color(YELLOW); */
  drawmode(NORMALDRAW);  cpack(HcCLR[ntr % 6]);
  scalePoly2(spol, trp->tetA[j], 3, pW->zX, pW->zY);
  /* wrn("spol %d:  %8.2f %8.2f  %8.2f %8.2f  %8.2f %8.2f"
	,ntr,spol[0][0],spol[0][1],spol[2][0],spol[2][1],spol[2][0],spol[2][1]); */
  poly2(3,spol);
  drawmode(NORMALDRAW);
winset(widA);

RETURN:
  trp->nvt = nvt;
  trp->ntr = ntr;
  drawmode(NORMALDRAW);
  curson();
}



static void
drawTanLines(dbl length)
{
flt     xy[2], dx, dy;

  dx = length*cos(the21);   dy = length*sin(the21);
  bgnline();
    cpack(FcRED); xy[0] = xA2 + dx;   xy[1] = yA2 + dy;   v2f(xy);
    cpack(FcGRN); xy[0] = xA2 - dx;   xy[1] = yA2 - dy;   v2f(xy); /* inside */
  endline();
  bgnline();
    cpack(FcYEL); xy[0] = xA1 + dx;   xy[1] = yA1 + dy;   v2f(xy); /* inside */
    cpack(FcBLU); xy[0] = xA1 - dx;   xy[1] = yA1 - dy;   v2f(xy);
  endline();
}

#if 	0000 /*_____________________________________________________________*/

static void
drawMedLine(float trip[3][2])
{
flt     xyo[2], xym[2];
  xym[0] = (trip[0][0]     + trip[1][0])*0.5;
  xym[1] = (trip[0][1]     + trip[1][1])*0.5;
  xyo[0] = (trip[2][0]*2.0 - xym[0]);
  xyo[1] = (trip[2][1]*2.0 - xym[1]);
  bgnline();
    v2f(xym);
    v2f(xyo);
  endline();
}

static void
drawBisector(float trip[3][2], dbl length)
{
float   xy[2];
dbl     dx, dy, theta, xx = trip[2][0], yy = trip[2][1];
  theta = 0.5 * ( atan2f((trip[1][1] - yy), (trip[1][0] - xx))
                + atan2f((trip[0][1] - yy), (trip[0][0] - xx)) );
  dx = length*cos(theta);   dy = length*sin(theta);
  bgnline();
    xy[0] = xx - dx;   xy[1] = yy - dy;   v2f(xy);
    xy[0] = xx + dx;   xy[1] = yy + dy;   v2f(xy);
  endline();
}

#endif	/* 0000 */


