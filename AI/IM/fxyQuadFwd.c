/*****************************************************************************
fxyQuadFwd.c    S.R.Lines (spraxlo@ai.mit.edu)    94.1.7
Combine two 2D vector fields (linear warping) into one quadratic warping fld.
As in all Newtonian polynomial interpolation, 3 versions are possible:
forward, centered, and backward.  For example, say we know the pixval at 
absolute location X0=[x,y] moves to X1=[x1,y1] and then from there to X2=[x2,y2]
(forward case).  In these coordinates the piecewise linear warping would be
	P(t) = X0 + 2 * t * (X1 - X0) for t <= 1/2 and
	P(t) = X1 + (2*t-1)*(X2 - X1) = 2*(1-t)*X1 + (2*t-1)*X2 for t > 1/2,
whereas the continuous quadratic warping would be
	Q(t) = A + B*t + C*t^2,
where A,B,C are determined from Q(0) = X0, Q(1/2) = X1, and Q(1) = X2 to be
	A = X0, B = -3*X0 + 4*X1 - X2, and C = 2*X0 - 4*X1 + 2*X2.
Note that A=X0 is just the location (array indices) where the computed values
of B and C are to be stored (and that the storage req. for X1,X2 is the same
as that for B,C).  
Usually we use relative coordinates, i.e., the displacements
	X = X0 (zero displacement), X' = X1 - X0, and X" = X2 - X1.  Then
	A = X, B = 3*X' - X", and C = 2*(X" - X')
which is simpler, although we must travel along X' in order to get to X".
NB:	The destination storage format is interleaved: the values in each
	row of float **xyB alternate between x and y B-coefficients, etc.
NB:	The source bounds xS..yE  are for keeping value-extraction to within
	real array (not nominal) bounds, not for selecting a sub-block within
	a larger rectangle (the latter has to be done beforehand).
****************************************************************************/
#include <assert.h>
#include <un/unBug.h>
#include <un/unTypes.h>
#include <un/unMath.h>
#include <rowZero.h>
#include <fxyQuad.h>

void
fxyQuadFwd( ppf qaX, ppf qaY, ppf *fvX, ppf *fvY
          , int sX, int sY, unt sW, unt sH, dbl trs)
{
ppf     vOMX = fvX[1] + sY, vOMY = fvY[1] + sY; /* vOO is zero */
ppf     vOPX = fvX[2] + sY, vOPY = fvY[2] + sY;
pfl     pX, pY, pOMX, pOMY, pOPX, pOPY;
dbl     coeff1 = (1.0     - trs)*trs*4.0;
dbl     coeff2 = (2.0*trs - 1.0)*trs;
int     j,k, jE = sY+sW, kE = sX+sW;

  qaX += sY, qaY += sY;

  for (j = sY; j < jE; j++) {

    pOMX = *vOMX++ + sX;
    pOMY = *vOMY++ + sX;
    pOPX = *vOPX++ + sX;
    pOPY = *vOPY++ + sX;
    pX   = *qaX++;
    pY   = *qaY++;

    for (k = sX; k < kE; k++) {

      *pX++ = (flt)(coeff1 * *pOMX++ + coeff2 * *pOPX++);
      *pY++ = (flt)(coeff1 * *pOMY++ + coeff2 * *pOPY++);

    }
  }
}

void
fxyQuadCtr( ppf qaX, ppf qaY, ppf *fvX, ppf *fvY
          , int sX, int sY, unt sW, unt sH, dbl trs)
{
ppf     vOMX = fvX[1] + sY, vOMY = fvY[1] + sY;	/* vOO is zero */
ppf     vOPX = fvX[2] + sY, vOPY = fvY[2] + sY;
pfl     pX, pY, pOMX, pOMY, pOPX, pOPY;
dbl	t2mt_2 = (trs*trs - trs)*0.5;
dbl	t2pt_2 = (trs*trs + trs)*0.5;
int     j,k, jE = sY+sW, kE = sX+sW;

  qaX += sY, qaY += sY;

  for (j = sY; j < jE; j++) {

    pOMX = *vOMX++ + sX;
    pOMY = *vOMY++ + sX;
    pOPX = *vOPX++ + sX;
    pOPY = *vOPY++ + sX;
    pX   = *qaX++;
    pY   = *qaY++;

    for (k = sX; k < kE; k++) {

      *pX++ = (flt)(t2mt_2 * *pOMX++ + t2pt_2 * *pOPX++);
      *pY++ = (flt)(t2mt_2 * *pOMY++ + t2pt_2 * *pOPY++);

    }
  }
}




