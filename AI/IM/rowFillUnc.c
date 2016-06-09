/*****************************************************************************
rowFillUnc.c    S.R.Lines (spraxlo@ai.mit.edu)    93.7.11
Doesn't yet handle interleaving of different types.
Naming scheme for fill functions:
	Blot: from ix,iy fill the maximal square, given boundaries & radius.
	Fill: from ctr, fill entire rectable except for 1 pxl border.
	Soak: from ix,iy, fill entire rectangle, even edge pxls. 
*****************************************************************************/
#include <un/unTypes.h>
#define   UNBUG 0
#include <un/unBug.h>
#include <un/unMacro.h>
#include "rowFill.h"

#define FILL_CENT	1
#define FILL_RECT	2
#define FILL_LAST	4
#define FILL_BORD	8

#define FILL_BLOT	0
#define FILL_FILL	( FILL_CENT | FILL_RECT | FILL_LAST )
#define FILL_SOAK	( FILL_CENT | FILL_RECT | FILL_BORD )

#define WNEW	4U	/* Defs as explicitly unsigned saves casts for ANSI */
#define WOLD	1U	/* --Otherwise, Sun acc warns on both SCONV & semantics of / */ 
#define WSUM	5U	/* WNEW + WOLD */
#define WKPT	4U

#define	SQRT2	(1.414213562373F)	/* Here, too, explicit float rather than */
#define IR2 	(0.707106781186F)	/* default double eliminates need for casts */

/*___________________________________________________________________________*
  Goes in clockwise square spiral from initial point ix,iy until distance
  from ix,iy == radius or hitting the closest boundary.
  The routines here use rows as rows, ie, as ptrs to ptrs, and don't try to
  get around the double array reference (potential double fetch) for every
  pxl.  If your rows really are in one contiguous (but not necessarily packed)
  block, then use memFill.c functions instead (with the parameter scan =
  row[1] - row[0).
  If mem = *row, then mem is guaranteed to be ptr to first row, but the others
  rows may not even be in one contiguous block, let alone packed in one.
 *___________________________________________________________________________*/
