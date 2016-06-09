/*****************************************************************************
 inCrsTri.c    S.R.Lines    95.5.1-21
 NB: Keep the SWAP_0_AND_1 case around to add multiple chain features later.
 *****************************************************************************/
#include        <stdio.h>
#include        <math.h>
#include        <un/unBug.h>
#include        <un/unMacro.h>
#include        <un/unTypes.h>
#include        <un/unFlags.h>
#include        <gl/gl.h>
#include        <gl/device.h>
#include	<m/mj.h>
#include	<winLoop.h>
#include	<winDraw.h>
#include        "mob.h"
#include        "inParm.h"

#define TINY 0.05
#define REP_0_WITH_2  0
#define REP_1_WITH_2  1
#define REPLACE_NONE  2
#define SWAP_0_AND_1  4		/* @@ get rid of this case */

#define rv0		11
static long     rv1 =   10; 	/* diag   =  14.142136; */
static long     rv2 =    9;
/*
static long     rv3 =    8;
static char	str[256];
*/
static	unl	Uclr = FcRED;
static	flt     radSqr = 100.0;
static	long	xB0,yB0, xB1,yB1, xB2,yB2;
static  long	triA[4][2], triB[4][2];
static  long    xA0,yA0, xA1,yA1, xA2,yA2;
static  long    xB0,yB0, xB1,yB1, xB2,yB2;
static	long	xBo,yBo, xBA,yBA;