void
fxyQuadFwdBC(ppf xyB, ppf xyC, ppf *fvX, ppf *fvY, int sX, int sY, unt sW, unt sH)
{
  int   inx, iny;
  int	xS=0, yS=0, xE=sW-1, yE=sH-1;
  unt   j, k;
  ppf	fx2 = fvX[2], fy2 = fvY[2];
  pfl   dB, dC, sx1, sy1;
  flt   dx1, dy1, dx2, dy2;

  for (j = 0; j < sH; j++) {
    dB  = *xyB++ ; dC  = *xyC++;
    sx1 = fvX[1][j];
    sy1 = fvY[1][j];

    for (k = 0; k < sW; k++) {
      dx1   = *sx1++;
      dy1   = *sy1++;
      inx   = k - IRINTF(dx1);  /* nearest neighbor coords for 2nd vct-fld */
      if      (inx < xS)  inx = 0 ;
      else if (inx > xE)  inx = xE;
      iny   = j - IRINTF(dy1);
      if      (iny < yS)  iny = 0 ;
      else if (iny > yE)  iny = yE;

      dx2   = fx2[iny][inx];    /* X" = X2 - X1 */
      dy2   = fy2[iny][inx];

      *dB++ = dx2 - dx1*3;      /* Bx   */
      *dB++ = dy2 - dy1*3;      /* By   */
      *dC++ = (dx1 - dx2)*2;    /* Cx   */
      *dC++ = (dy1 - dy2)*2;    /* Cy   */
    }
  }
}


/* Usual case: fx2 = flow(x1,x2), not fx2 = flow(x0,x2), etc.  Not optimized.  */
void
fxyQuadFwdPreWarpNen ( ppf xyB, ppf xyC, ppf prB, ppf prC, ppf srcB, ppf srcC
                     , ppf fx1, ppf fy1, ppf fx2, ppf fy2
                     , unt dW, unt dH, int xS, int yS, int xE, int yE)
{
  int   inx, iny;
  pfl   dB, dC, pB, pC, sx1, sy1;
  flt	dx1, dy1, dx2, dy2;
  unt   j, k;

  for (j = 0; j < dH; j++) {
    dB  = *xyB++ ; dC  = *xyC++;
    pB  = *prB++ ; pC  = *prC++;
    sx1 = *fx1++ ; sy1 = *fy1++;

    for (k = 0; k < dW; k++) {
      dx1   = *sx1++;
      dy1   = *sy1++;
      inx   = k - IRINTF(dx1);	/* nearest neighbor coords for 2nd vct-fld */
      if      (inx < xS)  inx = 0 ;
      else if (inx > xE)  inx = xE;
      iny   = j - IRINTF(dy1);
      if      (iny < yS)  iny = 0 ;
      else if (iny > yE)  iny = yE;
      *pB++ = srcB[iny][inx];

      dx2   = fx2[iny][inx];
      dy2   = fy2[iny][inx];
#if 0
      *dB++ = dx2 - dx1*3;	/* Bx	*/
      *dB++ = dy2 - dy1*3;	/* By	*/
      *dC++ = (dx1 - dx2)*2;	/* Cx	*/
      *dC++ = (dy1 - dy2)*2;	/* Cy	*/
#else
      *dB++ = dx1;	/* Bx	*/
      *dB++ = dy1;	/* By	*/
      *dC++ = 0.0;
      *dC++ = 0.0;
#endif

      dx1  += dx2;
      dy1  += dy2;
      inx   = k - IRINTF(dx1);
      if      (inx < xS)  inx = 0 ;
      else if (inx > xE)  inx = xE;
      iny   = j - IRINTF(dy1);
      if      (iny < yS)  iny = 0 ;
      else if (iny > yE)  iny = yE;
      *pC++ = srcC[iny][inx];
    }
  }
}


