/*****************************************************************************
 modFwdTri.c    S.R.Lines    93.8.17-9.21

 OBSOLETE: SEE triModFn2.c

 BIG CAVEAT:  Basically assumes the list of trapezoid is pre-ordered by 
 increasing y-locations.
 LETTER CODE: I=Initial, F=Final, M=Middle(interpolated), E=Extrapolated
 L=Left(side 1-3), R=Right(side 2-4), T=Top(side 1-2), B=Bottom(side 2-4)
 Capital X,Y refer to directed distances (vectors);
 lower case x,y refer to position (NYI).  Both are signed quantities.
 *****************************************************************************/
#undef		NDEBUG
#include        <stddef.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <assert.h>
#include        <math.h>
#include        <un/unMath.h>
#include        <un/unBug.h>
#include        <un/unTypes.h>
#include        <un/unBug.h>
/***
***/
#include        <m/rowCopy.h>
#include        <m/mj.h>
#include        "blerp.h"
/*#include        "rowModf.h"*/

#define DANGLE(dang23,x1,y1,x2,y2,x3,y3)        \
      ss = atan2f(y2 - y1, x2 - x1), \
      tt = atan2f(y3 - y1, x3 - x1), dang23 = tt - ss

#define TANGLE(a23,x1,y1,x2,y2,x3,y3) ((DANGLE(a23,x1,y1,x2,y2,x3,y3))>0.0F)