void
markCrsTri2 (void *v, int device, int flags)
{
static	dbl	theA10, cosA10, sinA10, licA10;
static	dbl	theA21, cosA21, sinA21, licA21;
static	dbl	theA32, cosA32, sinA32, licA32;
static  int     nextRep = REPLACE_NONE;
static	long	xAo,yAo;
pWmj    wA   = (pWmj)v, wB = wA->next, wP = wA->prev;
WmjAct *ap   = wA->act;
pMD     wwD  = (pMD)ap->data;
int     nA   = ap->nA;	pMOB  mA = wwD->mob[nA];
int 	nB   = ap->nB;	pMOB  mB = wwD->mob[nB];
ppf     floX = mB->foX;
ppf     floY = mB->foY;
pDmj    fD   = mB->fD;
int 	ntr  = mB->ntri, nvt = mB->nvrt;
pDmj	cD   = &wA->D;
dbl	zX   = wA->zX, zY   = wA->zY;
dbl	rX   = 1.0/zX, rY = 1.0/zY;
dbl	xoff =     cD->l,  yoff =     cD->b;
long    widP = wP->id, widB = wA->next->id, widA = winget();
int     j, k, km = fD->x, jm = fD->y, kM = km + fD->w - 1, jM = jm + fD->h - 1;
float	spol[3][2];
long	xBn=9999,yBn=9999, orgX, orgY, wd, ht, ixy[2];
register long	xAn,yAn;
#define BLINE(xy,x,y)	bgnline(); v2i(xy); ixy[0]=x, ixy[1]=y; v2i(ixy); endline()

/*
  if (!nvt)
    wrn("markCrsTri2: lb: %d %d, czXY: %.1f %.1f  pzXY: %.1f %.1f xyoff: %.3f %.3f"
       ,cD->l,cD->b,wA->zX,wA->zY,wP->zX,wP->zY, xoff, yoff);
 */

  getorigin(&orgX,&orgY);	
  getsize(&wd,&ht);	wd -= 1, ht -= 1;
  cursoff();				/* off only in window A */
  drawmode(OVERDRAW);
  winset(widB);
  if (nvt > 2 && nvt >= mB->sema) {
    mB->sema = nvt;
    cpack(Uclr); poly2i(3,triB);
    if (Devopts & O_WARN && Options & O_VERB) {
      winset(wB->prev->id);
      cpack(Uclr);
      scalePoly2(spol, mB->triB[ntr-1], 3, wB->prev->zX, wB->prev->zY);
      wrn("spol %d:  %8.2f %8.2f  %8.2f %8.2f  %8.2f %8.2f"
         ,ntr-1,spol[0][0],spol[0][1],spol[2][0],spol[2][1],spol[2][0],spol[2][1]);
      poly2(3,spol);
      winset(widB);
    }
  }
  drawmode(OVERDRAW);
  
  while(getbutton(device)) {
    xAn = getvaluator(MOUSEX) - orgX; if (xAn < 0) xAn=0; else if (xAn > wd) xAn=wd;
    yAn = getvaluator(MOUSEY) - orgY; if (yAn < 0) yAn=0; else if (yAn > ht) yAn=ht;
    if (xAn != xAo || yAn != yAo) {

      k = (flt)(xAn*rX + xoff + 0.5F);
      j = (flt)(yAn*rY + yoff + 0.5F);
      if (k < km)  k = km;  else if (k > kM)  k = kM;
      if (j < jm)  j = jm;  else if (j > jM)  j = jM;
      xBn = xAn - zX * floX[j][k];
      yBn = yAn - zY * floY[j][k];
      if (xBn < 0)  xBn = 0;  else if (xBn > wd)  xBn = wd;
      if (yBn < 0)  yBn = 0;  else if (yBn > ht)  yBn = ht;

      color(BLACK);
      drawExCtr( xBo, yBo, rv0);
      drawExCtr( xAo, yAo, rv0);

      winset(widA);
      color(BLACK);
      drawExCtr( xAo, yAo, rv0);

      switch (nvt) {
        case  0: break;
        case  1:	/* rubber line from vertices 0 to 1 */
          BLINE(triA[0],xAo,yAo); color(GREEN); BLINE(triA[0],xAn,yAn);
         winset(widB);
          BLINE(triB[0],xBo,yBo); color(GREEN); BLINE(triB[0],xBn,yBn);
          break;

        case  2:        	/* triangle: rubber from vert 1 to 2 & 2 to 0 */
          poly2i(3,triA);	/* always erase overdraw: normal drawn tri remains */
          triA[2][0] = xAn, triA[2][1] = yAn;
          color(GREEN);
          poly2i(3,triA);
         winset(widB);
          poly2i(3,triB);	/* always erase overdraw: normal drawn tri remains */
          triB[2][0] = xBn, triB[2][1] = yBn;
          color(GREEN);
          poly2i(3,triB);
          break;

        default:  		/* make new triangle by replacing third vertex */
          poly2i(3,triA);
          winset(widB);
          poly2i(3,triB);
          switch (nextRep)  {	/* prefer to stay on current side */
            case REP_0_WITH_2:
              if (yAn*cosA21 - xAn*sinA21 >= licA21)  nextRep = REPLACE_NONE; break;
            case REP_1_WITH_2:
              if (yAn*cosA32 - xAn*sinA32 >= licA32)  nextRep = REPLACE_NONE; break;
            default:          /** case SWAP_0_AND_1-OR- case REPLACE_NONE : **/
              if (yAn*cosA21 - xAn*sinA21 <  licA21) {
                nextRep = REP_0_WITH_2;
                triA[0][0] = xA2, triA[0][1] = yA2, triA[1][0] = xA1, triA[1][1] = yA1;
                triB[0][0] = xB2, triB[0][1] = yB2, triB[1][0] = xB1, triB[1][1] = yB1;
              } else if (yAn*cosA32 - xAn*sinA32 < licA32) {
                nextRep = REP_1_WITH_2;
                triA[0][0] = xA0, triA[0][1] = yA0, triA[1][0] = xA2, triA[1][1] = yA2;
                triB[0][0] = xB0, triB[0][1] = yB0, triB[1][0] = xB2, triB[1][1] = yB2;
              } else if (nvt == 3 && yAn*cosA10 - xAn*sinA10 < licA10) {
                nextRep = SWAP_0_AND_1;
                triA[0][0] = xA1, triA[0][1] = yA1, triA[1][0] = xA0, triA[1][1] = yA0;
                triB[0][0] = xB1, triB[0][1] = yB1, triB[1][0] = xB0, triB[1][1] = yB0;
              } else
                nextRep = REPLACE_NONE;
              break;
          } /** end of switch (nextRep) **/
          winset(widA);
          if (nextRep == REPLACE_NONE)
            curson();
          else	{
            cursoff();    color(GREEN); triA[2][0]=xAn, triA[2][1]=yAn; poly2i(3,triA);
            winset(widB); color(GREEN); triB[2][0]=xBn, triB[2][1]=yBn; poly2i(3,triB);
          }
          break;
      }

      winset(widA);
      color(RED);   drawExCtr( xAn, yAn, rv0);

      winset(widB);
      color(RED);   drawExCtr( xAn, yAn, rv0);
      color(GREEN); drawExCtr( xBn, yBn, rv0);

      xAo = xAn,  yAo = yAn;
      xBo = xBn,  yBo = yBn;
    }
  }
  if (yAo != yAn)  /* i.e. the mouse didn't move */
    goto RETURN;

  winset(widA);
  switch(nvt) {
    case  0:  /* erase old triangle vertex markers & draw new 1 */
      triA[0][0] = xA0 = xAn, triA[0][1] = yA0 = yAn;
      triB[0][0] = xB0 = xBn, triB[0][1] = yB0 = yBn;
      color(GREEN);	circi(xAn, yAn, rv0);
      winset(widB);
      color(GREEN);	circi(xBn, yBn, rv0);
      mB->sema = 0;
      break;
    case  1:  			/* just add second vertex */
      triA[1][0] = xA1 = xAn, triA[1][1] = yA1 = yAn;
      triB[1][0] = xB1 = xBn, triB[1][1] = yB1 = yBn;
      color(YELLOW);	circi(xAn, yAn, rv1);
      winset(widB);
      color(YELLOW);	circi(xBn, yBn, rv1);
      break;

    default:  				/* add third vertex and draw the triangle */
      color(BLACK);	poly2i(3,triA);		/* erase old overdrawn triangle */
      winset(widB);
      color(BLACK);	poly2i(3,triB);
      if (nvt > 2) {
        if (nextRep == REPLACE_NONE) {
          fprintf(stderr,"<@>");
          mB->sema = nvt+1;
          goto RETURN; 
        }
        winset(widA);
        color(BLACK);
        circi(xA0, yA0, rv0);
        circi(xA1, yA1, rv1);
        circi(xA2, yA2, rv2);
        winset(widB);
        color(BLACK);
        circi(xB0, yB0, rv0);
        circi(xB1, yB1, rv1);
        circi(xB2, yB2, rv2);
        switch (nextRep) {
          case REP_0_WITH_2: xA0 = xA2, yA0 = yA2, xB0 = xB2, yB0 = yB2;	break;
          case REP_1_WITH_2: xA1 = xA2, yA1 = yA2, xB1 = xB2, yB1 = yB2;	break;
          case SWAP_0_AND_1: SWAP(xA0,xA1,yA2);  SWAP(yA0,yA1,xA2);  
                             SWAP(xB0,xB1,yB2);  SWAP(yB0,yB1,xB2);  break;
          default: die("what's replace case %d?",nextRep);
        }
      }
      else {	/* Make sure first tringle is positively oriented (counterclockwise). */
        if ((j = (xA1-xA0)*(yAn-yA0) - (yA1-yA0)*(xAn-xA0)) < 0) { /* neg-orient tri */ 
          /** fprintf(stderr,"\nFIRST TRI nextRep: %d,  z: %d", nextRep,j); **/
          SWAP(xA1,xAn,xAo); SWAP(yA1,yAn,yAo); 	/* so xAo = xAn, still */
          SWAP(xB1,xBn,xBo); SWAP(yB1,yBn,yBo);
        }
      }
      /** fprintf(stderr,"\nREPLACED: %d", nextRep); **/
      nextRep = REPLACE_NONE;

      xA2 = xAn, yA2 = yAn;
      triA[0][0] = xA0;  mB->triA[ntr][0][0] = xA0*rX + xoff;
      triA[0][1] = yA0;  mB->triA[ntr][0][1] = yA0*rY + yoff;
      triA[1][0] = xA1;  mB->triA[ntr][1][0] = xA1*rX + xoff;
      triA[1][1] = yA1;  mB->triA[ntr][1][1] = yA1*rY + yoff;
      triA[2][0] = xA2;  mB->triA[ntr][2][0] = xA2*rX + xoff;
      triA[2][1] = yA2;  mB->triA[ntr][2][1] = yA2*rY + yoff;
      xB2 = xBA = xBn,   yB2 = yBA = yBn;
      triB[0][0] = xB0;  mB->triB[ntr][0][0] = xB0*rX + xoff;
      triB[0][1] = yB0;  mB->triB[ntr][0][1] = yB0*rY + yoff;
      triB[1][0] = xB1;  mB->triB[ntr][1][0] = xB1*rX + xoff;
      triB[1][1] = yB1;  mB->triB[ntr][1][1] = yB1*rY + yoff;
      triB[2][0] = xB2;  mB->triB[ntr][2][0] = xB2*rX + xoff;
      triB[2][1] = yB2;  mB->triB[ntr][2][1] = yB2*rY + yoff;

      /* Recompute all 3 angles, though in the default/generic case, only 2 change: */

      theA10 = atan2f((yA1 - yA0), (xA1 - xA0));	
      cosA10 = cos(theA10); sinA10 =  sin(theA10);
      licA10 = yA0*cosA10         -  xA0*sinA10;

      theA21 = atan2f((yA2 - yA1), (xA2 - xA1));
      cosA21 = cos(theA21); sinA21 =  sin(theA21);
      licA21 = yA1*cosA21         -  xA1*sinA21;

      theA32 = atan2f((yA0 - yA2), (xA0 - xA2));
      cosA32 = cos(theA32); sinA32 =  sin(theA32);
      licA32 = yA2*cosA32         -  xA2*sinA32;
      /** 
      wrn("xy0 xy1 licA10:  %5.1f %5.1f   %5.1f %5.1f   % 9.3f",  xA0,yA0,xA1,yA1,licA10);
      wrn("xy1 xy2 licA21:  %5.1f %5.1f   %5.1f %5.1f   % 9.3f",  xA1,yA1,xA2,yA2,licA21);
      wrn("xy2 xy3 licA32:  %5.1f %5.1f   %5.1f %5.1f   % 9.3f\n",xA2,yA2,xA0,yA0,licA32);
      **/


      winset(widA);
      color(GREEN);	circi(xA0, yA0, rv0);
      color(YELLOW);	circi(xA1, yA1, rv1);
      color(RED);	circi(xAn, yAn, rv2);
      drawmode(NORMALDRAW);
      Uclr = FcCLR[ntr % 6];  	cpack(Uclr);	poly2i(3,triA);

      winset(widB);
      /* drawmode(OVERDRAW); */
      color(GREEN);	circi(xB0, yB0, rv0);
      color(YELLOW);	circi(xB1, yB1, rv1);
      color(RED);	circi(xBn, yBn, rv2);
      /* drawmode(NORMALDRAW); cpack(Uclr); */
      /* color(ntr % 6 + 1); */	color(GREEN);	poly2i(3,triB);

#if 0
      drawTanLines(50.0);
      cpack(FcYEL);
      drawBisector(triA, 50.0);
      cpack(FcWHT);
      drawMedLine(triA);
#endif

      if (Devopts & O_WARN && Options & O_VERB) {
        winset(widP);
        /* drawmode(OVERDRAW); color(YELLOW); */
        /* drawmode(NORMALDRAW);  */
        cpack(Uclr);
        scalePoly2(spol, mB->triA[ntr], 3, wP->zX, wP->zY);
        wrd("spol %d:  %8.2f %8.2f  %8.2f %8.2f  %8.2f %8.2f"
             ,ntr,spol[0][0],spol[0][1],spol[2][0],spol[2][1],spol[2][0],spol[2][1]);
        poly2(3,spol);
        /* drawmode(NORMALDRAW); */
      }
      ntr++;
      break;
  }
  nvt++;

RETURN:
  mB->nvrt = nvt;
  mB->ntri = ntr;
  winset(widB);
  drawmode(NORMALDRAW);
  winset(widA);
  drawmode(NORMALDRAW);
  curson();
}


