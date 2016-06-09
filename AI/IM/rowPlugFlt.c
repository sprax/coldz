/*****************************************************************************
rowPlugFlt.c    S.R.Lines (spraxlo@ai.mit.edu)    93.7.24-9.14

  NB: The threshold for holes, infill, should be no greater than 0.5 after
  a bilinear forward smattering (rowSmatBil*); 0.33 seems good.
  After a biquadratic smattering, something like 0.2 is probably good.

Optimizations & Tunings for next version:
  ptrs seem faster...
  Different weight factors FINS & FOUT for neighbors in & out of spiral.
  Use separate variables for j-1,j, and j+1, etc. -OR-
  Replace all double indexing of row pointers by singly indexed pxl pointers,
    even using three pointers in parallel instead of jumping rows. 
  Separate function for single-pixel width border (last step).
  For higher quality, combine the results of clockwise & ccw spirals.
  Split thresholding into two levels, bounded by infimum & medium, with
    separate neighbor-weighting schemes.  The [inf < val < med]-case needs
    fewer neighbors.)
  Make a faster version which assumes all acc. weights are previously
    normalized to 0.0 or 1.0 -- they can even be stored as int (uchar probably
    not any faster, just smaller).  (As of 93.9.12, the difference in results
    between prenormed vals-n-wts and only prenormed values is imperceptible.)
*****************************************************************************/
#undef	UNBUG
#define	UNBUG 0
#include	<un/unBug.h>
#include	<un/unMacro.h>
#include	<un/unTypes.h>
#include	"rowPlug.h"

#define PLUG_CENT	1
#define PLUG_RECT	2
#define PLUG_LAST	4
#define PLUG_BORD	8

#define PLUG_BLOT	0
#define PLUG_PLUG	( PLUG_CENT | PLUG_RECT | PLUG_LAST )
#define PLUG_SOAK	( PLUG_CENT | PLUG_RECT | PLUG_BORD )

#define FNEW	23.0F
#define FOLD	 1.0F
#define FSUB	 1.5F	/* wt < infimum */
#define FKPT	 1.0F	/* KEPT val should be from .5 to 1.5 OLD val (?) */

#define	SQRT2	(1.414213562373095049F)
#define IR2 	(0.707106781186547524F)

/*___________________________________________________________________________*|
  Goes in clockwise square spiral from initial point xS,yS until distance
  from xS,yS == radius or hitting the closest boundary.
  The routines here use rows as rows, ie, as ptrs to ptrs, and don't try to
  get around the double array reference (potential double fetch) for every
  pxl.  If your rows really are in one contiguous (but not necessarily packed)
  block, then use memPlug.c functions instead (with the parameter scan =
  row[1] - row[0).

NB: where srcU == 0, srcX/Y must also == 0; this is assumed
true before the function starts, but could be enforced within.
Right now this function is correct only for infimum == 0

	WARNING: THIS FUNCTION IS A KLUDGE.
	THIS FUNCTION IS GOOD ONLY FOR PREVIOUSLY NORMED DATA,

	THAT IS, VALUES MUST ALREADY HAVE BEEN DIVIDED BY ACCUMULATED WEIGHTS.
|*___________________________________________________________________________*/

