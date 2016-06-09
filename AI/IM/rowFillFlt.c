/*****************************************************************************
rowFillFlt.c    S.R.Lines (spraxlo@ai.mit.edu)    93.7.24-9.8
*****************************************************************************/
#include <un/unTypes.h>
#undef	UNBUG
#define	UNBUG 0
#include <un/unBug.h>
#include <un/unMacro.h>
#include "rowFill.h"

#define	SQRT2	(1.414213562373095049F)
#define IR2 	(0.707106781186547524F)

#define FILL_CENT	1
#define FILL_RECT	2
#define FILL_LAST	4
#define FILL_BORD	8

#define FILL_BLOT	0
#define FILL_FILL	( FILL_CENT | FILL_RECT | FILL_LAST )
#define FILL_SOAK	( FILL_CENT | FILL_RECT | FILL_BORD )

#define FNEW	7.50F
#define FINS	1.33F
#define FINC	1.33F*IR2
#define FNXT	1.10F
#define FOUT	1.00F
#define FOUC	1.00F*IR2
#define FOLD	1.00F
#define FSUB	1.50F	/* wt < infimum */
#define FKPT	2.00F	/* KEPT val should be from .5 to 1.5 OLD val (?) */


/*___________________________________________________________________________*|
  Goes in clockwise square spiral from initial point xS,yS until distance
  from xS,yS == radius or hitting the closest boundary.
  The routines here use rows as rows, ie, as ptrs to ptrs, and don't try to
  get around the double array reference (potential double fetch) for every
  pxl.  If your rows really are in one contiguous (but not necessarily packed)
  block, then use memFill.c functions instead (with the parameter scan =
  row[1] - row[0).

NB: where srcU == 0, srcX/Y must also == 0; this is assumed
true before the function starts, but could be enforced within.
Right now this function is correct only for infimum == 0

	THIS FUNCTION IS GOOD ONLY FOR PREVIOUSLY NORMED DATA,
	THAT IS, VALUES MUST ALREADY HAVE BEEN DIVIDED BY ACCUMULATED WEIGHTS.
	
	THE MAJOR ASSUMPTION IS THAT IF (wts[j][k] < T), THEN (img[j][k] == 0)
|*___________________________________________________________________________*/