void
moveCrsTri2 (void *v, int device, int flags)
{
static	dbl	theB10, cosB10, sinB10, licB10;
static	dbl	theB21, cosB21, sinB21, licB21;
static	dbl	theB32, cosB32, sinB32, licB32;
pWmj    wB   = (pWmj)v, wA = wB->next, wP = (wB->prev ? wB->prev : wB);
WmjAct *ap   = wB->act;
pMD     wwD  = (pMD)ap->data;
int     nA   = ap->nA;  pMOB  mA = wwD->mob[nA];
int     nB   = ap->nB;  pMOB  mB = wwD->mob[nB];
int     ntr  = mB->ntri, nvt = mB->nvrt, wrongSide = 0;
pDmj	cD   = &wB->D;
dbl	zX   = wB->zX, zY   = wB->zY;
dbl	rX   = 1.0/zX, rY = 1.0/zY;
dbl	xoff = cD->l, yoff = cD->b;
long    widP = wP->id, widB = winget();
float	dx, dy, dist, spol[3][2];
long	j, orgX, orgY, wd, ht, ixy[2];
long	xBn,yBn;

  if (nvt && mB->sema > nvt)  { wrn("mB->sema(%d) >= nvt(%d)",mB->sema,nvt); return; }

  getsize(&wd,&ht);	wd -= 1, ht -= 1;
  /* winset(widB); */
  getorigin(&orgX,&orgY);	
  xBn  = (getvaluator(MOUSEX) - orgX);
  yBn  = (getvaluator(MOUSEY) - orgY);
  dx   = (xBn - xBo);
  dy   = (yBn - yBo);
  dist = dx*dx + dy*dy;

  /* wrn("BEGIN moveCrsTri2...  dist: %.3f", dist); */
  if (dist > radSqr)  return;

  if (nvt > 1) { /* Compute only the angle from vertex 0 to 1 */

    theB10 = atan2f((yB1 - yB0),  (xB1 - xB0));
    cosB10 = cos(theB10); sinB10 = sin(theB10);
    licB10 = yB0*cosB10          - xB0*sinB10;
  }

  cursoff();
  drawmode(OVERDRAW);

  while(getbutton(device)) {
    xBn = getvaluator(MOUSEX) - orgX; if (xBn < 0) xBn=0; else if (xBn > wd) xBn=wd;
    yBn = getvaluator(MOUSEY) - orgY; if (yBn < 0) yBn=0; else if (yBn > ht) yBn=ht;
    if (xBn != xBo || yBn != yBo) {

      color(BLACK);
      drawExCtr( xBo, yBo, rv0);

      switch (nvt) {
        case  0:	/* nvt++ already in markCrsTri2 */
        case  1: break;
        case  2:	/* rubber line from vertices 0 to 1 */
          BLINE(triB[0],xBo,yBo); color(GREEN); BLINE(triB[0],xBn,yBn);
          break;

#if 00
        case  3:        	/* triangle: rubber from vert 1 to 2 & 2 to 0 */
          poly2i(3,triB);	/* always erase overdraw: normal drawn tri remains */
          triB[2][0] = xBn, triB[2][1] = yBn;
          color(GREEN);
          poly2i(3,triB);
          break;
#endif

        default:  		/* make new triangle by replacing third vertex */
          poly2i(3,triB);
          if (yBn*cosB10 - xBn*sinB10 < licB10)  { wrongSide = 1; curson(); }
          else  {                                  wrongSide = 0; cursoff();
            color(YELLOW); triB[2][0] = xBn, triB[2][1] = yBn; poly2i(3,triB);
          }
          break;
      }
      color(GREEN); drawExCtr( xBn, yBn, rv0);
      xBo = xBn,  yBo = yBn;
    }
  }
  if (yBo != yBn)  /* i.e.mouse didn't move */
    goto RETURN;

  switch(nvt) {
    case  0:  break;	/* nothing's marked yet. */
    case  1:  /* erase old triangle vertex markers & draw new 1 */
      triB[0][0] = xB0 = xBn, triB[0][1] = yB0 = yBn;
      color(GREEN);	circi( xBn, yBn, rv0);
      break;
    case  2:  			/* just add second vertex */
      triB[1][0] = xB1 = xBn, triB[1][1] = yB1 = yBn;
      color(YELLOW);	circi( xBn, yBn, rv1);
      break;

    default:  				/* add third vertex and draw the triangle */
#ifdef ZZZORK
      color(BLACK);	poly2i(3,triB);
#endif
      j = ntr - 1;
      if (nvt > 2) {
        if (wrongSide) {
          fprintf(stderr,"<#>");
          triB[2][0] = xBo = xB2 = xBA;
          triB[2][1] = yBo = yB2 = yBA;
          color(YELLOW);  poly2i(3,triB);
          goto RETURN; 
        }
        color(BLACK);
        circi(xB0, yB0, rv0);
        circi(xB1, yB1, rv1);
        circi(xB2, yB2, rv2);
      } 
      mB->sema = nvt;
      xB2 = xBn, yB2 = yBn;
      triB[0][0] = xB0;  mB->triB[j][0][0] = xB0*rX + xoff;
      triB[0][1] = yB0;  mB->triB[j][0][1] = yB0*rY + yoff;
      triB[1][0] = xB1;  mB->triB[j][1][0] = xB1*rX + xoff;
      triB[1][1] = yB1;  mB->triB[j][1][1] = yB1*rY + yoff;
      triB[2][0] = xB2;  mB->triB[j][2][0] = xB2*rX + xoff;
      triB[2][1] = yB2;  mB->triB[j][2][1] = yB2*rY + yoff;

      color(RED);	circi(xBn, yBn, rv2);
      color(GREEN);	circi(xB0, yB0, rv0);
      color(YELLOW);	circi(xB1, yB1, rv1);
#ifdef ZZZORK
      poly2i(3,triB);	/* leave yellow in overdraw */
#endif

      if (Devopts & O_WARN && Options & O_VERB) {
        drawmode(NORMALDRAW);
        cpack(Uclr);	poly2i(3,triB);
        winset(widP);
        /* drawmode(OVERDRAW); color(YELLOW); */
        drawmode(NORMALDRAW);  cpack(HcCLR[(nvt-1) % 6]);
        scalePoly2(spol, mB->triB[j], 3, wP->zX, wP->zY);
        wrd("spol %d:  %8.2f %8.2f  %8.2f %8.2f  %8.2f %8.2f"
             ,ntr,spol[0][0],spol[0][1],spol[2][0],spol[2][1],spol[2][0],spol[2][1]);
        poly2(3,spol);
        drawmode(NORMALDRAW);
        winset(widB);
      }
      break;
  }

RETURN:
  drawmode(NORMALDRAW);
  curson();
  /* return nvt; */
}