#define tryRIGHT \
{\
  /* WQQ3("RIGHT:	(% 5d,% 5d)\n",j,k); */ \
  /* assert(w[j][k] <= 1.001F); */ \
  wp = &w[j][k];\
  qp = &q[j][k];\
  if (*wp > T) {\
    Lw       =  FOLD    + FNEW**wp     + wp[wN]        + wp[W];\
    *qp = Lq = (FOLD*Lq + FNEW**wp**qp + wp[wN]*qp[wN] + wp[W]*qp[W])/Lw;\
  }\
  else {\
    Lw       =  FKPT    + FSUB**wp     + wp[1]       + wp[wN]        + wp[W]       + IR2*wp[wQ]         + IR2*wp[wP];\
    *qp = Lq = (FKPT*Lq + FSUB**wp**qp + wp[1]*qp[1] + wp[wN]*qp[wN] + wp[W]*qp[W] + IR2*wp[wQ]*qp[wQ] + IR2*wp[wP]*qp[wP])/Lw;\
    *wp = infill;\
  }\
}
#define tryLEFT \
{\
  /* WQQ3("LEFT:	(% 5d,% 5d)\n",j,k); */ \
  wp = &w[j][k];\
  qp = &q[j][k];\
  if (*wp > T) {\
    Lw       =  FOLD    + FNEW**wp     + wp[wN]        + wp[W];\
    *qp = Lq = (FOLD*Lq + FNEW**wp**qp + wp[wN]*qp[wN] + wp[W]*qp[W])/Lw;\
  }\
  else {\
    Lw       =  FKPT    + FSUB**wp     + wp[-1]        + wp[wN]        + wp[W]       + IR2*wp[wM]        + IR2*wp[wO];\
    *qp = Lq = (FKPT*Lq + FSUB**wp**qp + wp[-1]*qp[-1] + wp[wN]*qp[wN] + wp[W]*qp[W] + IR2*wp[wM]*qp[wM] + IR2*wp[wO]*qp[wO])/Lw;\
    *wp = infill;\
  }\
}
#define tryDOWN \
{\
  /* WQQ3("DOWN:	(% 5d,% 5d)\n",j,k); */\
  wp = &w[j][k];\
  qp = &q[j][k];\
  if (*wp > T) {\
    Lw       =  FOLD    + wp[-1]        + FNEW**wp     + wp[1];\
    *qp = Lq = (FOLD*Lq + wp[-1]*qp[-1] + FNEW**wp**qp + wp[1]*qp[1])/Lw;\
  }\
  else {\
    Lw       =  FKPT    + FSUB**wp     + wp[-1]        + wp[1]       + IR2*wp[wM]        + wp[W]       + IR2*wp[wP];\
    *qp = Lq = (FKPT*Lq + FSUB**wp**qp + wp[-1]*qp[-1] + wp[1]*qp[1] + IR2*wp[wM]*qp[wM] + wp[W]*qp[W] + IR2*wp[wP]*qp[wP])/Lw;\
    *wp = infill;\
  }\
}
#define tryUPTOP \
{\
  /* WQQ3("UP: 	(% 5d,% 5d)\n",j,k); */\
  wp = &w[j][k];\
  qp = &q[j][k];\
  if (*wp > T) {\
    Lw       =  FOLD    + wp[-1]        + FNEW**wp     + wp[1];\
    *qp = Lq = (FOLD*Lq + wp[-1]*qp[-1] + FNEW**wp**qp + wp[1]*qp[1])/Lw;\
  }\
  else {\
    Lw       =  FKPT    + FSUB**wp     + wp[-1]        + wp[1]       + IR2*wp[wQ]        + wp[wN]        + IR2*wp[wO];\
    *qp = Lq = (FKPT*Lq + FSUB**wp**qp + wp[-1]*qp[-1] + wp[1]*qp[1] + IR2*wp[wQ]*qp[wQ] + wp[wN]*qp[wN] + IR2*wp[wO]*qp[wO])/Lw;\
    *wp = infill;\
  }\
}
	/* After the spiral, dL,dR,dT,dB will no longer needed for distance control, so reassign */
#define brkRIGHT	{ WQQ3("break RIGHT:	(% 5d,% 5d)\n",j,k); eR--; break;}
#define brkLEFT 	{ WQQ3("break LEFT: 	(% 5d,% 5d)\n",j,k); eL++; break;}
#define brkDOWN 	{ WQQ3("break DOWN: 	(% 5d,% 5d)\n",j,k); eB--; break;}
#define brkUPTOP	{ WQQ3("break UPTOP:	(% 5d,% 5d)\n",j,k); eT++; break;}





