/*****************************************************************************
rgbPlugFlt.c    S.R.Lines (spraxlo@ai.mit.edu)    93.7.24-9.14

  Goes in clockwise square spiral from initial point xS,yS until distance
  from xS,yS == radius or hitting the closest boundary.
  The routines here use rows as rows, ie, as ptrs to ptrs, and don't try to
  get around the double array reference (potential double fetch) for every
  pxl.  If your rows really are in one contiguous (but not necessarily packed)
  block, then use memPlug.c functions instead (with the parameter scan =
  row[1] - row[0).

NB: The pixvals in dsQ must already have been divided by corresponding
    values in the dsW before calling rgbPlugFlt; rgbNwts is the usual
    method.
    (This may seem kludgy, but it's both simpler and more
    efficient to perform this normalization step in one prior pass.)
    
Optimization / Fine-tuning:


DONE:
10.15	Replaced all double indexing of row pointers by singly indexed
	pxl pointers.  Compiled either -g or -O2, the ptrs implementation
	is consistently faster.  This stage saved as rgbPlugFlt.c.ptrs.
TODO:
10.16	Use tmp variables to eliminate repeated identical ptr indexing.
  
  Different weight factors FINS & FOUT for neighbors in & out of spiral.
  Use separate variables for j-1,j, and j+1, etc. -OR-
  Use three pointers in parallel instead of jumping rows while processing
  	a single line. 
  For higher quality, combine the results of clockwise & ccw spirals.
  Split thresholding into two levels, bounded by infimum & medium, with
    separate neighbor-weighting schemes.  The [inf < val < med]-case needs
    fewer neighbors.)
  Make a faster version which assumes all acc. weights are previously
    normalized to 0.0 or 1.0 -- they can even be stored as int (unchar is
    probably not any faster, just smaller).  (As of 93.9.12, the difference
    in results between prenormed vals-n-wts and only prenormed values is
    imperceptible.)
  Separate function for single-pixel width border (last step).
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

/*__________________________________________________________________________*|
Ordering & Notation: 3x3 pixel grid is numbered in the same sense as the
trajectory of the spiral fill (also used in aaTrace, etc):
6 7 8        6 N 8   corres.   (-w-1) (-w ) (1-w)   tho SGI,     (+w) (1+w)
5 0 1  -OR-  W C E   to ptr.   (  -1) ( 0 ) (1  )   NeXT, etc.   ( 0) (1  )
4 3 2        4 S 2   offsets:  (+w-1) (+w ) (1+w)   FB order is: (-w) (1-w)
|*__________________________________________________________________________*/