#if 	0000 /*_____________________________________________________________*/

static void
drawTanLines(dbl length)
{
flt     xy[2], dx, dy;

  dx = length*cos(theA21);   dy = length*sin(theA21);
  bgnline();
    cpack(FcRED); xy[0] = xA2 + dx;   xy[1] = yA2 + dy;   v2f(xy);
    cpack(FcGRN); xy[0] = xA2 - dx;   xy[1] = yA2 - dy;   v2f(xy); /* inside */
  endline();
  bgnline();
    cpack(FcYEL); xy[0] = xA1 + dx;   xy[1] = yA1 + dy;   v2f(xy); /* inside */
    cpack(FcBLU); xy[0] = xA1 - dx;   xy[1] = yA1 - dy;   v2f(xy);
  endline();
}


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


#if 00
void
zoomEditMenu2 (void *v, int device, int flags)	/* diff. menu for zoomed windows? */
{
pWmj    wA   = (pWmj)v, wB = wA->next, wp;
WmjMnu *mn   = wA->mnu;
pMD     wwD  = (pMD)mn->data;
long 	pval;
#define retitle(S)      sprintf(str,"%s: %s",wA->T,(S)); wintitle(str)

  if (! mn->id) mn->id = defpup("EDIT SUB %t|Start Over|New Chain|Map Chain");

  switch (pval = dopup(mn->id)) {
    case  1: retitle("restart");  qenter(REDRAW,wA->id); qenter(REDRAW,wB->id); break;
    case  2: retitle("new chain");                             break;
    case  3: retitle("mapped");    inMapTri2(wwD,mn->nA,mn->nB,0);    break;
    case -1: /* no selection */                         break;
    default: warn("zoomEditMenu2: Unknown option: pupval:%d",pval);
  }
}
#endif