static void
rowPlug_( ppf dstQ, ppf accW
        , int wide, int tall, int radius, int xS, int yS, int flags
        , double infimum, double infillwt, double dmean)
{
int 	d, dL,dR,dT,dB, eL,eR,eT,eB, wideM = wide-1, tallM = tall-1;
int 	W = accW[1] - accW[0];	/* NB: Assuming same length/spacing between rows of accW & dstQ */
int	wM = W-1, wP = W+1, wN = -W, wO = -1-W, wQ = 1-W;
rgs ppf	q = dstQ, w = accW;
rgs int	j,k;
flt 	*qp, *wp, Lw, T = (flt)infimum, infill = (flt)infillwt;
flt 	Lq = (flt)dmean;
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
  
  /*_______________________________________THE_SQUARE_SPIRAL______________________*/
  for (j = yS, k = xS, d = 1; 1 ; d++)  {
    /* WQQ3("SpiralTopLeft:(% 5d,% 5d)\n",j,k); */
    for (eR = xS+d; k < eR; k++)
      tryRIGHT
    if (k >= dR)
      brkRIGHT
    for (eB = yS+d; j < eB; j++)
      tryDOWN
    if (j >= dB)
      brkDOWN
    for (eL = xS-d; k > eL; k--)
      tryLEFT      
    if (k <= dL)
      brkLEFT
    for (eT = yS-d; j > eT; j--)
      tryUPTOP
    if (j <= dT)
      brkUPTOP
  }
  /* warn("END OF SPIRAL: eL,eR,eT,eB: %3d %3d %3d %3d",eL,eR,eT,eB); */
/*******************
  q[eT][eL] = 255.0;
  q[eT][eR] = 255.0;
  q[eB][eL] = 255.0;
  q[eB][eR] = 255.0;
  q[j][k]   = 128.0;
********************/

  if (flags & PLUG_RECT) {

    k  = eR;	/* eR is the rightmost column already filled */
    j  = eT;	/* eT is the topmost row already filled */
    Lq = q[j][k];
    WQQ4("Fill RIGHT RECT from (% 3d, %3d), initval % .3f\n",j,k+1,Lq);
    --j;
    while (++k < wideM) {
      while (++j <= eB)
        tryDOWN
      if    (++k >= wideM)  break;
      while (--j >= eT)
        tryUPTOP
    }
    


    k  = eL;	/* eL is the leftmost column already filled */
    j  = eT;	/* eT is the topmost row already filled */
    Lq = q[j][k+1];
    WQQ4("Fill  LEFT RECT from (% 3d, %3d), initval % .3f\n",j,k-1,Lq);
    --j;
    while (--k > 0) {
      while (++j <= eB)
        tryDOWN
      if    (--k <= 0)  break;
      while (--j >= eT)
        tryUPTOP
    }
    /*******************
    q[eT  ][eL] =   0.0;
    q[eT+1][eL] = 255.0;
    q[eT+2][eL] =   0.0;
    *******************/
    
    k  = 0;
    j  = eT;	/* eT is the topmost row already filled */
    Lq = q[j][k+1];
    WQQ4("Fill   TOP RECT from (% 3d, %3d), initval % .3f\n",j-1,k,Lq);
    while (--j > 0) {
      while (++k < wideM)
        tryRIGHT
      if    (--j <= 0)  break;
      while (--k >  0)
        tryLEFT
    }
    /*******************
    q[eT][eL  ] =   0.0;
    q[eT][eL+1] = 255.0;
    q[eT][eL+2] =   0.0;
    *******************/

    k  = 0;
    j  = eB;	/* eB is the bottom-most row already filled */
    Lq = q[j][k+1];
    WQQ4("Fill   BOT RECT from (% 3d, %3d), initval % .3f\n",j+1,k,Lq);
    while (++j < tallM) {
      while (++k < wideM)
        tryRIGHT
      if (++j >= tallM)  break;
      while (--k >  0)
        tryLEFT
    }
    /*******************
    q[eB][eL  ] =   0.0;
    q[eB][eL+1] = 255.0;
    q[eB][eL+2] =   0.0;
    *******************/
  }
  
  /****
  WQQ("rowPlug: Returning early, after ALL FOUR RECT FILLS");
  return;
  ****/

  /** THE FOLLOWING SHOULD BE A SEPARATE FUNCTION **/
  
  if (flags & PLUG_LAST) {
#if 0
    k = j = 0;
    do { w[j][++k] = Lq; } while (k < wideM);
    do { w[++j][k] = Lq; } while (j < tallM);
    do { w[j][--k] = Lq; } while (k > 0);
    do { w[--j][k] = Lq; } while (j > 0);
#else 
    Lq = q[1][1];	/* initialize kept values: */
    Lw = w[1][1];	/* guaranteed to be >= infill */
    for (j = 0, k = 1, wp = &w[j][k], qp = &q[j][k]; k < wideM; k++)
    {					/*_______RIGHTWARD_______*/
      if (*wp > T) {
        Lw    =       FOLD    + FNEW**wp     + IR2*wp[wM]        + wp[W];
        *qp++ = Lq = (FOLD*Lq + FNEW**wp**qp + IR2*wp[wM]*qp[wM] + wp[W]*qp[W])/Lw;
         wp++ ;
      } else {
        Lw    =       FKPT    + FSUB**wp     + wp[1]       + IR2*wp[wM]        + wp[W]       + IR2*wp[wP];
        *qp++ = Lq = (FKPT*Lq + FSUB**wp**qp + wp[1]*qp[1] + IR2*wp[wM]*qp[wM] + wp[W]*qp[W] + IR2*wp[wP]*qp[wP])/Lw;
        *wp++ = infill;
      }
    }
    if (*wp <= T) *qp  = Lq;		/* fill corner */
    for (wp += W,  qp += W ; ++j < tallM ; )
    {					/*_______DOWNWARD_______*/
      if (*wp > T) {
        Lw   =       FOLD    + FNEW**wp     + wp[-1]        + IR2*wp[wO];
        *qp  = Lq = (FOLD*Lq + FNEW**wp**qp + wp[-1]*qp[-1] + IR2*wp[wO]*qp[wO])/Lw;
      } else {
        Lw   =       FKPT    + FSUB**wp     + wp[-1]        + IR2*wp[wO]        + IR2*wp[wM]        + wp[W];
        *qp  = Lq = (FKPT*Lq + FSUB**wp**qp + wp[-1]*qp[-1] + IR2*wp[wO]*qp[wO] + IR2*wp[wM]*qp[wM] + wp[W]*qp[W])/Lw;
        *wp  = infill;
      }  wp += W, qp += W;
    }
    if (*wp-- <= T)  *qp-- = Lq;	/* fill corner */
    for (; --k > 0;)
    {					/* _______LEFTWARD_______ */
      if (*wp > T) {
        Lw    =       FOLD    + FNEW**wp     + wp[wN]        + IR2*wp[wQ];
        *qp-- = Lq = (FOLD*Lq + FNEW**wp**qp + wp[wN]*qp[wN] + IR2*wp[wQ]*qp[wQ])/Lw;
         wp-- ;
      }
      else {
        Lw    =       FKPT    + FSUB**wp     + wp[-1]        + IR2*wp[wO]        + wp[wN]        + IR2*wp[wQ];
        *qp-- = Lq = (FKPT*Lq + FSUB**wp**qp + wp[-1]*qp[-1] + IR2*wp[wO]*qp[wO] + wp[wN]*qp[wN] + IR2*wp[wQ]*qp[wQ])/Lw;
        *wp-- = infill;
      }
    }
    if (*wp <= T) *qp  = Lq;		/* fill corner */
    for (wp -= W,  qp -= W ; --j > 0;)
    {					/* _______UPWARD_______ */
      if (*wp > T) {
        Lw   =       FOLD    + FNEW**wp     + wp[1]       + IR2*wp[wP];
        *qp  = Lq = (FOLD*Lq + FNEW**wp**qp + wp[1]*qp[1] + IR2*wp[wP]*qp[wP])/Lw;
       }
      else {
        Lw   =       FKPT    + FSUB**wp     + wp[1]       + wp[wN]        + IR2*wp[wQ]        + IR2*wp[wP];
        *qp  = Lq = (FKPT*Lq + FSUB**wp**qp + wp[1]*qp[1] + wp[wN]*qp[wN] + IR2*wp[wQ]*qp[wQ] + IR2*wp[wP]*qp[wP])/Lw;
        *wp  = infill;
      }  wp -= W, qp -= W;
    }
    if (*wp <= T)  *qp = Lq;	/* fill corner */
#endif
  }
}


void
rowPlugFlt ( ppf dstQ, ppf accW, int xi, int yi, unt wide, unt tall
           , double infimum, double dmean)
{
  int j = yi+tall;
  double infillwt = infimum * 2.0;
  if (infillwt > 1.0)  infillwt = 1.0;
  
  if (xi != 0 || yi != 0)
    while (--j >= yi)  dstQ[j] += xi;
  rowPlug_( dstQ+yi, accW, wide, tall, 0 /*radius*/, (wide-1)/2, (tall-1)/2
          , PLUG_PLUG, infimum, infillwt, dmean);
}