#define tryRIGHT \
{\
  /* WQQ3("RIGHT:	(% 5d,% 5d)\n",j,k); */ \
  /* assert(dsW[j][k] <= 1.001F); */ \
  if (*w > T) {\
    w_C = *w*FNEW;\
    vW =         FOLD    + w_C    + w[wN]       + w[wS];\
    vR = *q++ = (FOLD*vR + w_C**q + w[wN]*q[qN] + w[wS]*q[qS])/vW;\
    vG = *q++ = (FOLD*vG + w_C**q + w[wN]*q[qN] + w[wS]*q[qS])/vW;\
    vB = *q++ = (FOLD*vB + w_C**q + w[wN]*q[qN] + w[wS]*q[qS])/vW;\
    w++;\
    }\
  else {\
    w_C = *w*FSUB;\
    vW =         FKPT    + w_C    + w[1]      + w[wN]       + w[wS]       + IR2*w[w8]       + IR2*w[w2];\
    vR = *q++ = (FKPT*vR + w_C**q + w[1]*q[3] + w[wN]*q[qN] + w[wS]*q[qS] + IR2*w[w8]*q[q8] + IR2*w[w2]*q[q2])/vW;\
    vG = *q++ = (FKPT*vG + w_C**q + w[1]*q[3] + w[wN]*q[qN] + w[wS]*q[qS] + IR2*w[w8]*q[q8] + IR2*w[w2]*q[q2])/vW;\
    vB = *q++ = (FKPT*vB + w_C**q + w[1]*q[3] + w[wN]*q[qN] + w[wS]*q[qS] + IR2*w[w8]*q[q8] + IR2*w[w2]*q[q2])/vW;\
    /* *w++ = infill; */ \
     w++;\
  }/*q should already be pointing to the right place */ \
}
#define tryLEFT \
{\
  /* WQQ3("LEFT:	(% 5d,% 5d)\n",j,k); */ \
  if (*w > T) {\
    vW   =       FOLD    + FNEW**w    + w[wN]       + w[wS];\
    *q++ = vR = (FOLD*vR + FNEW**w**q + w[wN]*q[qN] + w[wS]*q[qS])/vW;\
    *q++ = vG = (FOLD*vG + FNEW**w**q + w[wN]*q[qN] + w[wS]*q[qS])/vW;\
    *q   = vB = (FOLD*vB + FNEW**w**q + w[wN]*q[qN] + w[wS]*q[qS])/vW;\
     w--;\
  }\
  else {\
    vW   =       FKPT    + FSUB**w    + w[-1]       + w[wN]       + w[wS]       + IR2*w[w4]       + IR2*w[w6];\
    *q++ = vR = (FKPT*vR + FSUB**w**q + w[-1]*q[-3] + w[wN]*q[qN] + w[wS]*q[qS] + IR2*w[w4]*q[q4] + IR2*w[w6]*q[q6])/vW;\
    *q++ = vG = (FKPT*vG + FSUB**w**q + w[-1]*q[-3] + w[wN]*q[qN] + w[wS]*q[qS] + IR2*w[w4]*q[q4] + IR2*w[w6]*q[q6])/vW;\
    *q   = vB = (FKPT*vB + FSUB**w**q + w[-1]*q[-3] + w[wN]*q[qN] + w[wS]*q[qS] + IR2*w[w4]*q[q4] + IR2*w[w6]*q[q6])/vW;\
    *w-- = infill;\
  }  q  -= 5;\
}
#define tryDOWN \
{\
  /* WQQ3("DOWN:	(% 5d,% 5d)\n",j,k); */\
  w = &dsW[j][k];\
  if (*w > T) {\
    vW   =       FOLD    + w[-1]       + FNEW**w    + w[1];\
    *q++ = vR = (FOLD*vR + w[-1]*q[-3] + FNEW**w**q + w[1]*q[3])/vW;\
    *q++ = vG = (FOLD*vG + w[-1]*q[-3] + FNEW**w**q + w[1]*q[3])/vW;\
    *q   = vB = (FOLD*vB + w[-1]*q[-3] + FNEW**w**q + w[1]*q[3])/vW;\
  }\
  else {\
    vW   =       FKPT    + FSUB**w    + w[-1]       + w[1]      + IR2*w[w4]       + w[wS]       + IR2*w[w2];\
    *q++ = vR = (FKPT*vR + FSUB**w**q + w[-1]*q[-3] + w[1]*q[3] + IR2*w[w4]*q[q4] + w[wS]*q[qS] + IR2*w[w2]*q[q2])/vW;\
    *q++ = vG = (FKPT*vG + FSUB**w**q + w[-1]*q[-3] + w[1]*q[3] + IR2*w[w4]*q[q4] + w[wS]*q[qS] + IR2*w[w2]*q[q2])/vW;\
    *q   = vB = (FKPT*vB + FSUB**w**q + w[-1]*q[-3] + w[1]*q[3] + IR2*w[w4]*q[q4] + w[wS]*q[qS] + IR2*w[w2]*q[q2])/vW;\
    *w   = infill;\
  }  w  += wS, q += nextDN;\
}
#define tryUPTOP \
{\
  /* WQQ3("UP: 	(% 5d,% 5d)\n",j,k); */\
  w = &dsW[j][k];\
  if (*w > T) {\
    vW   =       FOLD    + w[-1]       + FNEW**w    + w[1];\
    *q++ = vR = (FOLD*vR + w[-1]*q[-3] + FNEW**w**q + w[1]*q[3])/vW;\
    *q++ = vG = (FOLD*vG + w[-1]*q[-3] + FNEW**w**q + w[1]*q[3])/vW;\
    *q   = vB = (FOLD*vB + w[-1]*q[-3] + FNEW**w**q + w[1]*q[3])/vW;\
  }\
  else {\
    vW   =       FKPT    + w[-1]       + FSUB**w    + w[1]      + IR2*w[w8]       + w[wN]       + IR2*w[w6];\
    *q++ = vR = (FKPT*vR + w[-1]*q[-3] + FSUB**w**q + w[1]*q[3] + IR2*w[w8]*q[q8] + w[wN]*q[qN] + IR2*w[w6]*q[q6])/vW;\
    *q++ = vG = (FKPT*vG + FSUB**w**q + w[-1]*q[-3] + w[1]*q[3] + IR2*w[w8]*q[q8] + w[wN]*q[qN] + IR2*w[w6]*q[q6])/vW;\
    *q   = vB = (FKPT*vB + FSUB**w**q + w[-1]*q[-3] + w[1]*q[3] + IR2*w[w8]*q[q8] + w[wN]*q[qN] + IR2*w[w6]*q[q6])/vW;\
    *w   = infill;\
  }  w  -= wS, q += nextUP;\
}
	/* After the spiral, dL,dR,dT,dB will no longer needed for distance control, so reassign */