void
zoomRectTri2 (void *v, int device, int flags)
{
static  WmjAct  markTri, moveTri;
static  WmjMnu  zoomMnu;
pWmj    wA   = (pWmj)v, wB = wA->next, wp;
WmjAct *ap   = wA->act;
pMD     wwD  = (pMD)ap->data;
int     nA   = ap->nA;  pMOB  mA = wwD->mob[nA];
int     nB   = ap->nB;  pMOB  mB = wwD->mob[nB];
flt     zX   = wA->zX, zY = wA->zY;
long    xAb, yAb, xAn, yAn, xAo,yAo, wx,ww,wy,wh;
long    xBb, yBb, xBn, yBn, xBo,yBo;
long	widB = wB->id, widA = winget();

  getorigin(&wx,&wy);       
  getsize(  &ww,&wh); ww -= 1; wh -= 1;

  wrn("zoomRectTri2: nA,nB: %d %d", nA,nB);

  xAn = xBn = xBo = xBb = xAo = xAb = (getvaluator(MOUSEX) - wx);
  yAn = yBn = yBo = yBb = yAo = yAb = (getvaluator(MOUSEY) - wy);

  /* cursoff(); */
  drawmode(OVERDRAW);             /* If no OVERDRAW, use PUPDRAW */
  winset(widB);
  drawmode(OVERDRAW);

  while(getbutton(device)) {
    xAn  = getvaluator(MOUSEX) - wx; if (xAn < 0)  xAn = 0;  else if (xAn > ww)  xAn = ww;
    yAn  = getvaluator(MOUSEY) - wy; if (yAn < 0)  yAn = 0;  else if (yAn > wh)  yAn = wh;
    if (xAn != xAo || yAn != yAo) {

      winset(widA);
      color(BLACK);                   /* black erases */
      rects(xAb,yAb,xAo,yAo);
      color(RED);                     /* set color of new circle */
      rects(xAb,yAb,xAn,yAn);
      xAo = xAn,  yAo = yAn;

      winset(widB);
      xBn = xAn, yBn = yAn;
      color(BLACK);                   /* black erases */
      sboxi(xBb,yBb,xBo,yBo);
      color(RED);                     /* set color of new circle */
      sboxi(xBb,yBb,xBn,yBn);
      xBo = xBn,  yBo = yBn;
    }
  }
  color(BLACK);
  sboxi(xBb,yBb,xBo,yBo);	/* still in viewport B */
  drawmode(NORMALDRAW);
  winset(widA);
  color(BLACK);
  sboxi(xAb,yAb,xAo,yAo);
  drawmode(NORMALDRAW);

#define MIN_BOX_LEN  20

  if (xAb > xAn)  xAn = xAb, xAb = xAo;
  if (yAb > yAn)  yAn = yAb, yAb = yAo;
  xAo = xAn - xAb;
  yAo = yAn - yAb;
  xAo = MIN(xAo,yAo);
  if (xAo < MIN_BOX_LEN) {
    wrn("zoomRectTri2: box size: %d < MIN: %d", xAo, MIN_BOX_LEN);
    return;
  }

  winset(widB);
  cpack(FcGRN);
  sboxi(xBb,yBb,xBn,yBn);

  winset(widA);
  cpack(FcGRN);
  sboxi(xAb,yAb,xAn,yAn);

  zoomMnu.func   = &winEditMenu2;
  zoomMnu.data   =  wwD;			/* ap->data; */
  zoomMnu.nA     =  nA;
  zoomMnu.nB     =  nB;

  markTri        = *ap;
  markTri.func   = &markSub2;
  wp = showSub(wA,  (int)(xAb/zX),(int)(yAb/zY), (int)(xAn/zX), (int)(yAn/zY),"subA",mA->nzX,mA->nzY,5);
  wp->in         =  nA;
  wp->act        = &markTri;
  wp->mnu        = &zoomMnu;
  wp->prev       =  wA;

  moveTri        = *ap;
  moveTri.func   = &moveSub2;
  wp->next = showSub(wA->next,xAb/zX,yAb/zY,xAn/zX,yAn/zY,"subB",mB->nzX,mB->nzY,7);
  wp->next->in   =  nB;
  wp->next->act  = &moveTri;
  wp->next->mnu  = &zoomMnu;
  wp->next->next =  wp;
  wp->next->prev =  wB;

  mB->nvrt = 0;		/* start new mesh/chain */

/*
  wrn("zoomRectTri2 addrs: wA:%lu  wp:%lu  wp->prev:%lu  wp->next:%lu"
	,(unl)wA,wp,wp->prev,wp->next);
  wrn("zoomRectTri2 winid: wA:%d   wp:%d   wp->prev:%d   wp->next:%d"
	,wA->id,wp->id,wp->prev->id,wp->next->id);
*/

  winset(widA);
  /* curson(); */

return;
}