void
fxyQuadFwdSpatBil ( ppf dst, ppf wts, ppf qxyB, ppf qxyC
                  , ppf pre0, ppf pre1, ppf pre2, unt dW, unt dH, dbl trans)
{
  unt   j, k;
  int   kT, jT, wM = dW-1, hM = dH-1;
  pfl   pB, pC, p0, p1, p2;
  flt	xT, yT, xM, yM, dV;
  flt	trs = (flt) trans;
  flt	tr2 = (flt)(trans*trans);
  flt	w0  = (flt)(1.0 - trs*2.0 + tr2);
  flt	w1  = (flt)((trs - tr2)*2.0);
  flt	w2  = tr2;
  flt	wT  = w0 + w1 + w2;
  w0 /= wT; w1 /= wT; w2 /= wT;
  dV  = w0 + w1 + w2;
#if 1	/**************************************/
  dV -= 1.0;
  if (dV < 0.0)  dV = -dV;
  assert(dV < 0.001);
#else	/**************************************/
  if (trans < 0.5)  w1 = 2.0*trans,     w0 = 1.0 - w1, w2 = 0.0;
  else              w2 = 2.0*trans-1.0, w1 = 1.0 - w2, w0 = 0.0;
#endif	/**************************************/
  rowZeroF(dst,0,0,dW,dH);
  rowZeroF(wts,0,0,dW,dH);
  for (j = 0; j < dH; j++) {
    pB  = *qxyB++ ; pC = *qxyC++ ;
    p0  = *pre0++ ; p1 = *pre1++ ; p2 = *pre2++;
    for (k = 0; k < dW; k++) {
      xT = k + trs * *pB++ + tr2 * *pC++;
      kT = IFLOOR(xT);
      if (0 <= kT && kT < wM) {
        yT = j + trs * *pB++ + tr2 * *pC++;
        jT = IFLOOR(yT);
        if (0 <= jT && jT < hM) {
          xT -= kT;
          xM  = 1.0F - xT;
          yT -= jT;
          yM  = 1.0F - yT;
          dV = w0**p0++ + w1**p1++ + w2**p2++;

          wT           = xM*yM;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;

          kT++;
          wT           = xT*yM;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;

          jT++;
          wT           = xT*yT;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;

          kT--;
          wT           = xM*yT;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;
        }
        else { p0++; p1++; p2++; }
      }
      else { p0++; p1++; p2++; pB++; pC++; }
    }
  }
}


void
fxyQuadFwdSpatBiq ( ppf dst, ppf wts, ppf qxyB, ppf qxyC
                  , ppf pre0, ppf pre1, ppf pre2, unt dW, unt dH, dbl trans)
{
  unt   j, k;
  int   kT, jT, wM = dW-1, hM = dH-1;
  pfl   pB, pC, p0, p1, p2;
  flt	xT, yT, dV, f0,f1,f2, g0,g1,g2;
  flt	trs = (flt) trans;
  flt	tr2 = (flt)(trans*trans);
  flt	w0  = (flt)(1.0 - trans*(2.0 - trans));
  flt	w1  = (flt)(trans*((1.0 - trans)*2.0));
  flt	w2  = tr2;
  flt	wT  = w0 + w1 + w2;
  w0 /= wT; w1 /= wT; w2 /= wT;
  dV  = w0 + w1 + w2;
#if 1	/**************************************/
  dV -= 1.0;
  if (dV < 0.0)  dV = -dV;
  assert(dV < 0.001);
#else	/**************************************/
  if (trans < 0.5)  w1 = 2.0*trans,     w0 = 1.0 - w1, w2 = 0.0;
  else              w2 = 2.0*trans-1.0, w1 = 1.0 - w2, w0 = 0.0;
#endif	/**************************************/
  rowZeroF(dst,0,0,dW,dH);
  rowZeroF(wts,0,0,dW,dH);
  for (j = 0; j < dH; j++) {
    pB  = *qxyB++ ; pC = *qxyC++ ;
    p0  = *pre0++ ; p1 = *pre1++ ; p2 = *pre2++;
    for (k = 0; k < dW; k++) {
      xT = k + trs * *pB++ + tr2 * *pC++;
      kT = IRINTF(xT);
      if (0 < kT && kT < wM) {
        yT = j + trs * *pB++ + tr2 * *pC++;
        jT = IRINTF(yT);
        if (0 < jT && jT < hM) {

          xT -= kT;
          wT  = ABS(xT);
          f1  = (1.0F - wT)*(1.0F + wT);
          wT *=  0.5F;
          f0  = (1.0F - xT)*wT;
          f2  = (1.0F + xT)*wT;
                               /* Which are better optimized: f's or g's? */
          yT -= jT;
          wT  = ABS(yT);
          g0  = (1.0F - yT);
          g2  = (1.0F + yT);
          g1  = g2*g0;
          wT *= 0.5F;
          g0 *= wT;
          g2 *= wT;

          dV = w0**p0++ + w1**p1++ + w2**p2++;

          wT           = f1*g1;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;

          kT++;
          wT           = f2*g1;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;

          jT++;
          wT           = f2*g2;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;

          kT--;
          wT           = f1*g2;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;

          kT--;
          wT           = f0*g2;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;

          jT--;
          wT           = f0*g1;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;

          jT--;
          wT           = f0*g0;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;

          kT++;
          wT           = f1*g0;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;
 
          kT++;
          wT           = f2*g0;
          wts[jT][kT] += wT;
          dst[jT][kT] += wT*dV;
        }
        else { p0++; p1++; p2++; }
      }
      else { p0++; p1++; p2++; pB++; pC++; }
    }
  }
}