static
void rowFillUnc_( ppu dst_src, unt wide, unt tall
                , int ix, int iy, int radius, int flags, int initval)
{
  int	d, q, dL,dR,dT,dB, wideM = wide-1, tallM = tall-1;
  register puc *r = dst_src;
  register int j,k;
  float den, val = (flt)initval;
  
  if (radius > 0) {
    radius++;
    dL = ix-radius;  if (dL < 0)      dL = 0;
    dR = ix+radius;  if (dR > wideM)  dR = wideM;
    dT = iy-radius;  if (dT < 0)      dT = 0;
    dB = iy+radius;  if (dB > tallM)  dB = tallM;
  } else {
    dL = dT = 0;
    dR = wide;
    dB = tall; 
  }
 
/* @@NB: I don't know if (r[j][k]!='\0') makes any difference from (r[j][k]!=0) */
#define tryRIGHT	\
    {\
      /* WQQ3("RIGHT:	(% 5d,% 5d)\n",j,k); */ \
      if (r[j][k]) {\
        den = (flt)(WSUM + (r[j-1][k]!='\0') +                (r[j+1][k]!='\0'));\
        val = (val + (r[j-1][k]        + WNEW*r[j][k] + r[j+1][k]))/den;\
      }\
      else {\
        den=     WKPT+     (r[j][k+1]!='\0')+(r[j-1][k]!='\0')+(r[j+1][k]!='\0')+IR2*(r[j-1][k+1]!='\0')+IR2*(r[j+1][k+1]!='\0');\
        val=(val*WKPT+((unt)r[j][k+1]       + r[j-1][k]       + r[j+1][k])      +(flt)r[j-1][k+1]*IR2   +(flt)r[j+1][k+1]*IR2)/den;\
        r[j][k]  =  (unc)(val + 0.5);\
      }\
    }
#define tryLEFT 	\
    {\
      /* WQQ3("LEFT:	(% 5d,% 5d)\n",j,k); */ \
      if (r[j][k]) {\
        den = (flt)(WSUM + (r[j-1][k]!=0) +               (r[j+1][k]!=0));\
        val = (val + (r[j-1][k]     + WNEW*r[j][k] + r[j+1][k]))/den;\
      }\
      else {\
        den =      WKPT+     (r[j][k-1]!=0)+(r[j-1][k]!=0)+(r[j+1][k]!=0)+IR2*(r[j+1][k-1]!=0)+ IR2*(r[j-1][k-1]!=0);\
        val = (val*WKPT+((unt)r[j][k-1]    + r[j-1][k]    + r[j+1][k])   +(flt)r[j+1][k-1]*IR2+ (flt)r[j-1][k-1]*IR2)/den;\
        r[j][k]  =  (unc)(val + 0.5);\
      }\
    }
#define tryDOWN \
    {\
      /* WQQ3("DOWN:	(% 5d,% 5d)\n",j,k); */\
      if (r[j][k]) {\
        den = (flt)(WSUM + (r[j][k-1]!=0) +                (r[j][k+1]!=0));\
        val = (val + (r[j][k-1]     + WNEW*r[j][k] +  r[j][k+1]))/den;\
      }\
      else {\
        den =      WKPT+(     r[j][k-1]!=0)+(r[j][k+1]!=0)+(r[j+1][k]!=0) + IR2*(r[j+1][k-1]!=0) + IR2*(r[j+1][k+1]!=0);\
        val = (val*WKPT+((unt)r[j][k-1]    + r[j][k+1]    + r[j+1][k])    + (flt)r[j+1][k-1]*IR2 + (flt)r[j+1][k+1]*IR2)/den;\
        r[j][k]  =  (unc)(val + 0.5);\
      }\
    }
#define tryUPTOP \
    {\
      /* WQQ3("UP: 	(% 5d,% 5d)\n",j,k); */\
      if (r[j][k]) {\
        den = (flt)(WSUM + (r[j][k-1]!=0)                + (r[j][k+1]!=0));\
        val = (val + (r[j][k-1]     + WNEW*r[j][k] +  r[j][k+1]))/den;\
      }\
      else {\
        den =     WKPT+(     r[j][k-1]!=0)+(r[j][k+1]!=0)+(r[j-1][k]!=0) + IR2*(r[j-1][k-1]!=0) + IR2*(r[j-1][k+1]!=0);\
        val =(val*WKPT+((unt)r[j][k-1]    + r[j][k+1]    + r[j-1][k])    + (flt)r[j-1][k-1]*IR2 + (flt)r[j-1][k+1]*IR2)/den;\
        r[j][k]  =  (unc)(val + 0.5);\
      }\
    }
#define brkRIGHT	{ WQQ3("break RIGHT:	(% 5d,% 5d)\n",j,k); break;}
#define brkLEFT 	{ WQQ3("break LEFT:	(% 5d,% 5d)\n",j,k); break;}
#define brkDOWN 	{ WQQ3("break DOWN:	(% 5d,% 5d)\n",j,k); break;}
#define brkUPTOP	{ WQQ3("break UPTOP:	(% 5d,% 5d)\n",j,k); break;}
    

  /*_______________________________________________________________________*/
  for (j = iy, k = ix, d = 1;  ; d++)  {
    /* WQQ3("SpiralTopLeft:	yx=(% 5d,% 5d)\n",j,k); */
    for (q = ix+d; k < q; k++)
      tryRIGHT
    if (k >= dR)
      brkRIGHT
    for (q = iy+d; j < q; j++)
      tryDOWN
    if (j >= dB)
      brkDOWN
    for (q = ix-d; k > q; k--)
      tryLEFT      
    if (k <= dL)
      brkLEFT
    for (q = iy-d; j > q; j--)
      tryUPTOP
    if (j <= dT)
      brkUPTOP
  }


  if (flags & FILL_RECT) {

    WQQ3("Fill RIGHT RECT from (% 3d, %3d)\n",j,k);
    val = r[iy-d+1][ix+d-1];
    for (k =   ix+d              ; k < wideM; k++) {
      for (j = iy-d, q = iy+d; ++j < q;)
        tryRIGHT
      if (++k >= wideM) break;
      for (q = iy - d; --j > q;)
        tryUPTOP
    }
    /* r[iy-d+1][ix+d] = 255; */

    WQQ3("Fill  LEFT RECT from (% 3d, %3d)\n",j,k);
    val = r[iy-d+1][ix-d+1];
    for (k =   ix-d              ; k > 0; k--) {
      for (j = iy-d, q = iy+d; ++j < q;)
        tryDOWN
      if (--k <= 0) break;
      for (q = iy - d; --j > q;)
        tryUPTOP
    }
    /* r[iy-d+1][ix-d] = 255; */
  
    WQQ3("Fill   TOP RECT from (% 3d, %3d)\n",j,k);
    val = r[iy-d+1][ix-d+1];
    for (  j = iy-d; j > 0   ; j--) {
      for (k = 0   ; ++k < wideM;)
        tryRIGHT
      if (--j <= 0)  break;
      for (; --k > 0;)
        tryLEFT
    }
    /* r[iy-d][1] = 255; */
  
    WQQ3("Fill   BOT RECT from (% 3d, %3d)\n",j,k);
    val = r[iy-d+1][ix-d+1];
    for (  j = iy+d; j < tallM; j++) {
      for (k = 0   ; ++k < wideM;)
        tryRIGHT
      if (++j >= tallM)  break;
      for (; --k > 0;)
        tryLEFT
    }
    /* r[iy+d][1] = 255; */
  }
  
  
  if (flags & FILL_LAST) {
#if 0
    k = j = 0;
    do { r[j][++k] = (unc)val; } while (k < wideM);
    do { r[++j][k] = (unc)val; } while (j < tallM);
    do { r[j][--k] = (unc)val; } while (k > 0);
    do { r[--j][k] = (unc)val; } while (j > 0);
#else 
    val = r[1][1];
    for (j = 0, k = 1; k < wideM; k++)
    {
      if (r[j][k]) {
        den = WSUM +                (r[j+1][k]!=0) + IR2*(r[j+1][k-1]!=0);
        val = (val + (r[j][k]*WNEW + r[j+1][k])    + (flt)r[j+1][k-1]*IR2)/den;
      }
      else {
        den =      WKPT+     (r[j][k+1]!=0)+(r[j+1][k]!=0)+IR2*(r[j+1][k-1]!=0)+IR2*(r[j+1][k+1]>0);
        val = (val*WKPT+((unt)r[j][k+1]    + r[j+1][k])   +(flt)r[j+1][k-1]*IR2+(flt)r[j+1][k+1])/den;
        r[j][k]  =  (unc)val;
      }
    }
#if 1	/* Sun's acc -Xa will complain about the rest; seems safe to ignore it. */
    r[j++][k] = (unc)val;			/* fill corner */
    for (; j < tallM; j++)
    { 
      if (r[j][k] > 0) {
        den = WSUM + (r[j][k-1]>0)  +               IR2*(r[j-1][k-1]>0);
        val = (val +  r[j][k-1]     + r[j][k]*WNEW +IR2* r[j-1][k-1]) / den;
      }
      else {
        den =     WKPT+(r[j][k-1]>0)+IR2*(r[j-1][k-1]>0) +IR2*(r[j+1][k-1]>0)+(r[j+1][k]>0);
        val =(val*WKPT+ r[j][k-1]   +IR2* r[j-1][k-1]    +IR2* r[j+1][k-1]   + r[j+1][k])/den;
        r[j][k]  = (unc)val;
      }
    }
    r[j][k] = (unc)val;
    for (; --k > 0;)
    {
      if (r[j][k] > 0) {
        den =               WSUM + (r[j-1][k]>0)+IR2*(r[j-1][k+1]>0);
        val =(val + r[j][k]*WNEW +  r[j-1][k]   +IR2* r[j-1][k+1])/den;
      }
      else {
        den =     WKPT + (r[j][k-1]>0) + IR2*(r[j-1][k-1]>0) + (r[j-1][k]>0) + IR2*(r[j-1][k+1]>0);
        val =(val*WKPT +  r[j][k-1]    + IR2* r[j-1][k-1]    +  r[j-1][k]    + IR2* r[j-1][k+1])/den;
        r[j][k]  = (unc)val;
      }
    }
    r[j][k] = (unc)val;
    for (; --j > 0;)
    {
      if (r[j][k] > 0) {
        den = WSUM + (r[j][k+1]>0)            + IR2*(r[j+1][k+1]>0);
        val = (val +  r[j][k+1]+ r[j][k]*WNEW + IR2* r[j+1][k+1])/den;
      }
      else {
        den =     WKPT + (r[j-1][k]>0) + IR2*(r[j-1][k+1]>0)+(r[j][k+1]>0)+IR2*(r[j+1][k+1]>0);
        val =(val*WKPT +  r[j-1][k]    + IR2* r[j-1][k+1]   + r[j][k+1]   +IR2* r[j+1][k+1])/den;
        r[j][k]  = (unc)val;
      }
    }
    r[j][k] = (unc)val;
#endif
  }
  
  /******* r[j][k] = 255;  --was to verify reported position ****/

#endif
}




void rowFillUnc (puc *row, int xi, int yi, unt wide, unt tall, int val)
{
  int j = yi+tall;
  while (--j >= yi)  row[j] += xi;
  rowFillUnc_(row+yi, wide, tall, (wide-1)/2, (tall-1)/2, 0, FILL_FILL, val);
}

void rowSoakUnc ( puc *row, unt wide, unt tall, int val)
{ /* Soak does everything as if there were borders -- there better be! */
  rowFillUnc_(row, wide+2, tall+2,  (wide+1)/2, (tall+1)/2, 0, FILL_SOAK, val);
}

void rowBlotUnc ( puc *row, unt wide, unt tall
                , int ix, int iy, int radius, int val)
{  
  rowFillUnc_(row, wide, tall, ix, iy, radius, FILL_BLOT, val);
}






