#define brkRIGHT	{ WQQ3("break RIGHT:	(% 5d,% 5d)\n",j,k); eR--; break;}
#define brkLEFT 	{ WQQ3("break LEFT: 	(% 5d,% 5d)\n",j,k); eL++; break;}
#define brkDOWN 	{ WQQ3("break DOWN: 	(% 5d,% 5d)\n",j,k); eB--; break;}
#define brkUPTOP	{ WQQ3("break UPTOP:	(% 5d,% 5d)\n",j,k); eT++; break;}


/*=================================================================================*/
/*=================================================================================*/
static void
rgbPlug_( ppf dsQ, ppf dsW
        , unt wide, unt tall, int radius, int xS, int yS, int flags
        , double infimum, double infillwt, double dmean)
{
int 	d, dL,dR,dT,dB, eL,eR,eT,eB, wideM = wide-1, tallM = tall-1;
int 	wS = dsW[1]-dsW[0];	/* ptr. diff. betw. wts scan lines, or "w South" */
int 	qS = dsQ[1]-dsQ[0];	/* ptr. diff. betw. rgb scan lines, or "q South" */
int 	w2 = wS+1, w4 = wS-1, w6 = -1-wS, wN = -wS, w8 = 1-wS;
int 	q2 = qS+3, q4 = qS-3, q6 = -3-qS, qN = -qS, q8 = 3-qS;
int 	nextDN = qS-2, nextUP = (-qS-2);
regint	j,k;
flt 	*q, *w;
flt 	vW, T = (flt)infimum, infill = (flt)infillwt, vR = (flt)dmean, vG = (flt)dmean, vB = (flt)dmean;
flt 	w_C;
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
    for (w = &dsW[j][k], q = &dsQ[j][k*3], eR = xS+d; k < eR; k++)
      tryRIGHT
    if (k >= dR)
      brkRIGHT
    for (w = &dsW[j][k], q = &dsQ[j][k*3], eB = yS+d; j < eB; j++)
      tryDOWN
    if (j >= dB)
      brkDOWN
    for (w = &dsW[j][k], q = &dsQ[j][k*3], eL = xS-d; k > eL; k--)
      tryLEFT      
    if (k <= dL)
      brkLEFT
    for (w = &dsW[j][k], q = &dsQ[j][k*3], eT = yS-d; j > eT; j--)
      tryUPTOP
    if (j <= dT)
      brkUPTOP
  }
  /* warn("END OF SPIRAL: eL,eR,eT,eB: %3d %3d %3d %3d",eL,eR,eT,eB); */