void
fxyQuadFwdWarpNen ( ppf dst, ppf qxyB, ppf qxyC, ppf pre0, ppf pre1, ppf pre2
                  , unt dW, unt dH, dbl trans)
{
  unt   j, k;
  int   inx, iny, iW = dW, iH = dH;
  pfl   pB, pC, p0, p1, p2;
  flt	del;
  flt	trs = (flt)trans;
  flt	tr2 = trs*trs;
  flt	w0 = 1.0F - trs*2.0F + tr2;
  flt	w1 = (trs - tr2)*2.0F;
  flt	w2 = tr2;

  for (j = 0; j < dH; j++) {
    pB  = *qxyB++ ; pC = *qxyC++ ;
    p0  = *pre0++ ; p1 = *pre1++ ; p2 = *pre2++;
    for (k = 0; k < dW; k++) {
      del = trs * *pB++ + tr2 * *pC++;
      inx = k + IRINTF(del);
      if (0 <= inx && inx < iW) {
        del = trs * *pB++ + tr2 * *pC++;
        iny = j + IRINTF(del);
        if (0 <= iny && iny < iH)
          dst[iny][inx] = w0**p0++ + w1**p1++ + w2**p2++;
      }
    }
  }
}


void
fxyQuadFwdWarp ( ppf xyB, ppf xyC, ppf fx1, ppf fy1, ppf fx2, ppf fy2
               , unt dW, unt dH, int xS, int yS, int xE, int yE)
{
  unt   j, k;
  int   inx, iny;
  pfl   dB, sx1;
  pfl   dC, sy1;
  flt	dx1, dy1, dx2, dy2;

  for (j = 0; j < dH; j++) {
    dB  = *xyB++ ; dC  = *xyC++;
    sx1 = *fx1++ ; sy1 = *fy1++;

    for (k = 0; k < dW; k++) {
      dx1 = *sx1++;
      dy1 = *sy1++;
      inx = k + IRINTF(dx1);	/* nearest neighbor coords for 2nd vct-fld */
      if      (inx < xS)  inx = 0 ;
      else if (inx > xE)  inx = xE;
      iny = j + IRINTF(dy1);
      if      (iny < yS)  iny = 0 ;
      else if (iny > yE)  iny = yE;

      dx2 = fx2[iny][inx];	/* X2 = X" - X'	*/
      dy2 = fy2[iny][inx];
      
      *dB++ = dx1*3 - dx2;	/* Bx	*/
      *dB++ = dy1*3 - dy2;	/* By	*/
      *dC++ = (dx2 - dx1)*2;	/* Cx	*/
      *dC++ = (dy2 - dy1)*2;	/* Cy	*/
    }
  }
}







/* For fx2 = flow(x0,x2) instead of fx2 = flow(x1,x2), etc.  Not optimized. */
void
fxyQuadFwdIso ( ppf xyB, ppf xyC, ppf fx1, ppf fy1, ppf fx2, ppf fy2
              , int dX, int dY, unt dW, unt dH, int sX, int sY)
{
  unt   k;
  pfl   dB, sx1, sx2;
  pfl   dC, sy1, sy2;

  while (dH--) {
    dB  = *xyB++;
    dC  = *xyC++;
    sx1 = *fx1++; sy1 = *fy1++;
    sx2 = *fx2++; sy2 = *fy2++;

    for (k = 0; k < dW; k++) {
      *dB++ = 4**sx1   - *sx2 - 3*k;		/* Bx	*/
      *dB++ = 4**sy1   - *sy2 - 3*k;		/* By	*/
      *dC++ = ( *sx2++ - 2**sx1++ + k)*2;	/* Cx	*/
      *dC++ = ( *sy2++ - 2**sy1++ + k)*2;	/* Cy	*/
    }
  }
}