void modFwdTri( ppf moX, ppf moY, ppf baX, ppf baY, ppf scA, ppf scB
              , int dX, int dY, unt dW, unt dH
              , int sX, int sY, unt sW, unt sH
              , flt triP[][12], unt numTri, dbl trs, unt flags)
{
regflt	ss,tt;
int 	q, vord;
pfl 	tp;
flt 	tang;
flt 	Ax1,Ax2,Ax3,Ay1,Ay2,Ay3;
flt 	Bx1,Bx2,Bx3,By1,By2,By3;
flt 	A[6], F[6];
  
  wrn("MMMMMMMMMMMMMM This is modFwdTri MMMMMMMMMMMMMM ");
  rowCopyFF(moX,baX,sX,sY,sW,sH,sX,sY);
  rowCopyFF(moY,baY,sX,sY,sW,sH,sX,sY);

  for (q = 0; q <= numTri; q++) {
    tp   = triP[q];
    

	/** @@ KLUDGE TIL FINISH REORIENTING Y-AXIS */

    Ax1 = tp[ 0];  Ay1 = tp[ 1];
    Ax2 = tp[ 2];  Ay2 = tp[ 3];
    Ax3 = tp[ 4];  Ay3 = tp[ 5];

    Bx1 = tp[ 6];  By1 = tp[ 7];
    Bx2 = tp[ 8];  By2 = tp[ 9];
    Bx3 = tp[10];  By3 = tp[11];

    /**wrn("%3d BEGIN: A: %4.f,%4.f    %4.f,%4.f    %4.f,%4.f   B: %4.f,%4.f    %4.f,%4.f    %4.f,%4.f"
    ,q, Ax1,Ay1, Ax2,Ay2, Ax3,Ay3, Bx1,By1, Bx2,By2, Bx3,By3);**/

#if 1
    if      (Ay1 <= Ay2) {
      if    (Ay1 <= Ay3) { /* y1 <= y2 AND y1 <= y3 -- the optimal case. */
        if  (TANGLE(tang,Ax1,Ay1,Ax2,Ay2,Ax3,Ay3))  vord = 123;  else vord = 132;
      }
      else  {		  /* y3 < y1 <= y2 */
        if  (TANGLE(tang,Ax3,Ay3,Ax1,Ay1,Ax2,Ay2))  vord = 312;  else vord = 321;
      }
    }
    else if (Ay2 <= Ay3) {/* y2 < y1 && y2 <= y3 */
      if    (TANGLE(tang,Ax2,Ay2,Ax3,Ay3,Ax1,Ay1))  vord = 231;  else vord = 213;
    }
    else    {			/* y3 < y2 < y1 */
      if    (TANGLE(tang,Ax3,Ay3,Ax1,Ay1,Ax2,Ay2))  vord = 312;  else vord = 321;
    }

#else

    if (Ay1 > Ay2) {/* y2 < y1 */
      if (Ay2 > Ay3) {/* y3 < y2 < y1 */
        if (TANGLE(tang,Ax3,Ay3,Ax1,Ay1,Ax2,Ay2))  vord = 312;  else vord = 321;
      }
      else {				/* y2 <= y3 and y2 < y1 */
        if (TANGLE(tang,Ax2,Ay2,Ax3,Ay3,Ax1,Ay1))  vord = 231;  else vord = 213;
      }
    }
    else if (Ay1 > Ay3) {/* y3 < y1 <= y2 */
        if (TANGLE(tang,Ax3,Ay3,Ax1,Ay1,Ax2,Ay2))  vord = 312;  else vord = 321;
    }
    else {/* y1 <= y2 <= y3 */
        if (TANGLE(tang,Ax1,Ay1,Ax2,Ay2,Ax3,Ay3)) vord = 123;  else vord = 132;
    }
#endif

    switch (vord) {
      case 123:
        A[0]=Ax1,     A[1]=Ay1,     A[2]=Ax2,     A[3]=Ay2,     A[4]=Ax3,     A[5]=Ay3;
        F[0]=Ax1-Bx1, F[1]=Ay1-By1, F[2]=Ax2-Bx2, F[3]=Ay2-By2, F[4]=Ax3-Bx3, F[5]=Ay3-By3;
        break;

      case 132:
        A[0]=Ax1,     A[1]=Ay1,     A[2]=Ax3,     A[3]=Ay3,     A[4]=Ax2,     A[5]=Ay2;
        F[0]=Ax1-Bx1, F[1]=Ay1-By1, F[2]=Ax3-Bx3, F[3]=Ay3-By3, F[4]=Ax2-Bx2, F[5]=Ay2-By2;
        break;

      case 213:
        A[0]=Ax2,     A[1]=Ay2,     A[2]=Ax1,     A[3]=Ay1,     A[4]=Ax3,     A[5]=Ay3;
        F[0]=Ax2-Bx2, F[1]=Ay2-By2, F[2]=Ax1-Bx1, F[3]=Ay1-By1, F[4]=Ax3-Bx3, F[5]=Ay3-By3;
        break;

      case 231:
        A[0]=Ax2,     A[1]=Ay2,     A[2]=Ax3,     A[3]=Ay3,     A[4]=Ax1,     A[5]=Ay1;
        F[0]=Ax2-Bx2, F[1]=Ay2-By2, F[2]=Ax3-Bx3, F[3]=Ay3-By3, F[4]=Ax1-Bx1, F[5]=Ay1-By1;
        break;

      case 312:
        A[0]=Ax3,     A[1]=Ay3,     A[2]=Ax1,     A[3]=Ay1,     A[4]=Ax2,     A[5]=Ay2;
        F[0]=Ax3-Bx3, F[1]=Ay3-By3, F[2]=Ax1-Bx1, F[3]=Ay1-By1, F[4]=Ax2-Bx2, F[5]=Ay2-By2;
        break;

      case 321:
        A[0]=Ax3,     A[1]=Ay3,     A[2]=Ax2,     A[3]=Ay2,     A[4]=Ax1,     A[5]=Ay1;
        F[0]=Ax3-Bx3, F[1]=Ay3-By3, F[2]=Ax2-Bx2, F[3]=Ay2-By2, F[4]=Ax1-Bx1, F[5]=Ay1-By1;
        break;

      default: wrn("default in switch -- what the hell is vord = %d?", vord);
    }
    /**
    if (vord != 123)
      wrn("%3d %d  Swap: %4.f,%4.f    %4.f,%4.f    %4.f,%4.f -> %4.f,%4.f    %4.f,%4.f    %4.f,%4.f"
      ,q,vord, Ax1,Ay1, Ax2,Ay2, Ax3,Ay3,  A[0],A[1],A[2],A[3],A[4],A[5]);
    **/

    if (A[1] > A[3] || A[1] > A[5])
      warn("YOU REALLY SCREWED UP!  q=%d, vord=%d, but y123: %5.1f  %5.1f  %5.1f"
          ,q,vord,A[1],A[3],A[5]);

	/*** TRIANGLE ORDER: default is <1,2,3> *******************/

    interpTriBil(moX,moY,A,F);			     /* Correct moX,moY as best we can... */

    A[0] *=   3.0F, A[2] *=   3.0F, A[4] *=   3.0F;  /* Now screw around... */
    F[0]  =  255.0, F[2]  =  127.5, F[4]  =    0.0;
    F[1]  =    0.0, F[3]  =  127.5, F[5]  =  255.0;
    interpTriBil(scA,scB,A,F);
  }
}