static void
rowFill_( ppf dstQ, ppf accW
        , int wide, int tall, int radius, int xS, int yS, int flags
        , double infimum, double dmean)
{
  int	z, d, dL,dR,dT,dB;
  int   wideM = wide-1, tallM = tall-1;
  register ppf q = dstQ, w = accW;
  register int j,k;
  flt Lw, Lq = (flt)dmean, T = (flt)infimum;
  
  if (radius > 0) {
    radius++;
    dL = xS - radius;   if (dL < 0)      dL = 0;
    dR = xS + radius;   if (dR > wideM)  dR = wideM;
    dT = yS - radius;   if (dT < 0)      dT = 0;
    dB = yS + radius;   if (dB > tallM)  dB = tallM;
  } else {
    dL = dT = 0;
    dR = wideM;
    dB = tallM; 
  }
  
#define tryRIGHT \
    {\
      /* WQQ3("RIGHT:	(% 5d,% 5d)\n",j,k); */ \
      if (w[j][k] > T) {\
        Lw = FOLD    + FNEW*w[j][k]         + FOUT*(w[j-1][k]>T) + FINS*(w[j+1][k]>T);\
        Lq = FOLD*Lq + FNEW*w[j][k]*q[j][k] + FOUT* q[j-1][k]    + FINS* q[j+1][k];\
        q[j][k] = (flt)(Lq /= Lw);\
      }\
      else {\
        Lw = FKPT    + FSUB*w[j][k]         + FNXT*(w[j][k+1]>T) + FOUT*(w[j-1][k]>T) + FINS*(w[j+1][k]>0) + FOUC*(w[j-1][k+1]>T) + FINC*(w[j+1][k+1]>T);\
        Lq = FKPT*Lq + FSUB*w[j][k]*q[j][k] + FNXT* q[j][k+1]    + FOUT* q[j-1][k]    + FINS* q[j+1][k]    + FOUC* q[j-1][k+1]    + FINC* q[j+1][k+1];\
        q[j][k] = (flt)(Lq /= Lw);\
      }\
      /**** w[j][k] = 1.0F; ****/\
    }
#define tryLEFT \
    {\
      /* WQQ3("LEFT:	(% 5d,% 5d)\n",j,k); */ \
      if (w[j][k] > T) {\
        Lw = FOLD    + FNEW*w[j][k]         + FINS*(w[j-1][k]>T) + FOUT*(w[j+1][k]>T);\
        Lq = FOLD*Lq + FNEW*w[j][k]*q[j][k] + FINS* q[j-1][k]    + FOUT* q[j+1][k];\
        q[j][k] = (flt)(Lq /= Lw);\
      }\
      else {\
        Lw = FKPT    + FSUB*w[j][k]         + FNXT*(w[j][k-1]>T) + FINS*(w[j-1][k]>T) + FOUT*(w[j+1][k]>0) + FINC*(w[j-1][k-1]>T) + FOUC*(w[j+1][k-1]>T);\
        Lq = FKPT*Lq + FSUB*w[j][k]*q[j][k] + FNXT* q[j][k-1]    + FINS* q[j-1][k]    + FOUT* q[j+1][k]    + FINC* q[j-1][k-1]    + FOUC* q[j+1][k-1];\
        q[j][k] = (flt)(Lq /= Lw);\
      }\
      /**** w[j][k] = 1.0F; ****/\
    }
#define tryDOWN \
    {\
      /* WQQ3("DOWN:	(% 5d,% 5d)\n",j,k); */\
      if (w[j][k] > T) {\
        Lw = FOLD    + w[j][k-1]           + FNEW*w[j][k]         + w[j][k+1];\
        Lq = FOLD*Lq + w[j][k-1]*q[j][k-1] + FNEW*w[j][k]*q[j][k] + w[j][k+1]*q[j][k+1];\
        q[j][k] = (flt)(Lq /= Lw);\
      }\
      else {\
        Lw = FKPT    + FSUB*w[j][k]         + w[j][k-1]           + w[j][k+1]           + IR2*w[j+1][k-1]             + w[j+1][k]           + IR2*w[j+1][k+1];\
        Lq = FKPT*Lq + FSUB*w[j][k]*q[j][k] + w[j][k-1]*q[j][k-1] + w[j][k+1]*q[j][k+1] + IR2*w[j+1][k-1]*q[j+1][k-1] + w[j+1][k]*q[j+1][k] + IR2*w[j+1][k+1]*q[j+1][k+1];\
        q[j][k] = (flt)(Lq /= Lw);\
      }\
      /**** w[j][k] = 1.0F; ****/\
    }
#define tryUPTOP \
    {\
      /* WQQ3("UP: 	(% 5d,% 5d)\n",j,k); */\
      if (w[j][k] > T) {\
        Lw = FOLD    + w[j][k-1]           + FNEW*w[j][k]         + w[j][k+1];\
        Lq = FOLD*Lq + w[j][k-1]*q[j][k-1] + FNEW*w[j][k]*q[j][k] + w[j][k+1]*q[j][k+1];\
        q[j][k] = (flt)(Lq /= Lw);\
      }\
      else {\
        Lw = FKPT    + FSUB*w[j][k]         + w[j][k-1]           + w[j][k+1]           + IR2*w[j-1][k+1]             + w[j-1][k]           + IR2*w[j-1][k-1];\
        Lq = FKPT*Lq + FSUB*w[j][k]*q[j][k] + w[j][k-1]*q[j][k-1] + w[j][k+1]*q[j][k+1] + IR2*w[j-1][k+1]*q[j-1][k+1] + w[j-1][k]*q[j-1][k] + IR2*w[j-1][k-1]*q[j-1][k-1];\
        q[j][k] = (flt)(Lq /= Lw);\
      }\
      /**** w[j][k] = 1.0F; ****/\
    }
#define brkRIGHT	{ WQQ3("break RIGHT:	(% 5d,% 5d)\n",j,k); break;}
#define brkLEFT 	{ WQQ3("break LEFT: 	(% 5d,% 5d)\n",j,k); break;}
#define brkDOWN 	{ WQQ3("break DOWN: 	(% 5d,% 5d)\n",j,k); break;}
#define brkUPTOP	{ WQQ3("break UPTOP:	(% 5d,% 5d)\n",j,k); break;}

  /*_______________________________________________________________________*/
  for (j = yS, k = xS, d = 1;  ; d++)  {
    WQQ3("SpiralTopLeft:(% 5d,% 5d)\n",j,k);
    for (z = xS+d; k < z; k++)
      tryRIGHT
    if (k >= dR)
      brkRIGHT
    for (z = yS+d; j < z; j++)
      tryDOWN
    if (j >= dB)
      brkDOWN
    for (z = xS-d; k > z; k--)
      tryLEFT      
    if (k <= dL)
      brkLEFT
    for (z = yS-d; j > z; j--)
      tryUPTOP
    if (j <= dT)
      brkUPTOP
  }


#undef	tryUPTOP	/* @@ delete */
#define	tryUPTOP	q[j][k]=254.0;


  if (flags & FILL_RECT) {
    static int tyco = 0;
    if (++tyco == 1)
      warn("\n\n\tYES, IT'S TRUE: (flags & FILL_RECT) !!!!!!!!!!!!!!!!!!!!\n\n");

    WQQ3("Fill RIGHT RECT from (% 3d, %3d)\n",yS-d+1,xS+d);
    Lq = q[yS-d+1][xS+d-1];
    for (k =   xS+d; k < wideM; k++) {
      for (z = yS+d, j = yS-d; ++j < z ; )
        tryDOWN
      if (++k >= wideM) break;
      for (z = yS-d          ; --j > z ; )
        tryUPTOP
    }
    /* q[yS-d+1][xS+d] = 255.0; */


    WQQ3("Fill  LEFT RECT from (% 3d, %3d)\n",yS-d+1,xS-d);

q[yS-d+1][xS-d  ] = 255.0;
w[yS-d+1][xS-d  ] =   5.0;
q[yS-d+1][xS-d+1] = 255.0;
w[yS-d+1][xS-d+1] =   5.0;

    Lq = q[yS-d+1][xS-d+1];
    for (k =   xS-d; k > 0; k--) {
      for (z = yS+d, j = yS-d; ++j < z ; )
        tryDOWN
      if (--k <= 0)     break;
      for (z = yS-d          ; --j > z ; )
        tryUPTOP
    }
    /* q[yS-d+1][xS-d] = 255.0; */

q[yS-d+1][xS-d  ] = 255.0;
q[yS-d+2][xS-d  ] = 255.0;
q[yS-d+1][xS-d+1] = 255.0;
q[yS-d+2][xS-d+1] = 255.0;
  
    WQQ3("Fill   TOP RECT from (% 3d, %3d)\n",yS-d,1);
    Lq = q[yS-d+1][xS-d+1];
    for (  j = yS-d ;   j > 0     ; j--) {
      for (k = 0    ; ++k < wideM ; )
        tryRIGHT
      if (--j <= 0) break;
      for (; --k > 0;)
        tryLEFT
    }
    /* q[yS-d][1] = 255.0; */
  
    WQQ3("Fill   BOT RECT from (% 3d, %3d)\n",yS+d,1);
    Lq = q[yS+d-1][xS-d+1];
    for (  j = yS+d ;   j < tallM ; j++) {
      for (k = 0    ; ++k < wideM ; )
        tryRIGHT
      if (++j >= tallM)  break;
      for (; --k > 0;)
        tryLEFT
    }
    /* q[yS+d][1] = 255.0; */
  }
  
  


  /** THE FOLLOWING SHOULD BE SEPARATE FUNCTION **/
  
  if (flags & FILL_LAST) {
#if 0
    k = j = 0;
    do {  w[j][++k] = Lq; } while (k < wideM);
    do {  w[++j][k] = Lq; } while (j < tallM);
    do {  w[j][--k] = Lq; } while (k > 0);
    do {  w[--j][k] = Lq; } while (j > 0);
#else 
    Lw = w[1][1];
    Lq = q[1][1];	/* initialize kept values */
    for (j = 0, k = 1; k < wideM; k++)
    {					/* _______RIGHTWARD_______ */
      if (w[j][k] > T) {
        Lw = FOLD    + FNEW*w[j][k]         + IR2*w[j+1][k-1]             + w[j+1][k];
        Lq = FOLD*Lq + FNEW*w[j][k]*q[j][k] + IR2*w[j+1][k-1]*q[j+1][k-1] + w[j+1][k]*q[j+1][k];
        q[j][k] = (flt)(Lq /= Lw);
      }
      else {
        Lw = FKPT    + FSUB*w[j][k]         + w[j][k+1]           + IR2*w[j+1][k-1]             + w[j+1][k]           + IR2*w[j+1][k+1];
        Lq = FKPT*Lq + FSUB*w[j][k]*q[j][k] + w[j][k+1]*q[j][k+1] + IR2*w[j+1][k-1]*q[j+1][k-1] + w[j+1][k]*q[j+1][k] + IR2*w[j+1][k+1]*q[j+1][k+1];
        q[j][k] = (flt)(Lq /= Lw);
      }
      /**** w[j][k] = 1.0F; ****/
    }
    /**** w[j][k] = 1.0F; ****/
    q[j][k] = (flt)Lq;	/* fill corner */
    for (; ++j < tallM;)
    {					/* _______DOWNWARD_______ */
      if (w[j][k] > T) {
        Lw = FOLD    + FNEW*w[j][k]         + w[j][k-1]           + IR2*w[j-1][k-1];
        Lq = FOLD*Lq + FNEW*w[j][k]*q[j][k] + w[j][k-1]*q[j][k-1] + IR2*w[j-1][k-1]*q[j-1][k-1];
        q[j][k] = (flt)(Lq /= Lw);
      }
      else {
        Lw = FKPT    + FSUB*w[j][k]         + w[j][k-1]           + IR2*w[j-1][k-1]             + IR2*w[j+1][k-1]             + w[j+1][k];
        Lq = FKPT*Lq + FSUB*w[j][k]*q[j][k] + w[j][k-1]*q[j][k-1] + IR2*w[j-1][k-1]*q[j-1][k-1] + IR2*w[j+1][k-1]*q[j+1][k-1] + w[j+1][k]*q[j+1][k];
        q[j][k] = (flt)(Lq /= Lw);
      }
      /**** w[j][k] = 1.0F; ****/
    }
    /**** w[j][k] = 1.0F; ****/
    q[j][k] = Lq;	/* fill corner */
    for (; --k > 0;)
    {					/* _______LEFTWARD_______ */
      if (w[j][k] > T) {
        Lw = FOLD    + FNEW*w[j][k]         + w[j-1][k]           + IR2*w[j-1][k+1];
        Lq = FOLD*Lq + FNEW*w[j][k]*q[j][k] + w[j-1][k]*q[j-1][k] + IR2*w[j-1][k+1]*q[j-1][k+1];
        q[j][k] = (flt)(Lq /= Lw);
      }
      else {
        Lw = FKPT    + FSUB*w[j][k]         + w[j][k-1]           + IR2*w[j-1][k-1]             + w[j-1][k]           + IR2*w[j-1][k+1];
        Lq = FKPT*Lq + FSUB*w[j][k]*q[j][k] + w[j][k-1]*q[j][k-1] + IR2*w[j-1][k-1]*q[j-1][k-1] + w[j-1][k]*q[j-1][k] + IR2*w[j-1][k+1]*q[j-1][k+1];
        q[j][k] = (flt)(Lq /= Lw);
      }
      /**** w[j][k] = 1.0F; ****/
    }
    /**** w[j][k] = 1.0F; ****/
    q[j][k] = Lq;	/* fill corner */    
    for (; --j > 0;)
    {					/* _______UPWARD_______ */
      if (w[j][k] > T) {
        Lw = FOLD    + FNEW*w[j][k]         + w[j][k+1]           + IR2*w[j+1][k+1];
        Lq = FOLD*Lq + FNEW*w[j][k]*q[j][k] + w[j][k+1]*q[j][k+1] + IR2*w[j+1][k+1]*q[j+1][k+1];
        q[j][k] = (flt)(Lq /= Lw);
      }
      else {
        Lw = FKPT    + FSUB*w[j][k]         + w[j][k+1]           + w[j-1][k]           + IR2*w[j-1][k+1]             + IR2*w[j+1][k+1];
        Lq = FKPT*Lq + FSUB*w[j][k]*q[j][k] + w[j][k+1]*q[j][k+1] + w[j-1][k]*q[j-1][k] + IR2*w[j-1][k+1]*q[j-1][k+1] + IR2*w[j+1][k+1]*q[j+1][k+1];
        q[j][k] = (flt)(Lq /= Lw);
      }
      /**** w[j][k] = 1.0F; ****/
    }
    /**** w[j][k] = 1.0F; ****/
    q[j][k] = Lq;	/* fill corner */
#endif
  }
}


void
rowFillFlt ( ppf dstQ, ppf accW, int xi, int yi, unt wide, unt tall
           , double infimum, double dmean)
{
  int j = yi+tall;
  if (xi != 0 || yi != 0)
    while (--j >= yi)  dstQ[j] += xi;
  rowFill_( dstQ+yi, accW, wide, tall, 0 /*radius*/, (wide-1)/2, (tall-1)/2
          , FILL_FILL, infimum, dmean);
}