/*******************
  dsQ[eT][eL] = 255.0;
  dsQ[eT][eR] = 255.0;
  dsQ[eB][eL] = 255.0;
  dsQ[eB][eR] = 255.0;
  dsQ[j][k]   = 128.0;
********************/

  if (flags & PLUG_RECT) {

    k  = eR;	/* eR is the rightmost column already filled */
    j  = eT;	/* eT is the topmost row already filled */
    q = &dsQ[j][k*3]; vR = *q, vG = q[1], vB = q[2];
    WQQ4("Fill RIGHT RECT from (% 3d, %3d), initval % .3f\n",j,k+1,vR);
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
    vR = dsQ[j][k+1];
    WQQ4("Fill  LEFT RECT from (% 3d, %3d), initval % .3f\n",j,k-1,vR);
    --j;
    while (--k > 0) {
      while (++j <= eB)
        tryDOWN
      if    (--k <= 0)  break;
      while (--j >= eT)
        tryUPTOP
    }
    /*******************
    dsQ[eT  ][eL] =   0.0;
    dsQ[eT+1][eL] = 255.0;
    dsQ[eT+2][eL] =   0.0;
    *******************/
    
    k  = 0;
    j  = eT;	/* eT is the topmost row already filled */
    vR = dsQ[j][k+1];
    WQQ4("Fill   TOP RECT from (% 3d, %3d), initval % .3f\n",j-1,k,vR);
    while (--j > 0) {
      while (++k < wideM)
        tryRIGHT
      if    (--j <= 0)  break;
      while (--k >  0)
        tryLEFT
    }
    /*******************
    dsQ[eT][eL  ] =   0.0;
    dsQ[eT][eL+1] = 255.0;
    dsQ[eT][eL+2] =   0.0;
    *******************/

    k  = 0;
    j  = eB;	/* eB is the bottom-most row already filled */
    vR = dsQ[j][k+1];
    WQQ4("Fill   BOT RECT from (% 3d, %3d), initval % .3f\n",j+1,k,vR);
    while (++j < tallM) {
      while (++k < wideM)
        tryRIGHT
      if (++j >= tallM)  break;
      while (--k >  0)
        tryLEFT
    }
    /*******************
    dsQ[eB][eL  ] =   0.0;
    dsQ[eB][eL+1] = 255.0;
    dsQ[eB][eL+2] =   0.0;
    *******************/
  }
  
  /****
  WQQ("rgbPlug: Returning early, after ALL FOUR RECT FILLS");
  return;
  ****/

  /** THE FOLLOWING SHOULD BE A SEPARATE FUNCTION **/
  
  if (flags & PLUG_LAST) {
#if 0
    k = j = 0;
    do { dsW[j][++k] = vR; } while (k < wideM);
    do { dsW[++j][k] = vR; } while (j < tallM);
    do { dsW[j][--k] = vR; } while (k > 0);
    do { dsW[--j][k] = vR; } while (j > 0);
#else 
    vW = dsW[1][1];
    vR = dsQ[1][3];	/* initialize kept values */
    vG = dsQ[1][4];	/* initialize kept values */
    vB = dsQ[1][5];	/* initialize kept values */

    /* for (w = &dsW[0][1], q = &dsQ[0][3], k = wide-2; k-- ; ) */
    for (j = 0, k = 1, w = &dsW[0][1], q = &dsQ[0][3]; k < wideM; k++)
    {					/* _______RIGHTWARD_______ */
      if (*w > T) {
        vW   =       FOLD    + FNEW**w    + IR2*w[w4]       + w[wS];
        *q++ = vR = (FOLD*vR + FNEW**w**q + IR2*w[w4]*q[q4] + w[wS]*q[qS])/vW;
        *q++ = vG = (FOLD*vG + FNEW**w**q + IR2*w[w4]*q[q4] + w[wS]*q[qS])/vW;
        *q++ = vB = (FOLD*vB + FNEW**w**q + IR2*w[w4]*q[q4] + w[wS]*q[qS])/vW;
         w++ ;
      }
      else {
        vW   =       FKPT    + FSUB**w    + w[1]      + IR2*w[w4]       + w[wS]      + IR2*w[w2];
        *q++ = vR = (FKPT*vR + FSUB**w**q + w[1]*q[3] + IR2*w[w4]*q[q4] + w[wS]*q[qS] + IR2*w[w2]*q[q2])/vW;
        *q++ = vG = (FKPT*vG + FSUB**w**q + w[1]*q[3] + IR2*w[w4]*q[q4] + w[wS]*q[qS] + IR2*w[w2]*q[q2])/vW;
        *q++ = vB = (FKPT*vB + FSUB**w**q + w[1]*q[3] + IR2*w[w4]*q[q4] + w[wS]*q[qS] + IR2*w[w2]*q[q2])/vW;
        *w++ = infill;
      }
    }
    
    if (*w <= T)  *q  = vR;  q[1] = vG;  q[2] = vB;	/* fill corner */
    /* for (w += wS,  q += qS, j = tall-2; j-- ; ) */
    for (w += wS,  q += qS ; ++j < tallM ; )
    {					/* _______DOWNWARD_______ */
      if (*w > T) {
        vW   =       FOLD    + FNEW**w    + w[-1]       + IR2*w[w6];
        *q++ = vR = (FOLD*vR + FNEW**w**q + w[-1]*q[-3] + IR2*w[w6]*q[q6])/vW;
        *q++ = vG = (FOLD*vG + FNEW**w**q + w[-1]*q[-3] + IR2*w[w6]*q[q6])/vW;
        *q   = vB = (FOLD*vB + FNEW**w**q + w[-1]*q[-3] + IR2*w[w6]*q[q6])/vW;
      }
      else {
        vW   =       FKPT    + FSUB**w    + w[-1]       + IR2*w[w6]       + IR2*w[w4]       + w[wS];
        *q++ = vR = (FKPT*vR + FSUB**w**q + w[-1]*q[-3] + IR2*w[w6]*q[q6] + IR2*w[w4]*q[q4] + w[wS]*q[qS])/vW;
        *q++ = vG = (FKPT*vG + FSUB**w**q + w[-1]*q[-3] + IR2*w[w6]*q[q6] + IR2*w[w4]*q[q4] + w[wS]*q[qS])/vW;
        *q   = vB = (FKPT*vB + FSUB**w**q + w[-1]*q[-3] + IR2*w[w6]*q[q6] + IR2*w[w4]*q[q4] + w[wS]*q[qS])/vW;
        *w   = infill;
      }  w += wS, q += nextDN;
    }
    if (*w-- <= T)  *q = vR;  q[1] = vG;  q[2] = vB;	/* fill corner */
    /* warn("before LEFT   along bottom, starting at [j,k] == [%d,%d]",j,k); */
    /* for (q -= 3, k = wide-2; k-- ;) */
    for (q -= 3; --k > 0;)
    {					/* _______LEFTWARD ALONG THE BOTTOM_______ */
      if (*w > T) {
        vW   =       FOLD    + FNEW**w    + w[wN]       + IR2*w[w8];
        *q++ = vR = (FOLD*vR + FNEW**w**q + w[wN]*q[qN] + IR2*w[w8]*q[q8])/vW;
        *q++ = vG = (FOLD*vG + FNEW**w**q + w[wN]*q[qN] + IR2*w[w8]*q[q8])/vW;
        *q   = vB = (FOLD*vB + FNEW**w**q + w[wN]*q[qN] + IR2*w[w8]*q[q8])/vW;
         w-- ;
      }
      else {
        vW   =       FKPT    + FSUB**w    + w[-1]       + IR2*w[w6]       + w[wN]       + IR2*w[w8];
        *q++ = vR = (FKPT*vR + FSUB**w**q + w[-1]*q[-3] + IR2*w[w6]*q[q6] + w[wN]*q[qN] + IR2*w[w8]*q[q8])/vW;
        *q++ = vG = (FKPT*vG + FSUB**w**q + w[-1]*q[-3] + IR2*w[w6]*q[q6] + w[wN]*q[qN] + IR2*w[w8]*q[q8])/vW;
        *q   = vB = (FKPT*vB + FSUB**w**q + w[-1]*q[-3] + IR2*w[w6]*q[q6] + w[wN]*q[qN] + IR2*w[w8]*q[q8])/vW;
        *w-- = infill;
      }  q  -= 5;
    }
    if (*w <= T)  *q  = vR;  q[1] = vG;  q[2] = vB;	/* fill corner */
    /* warn("before UPWARD along left side, starting at [j,k] == [%d,%d]",j,k); */
    /* for (w -= wS,  q -= qS, j = tall-2; j-- ;) */
    for (w -= wS, q -= qS; --j > 0;)
    {					/* _______UPWARD_______ */
      if (*w > T) {
        vW   =       FOLD    + FNEW**w    + w[1]      + IR2*w[w2];
        *q++ = vR = (FOLD*vR + FNEW**w**q + w[1]*q[3] + IR2*w[w2]*q[q2])/vW;
        *q++ = vG = (FOLD*vG + FNEW**w**q + w[1]*q[3] + IR2*w[w2]*q[q2])/vW;
        *q   = vB = (FOLD*vB + FNEW**w**q + w[1]*q[3] + IR2*w[w2]*q[q2])/vW;
       }
      else {
        vW   =       FKPT    + FSUB**w    + w[1]      + w[wN]       + IR2*w[w8]       + IR2*w[w2];
        *q++ = vR = (FKPT*vR + FSUB**w**q + w[1]*q[3] + w[wN]*q[qN] + IR2*w[w8]*q[q8] + IR2*w[w2]*q[q2])/vW;
        *q++ = vG = (FKPT*vG + FSUB**w**q + w[1]*q[3] + w[wN]*q[qN] + IR2*w[w8]*q[q8] + IR2*w[w2]*q[q2])/vW;
        *q   = vB = (FKPT*vB + FSUB**w**q + w[1]*q[3] + w[wN]*q[qN] + IR2*w[w8]*q[q8] + IR2*w[w2]*q[q2])/vW;
        *w   = infill;
      }  w  -= wS, q += nextUP;
    }
    if (*w <= T)  *q = vR;  q[1] = vG;  q[2] = vB;	/* fill corner */
#endif
  }
}


void
rgbPlugFlt ( ppf dsQ, ppf dsW, int xi, int yi, unt wide, unt tall
           , double infimum, double dmean)
{
  int j = yi+tall;
  double infillwt = infimum * 2.0;
  if (infillwt > 1.0)  infillwt = 1.0;
  
  if (xi != 0 || yi != 0)
    while (--j >= yi)  dsQ[j] += xi;
  rgbPlug_( dsQ+yi, dsW, wide, tall, 0 /*radius*/, (wide-1)/2, (tall-1)/2
          , PLUG_PLUG, infimum, infillwt, dmean);
}


