/*****************************************************************************
 rowSmatBiq.c    S.R.Lines     93.7.15-8.29
 FUNCS: rowSmatBil, rowSmatBiq
 Preserves the sign convention of fwdX.
 NB: Hits out of bounds or right on the edge yield no contribution;
 rely on fill to clean up other's boundary messes.  There is actually
 a discrapancy in bilinear routings between 0 and size-1 borders, i.e.,
 top and left borders take some hits that right and bottom borders exclude

 Boundary handling here is a half-n-half kludge to compensate for Sarnoff's
 infelicities.  Should demand that every be aligned and carry row pointers
 pointing to true leading edges, with real sizes (instead of borders).
 *****************************************************************************/
#define 	 NDEBUG 1
#include	<assert.h>
#include	<un/unBug.h>
#include	<un/unMath.h>
#include	<un/unTypes.h>
#include	<m/rowZero.h>
#include	<m/mj.h>
#include	<m/rowSmat.h>

/*____________________________________________________________________________*
  Assumes dims(dstT) == dims(dstW) == dims(invX) 
  and     dims(srcA) == dims(fwdX) == dims(fwdY)
 *____________________________________________________________________________*/

void rowSmatBiq ( ppf dstT, ppf dstW, ppf srcA, ppf fwdX, ppf fwdY
                , int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, dbl trs)
{
  int dx1 = dX + dW - 1;
  int dy1 = dY + dH - 1;
  int sx1 = sX + sW;
  int sy1 = sY + sH;
  int jA, kA;
  pfl pX, pY, pS;
  flt xT, yT, f0, f1, f2, g0, g1, g2;
  dbl fac = trs;
  register int kT, jT;
  register flt wT, vA;

  rowZeroF(dstT,dX,dY,dW,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (jA = sY, srcA += jA, fwdX += jA, fwdY += jA; jA < sy1; jA++) {
    kA = sX, pS = *srcA++ + kA, pX = *fwdX++ + kA, pY = *fwdY++ + kA;
    for (; kA < sx1; pS++, pX++, kA++) {

      yT = (flt)(jA - fac * *pY++);		/* Only here, for single pxl. smat, does fac makes sense */
      jT = IRINTF(yT);

      if (dY < jT && jT < dy1) {	/* @ NB: direct hits on borders are lost! */
      
        xT = (flt)(kA - fac * *pX);
        kT = IRINTF(xT);

        if (dX < kT && kT < dx1) {

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

          vA  = *pS;

          wT            = f1*g1;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;
 
          kT++;
          wT            = f2*g1;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;

          jT++;
          wT            = f2*g2;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;

          kT--;
          wT            = f1*g2;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;

          kT--;
          wT            = f0*g2;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;
  
          jT--;
          wT            = f0*g1;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;
 
          jT--;
          wT            = f0*g0;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;

          kT++;
          wT            = f1*g0;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;
  
          kT++;
          wT            = f2*g0;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;
        }  
      }
    }
  }
}




void rowSmatBiq2 ( ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf fwdX, ppf fwdY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr)
{
  int dx1 = dX + dW - 1;
  int dy1 = dY + dH - 1;
  int sx1 = sX + sW - 1;	/* For other src val. interp. from non-int coords. */
  int sy1 = sY + sH - 1;
  int jA, kA, jB, kB, scanB = srcB[1] - srcB[0] - 1;
  pfl pX, pY, pA, pB;
  flt xT, yT, xB, yB, f0, f1, f2, g0, g1, g2, fac = (flt)trs;//FIXME
  dbl cB = clr, cA = 1.0 - cB;
  register int kT, jT;
  register flt wT, vA;

  rowZeroF(dstT,dX,dY,dW,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (jA = sY, srcA += jA, fwdX += jA, fwdY += jA; jA <= sy1; jA++) {
    for (kA = sX, pA = *srcA++ + kA, pX = *fwdX++ + kA, pY = *fwdY++ + kA; kA <= sx1; pA++, pX++, kA++) {

      yB = -*pY++;	/* sarnoff's sign convention is unfortunate here */
      yT = yB*fac + jA;
      jT = IRINTF(yT);

      if (dY < jT && jT < dy1) {
      
        xB = -*pX;
        xT = xB*fac + kA;
        kT = IRINTF(xT);

        if (dX < kT && kT < dx1) {

          /* @@ The following extraction of a pixval from srcB
                is bilinear, not biquadratic */

          xB += kA;
          kB  = IFLOOR(xB);
          yB += jA;		/* i.e., yB = jA - 1.0 * *pY */
          jB  = IFLOOR(yB);

          /* NB: src pixloc [jB][kB] is NOT always inbounds. */
          if (sX <= kB && kB < sx1 && sY <= jB && jB < sy1) {

            yB -= jB;
            xB -= kB;

#if 1       /* Assumes that all rows in srcB are equally separated (tho not nec. contig.): */
            pB  =  srcB[jB]  + kB;
            wT  =  *pB++;
            wT += (*pB - wT) * xB;
            pB +=  scanB;
            vA  =  *pB++;
            vA += (*pB - vA) * xB;
            wT += ( vA - wT) * yB;
#else
            wT  =  srcB[jB  ][kB  ];
            wT += (srcB[jB  ][kB+1] - wT) * xB;
            vA  = (srcB[++jB][kB  ];
            vA += (srcB[jB  ][kB+1] - vA) * xB;
            wT += ( vA - wT) * yB;
#endif
            assert(-0.5F < wT && wT < 255.5F);
            vA = (flt)(wT*cB + *pA*cA);
          }
          else
            vA  = *pA;


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

          wT            = f1*g1;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;
 
          kT++;
          wT            = f2*g1;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;

          jT++;
          wT            = f2*g2;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;

          kT--;
          wT            = f1*g2;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;

          kT--;
          wT            = f0*g2;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;
  
          jT--;
          wT            = f0*g1;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;
 
          jT--;
          wT            = f0*g0;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;

          kT++;
          wT            = f1*g0;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;
  
          kT++;
          wT            = f2*g0;
          dstW[jT][kT] += wT   ;
          dstT[jT][kT] += wT*vA;
        }  
      }
    }
  }
}


/* The following is probably broken! @@@@@@@@@@@ */


void rowSmatBiq3 ( ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH
                 , flt clrs[4], dbl trs, dbl infimum)
{
  int	dx1 = dX + dW - 1;
  int	dy1 = dY + dH - 1;
  int	sy1 = sY + sH - 1;
  int	sx1 = sX + sW - 1;
  ppf	XvA = fvX[0] + sY, YvA = fvY[0] + sY, srcA = srcS[0] + sY;
  ppf	XvB = fvX[1] + sY, YvB = fvY[1] + sY, srcB = srcS[1] + sY;
  ppf	XvC = fvX[2] + sY, YvC = fvY[2] + sY, srcC = srcS[2] + sY;
  pfl	XpA, YpA, XpB, YpB, XpC, YpC, pM;
  int	jA, kA,  jT, kT, scanM = srcB[1] - srcB[0] - 1;
  flt	f0,f1,f2, g0,g1,g2;
  flt	cA = clrs[0], cB = clrs[1], cC = clrs[2];
  flt	fac = (flt) trs, inf = (flt) infimum;
  flt	    yM, xM, vT, wT, xT, yT;
  rgst	int jM, kM;
  rgst  flt v1, v2;

  rowZeroF(dstT,dX,dY,dW,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (jA = sY; jA <= sy1; jA++) {

    kA  =  sX;
    XpA = *XvA++ + kA, YpA = *YvA++ + kA;
    XpB = *XvB++ + kA, YpB = *YvB++ + kA;
    XpC = *XvC++ + kA, YpC = *YvC++ + kA;

    for ( ; kA <= sx1; XpA++, XpB++, YpB++, XpC++, YpC++, kA++) {

      yT  = jA - fac * *YpA++;		/* NB: expect trs == 1.0 */
      jT  = IFLOOR(yT);
      if (dY < jT && jT < dy1) {	/* was dY <= jT, a bug, but now border hits are lost */
        xT  = kA - fac * *XpA;		/* Target y value is within dst area */
        kT  = IFLOOR(xT);
        if (dX < kT && kT < dx1) {	/* (was dX<=kT) Target x value is also within dst area */

          wT  = cA;
          vT  = cA*srcA[jA][kA];

          /* @@ The following extractions of a pixvals from src B & C
                are bilinear, not biquadratic */
                
          xM  = kA - *XpB;		/* NB (@@): srcB(yM,xM) will be same every time! */ 
          kM  = IFLOOR(xM);
          if (sX <= kM && kM < sx1) {
            yM  = jA - *YpB;
            jM  = IFLOOR(yM);
            if (sY <= jM && jM < sy1) {	/*  srcB(jM,kM) is in bounds */
              xM -= kM;
              yM -= jM;
              /* Assuming all rows in srcB, etc, are separated/spaced equally: */
              pM  =  srcB[jM]  + kM;
              v1  =  *pM++;
              v1 += (*pM - v1) * xM;
              pM +=  scanM;
              v2  =  *pM++;
              v2 += (*pM - v2) * xM;
              v1 += ( v2 - v1) * yM;
              wT += cB;
              vT += cB*v1;
            }
          }

          xM  = kA - *XpC;
          kM  = IFLOOR(xM);
          if (sX <= kM && kM < sx1) {
            yM  = jA - *YpC;
            jM  = IFLOOR(yM);
            if (sY <= jM && jM < sy1) {	/*  srcC(jM,kM) is in bounds */
              xM -= kM;
              yM -= jM;
              pM  =  srcC[jM]  + kM;
              v1  =  *pM++;
              v1 += (*pM - v1) * xM;
              pM +=  scanM;
              v2  =  *pM++;
              v2 += (*pM - v2) * xM;
              v1 += ( v2 - v1) * yM;
              wT += cC;
              vT += cC*v1;
            }
          }

          if (wT > inf) {


            /* Now (vT/wT) would yield weighted mean of interpolated values,
               but this is postponed til after distributing it over a 3x3 pxl block: */
            
            xT -= kT;
            v1  = ABS(xT);
            f1  = (1.0F - v1)*(1.0F + v1);
            v1 *=  0.5F;
            f0  = (1.0F - xT)*v1;
            f2  = (1.0F + xT)*v1;
	  			/* Which are better optimized: f's or g's? */
            yT -= jT;
            v1  = ABS(yT);
            g0  = (1.0F - yT);
            g2  = (1.0F + yT);
            g1  = g2*g0;
            v1 *= 0.5F;
            g0 *= v1;
            g2 *= v1;

            v1            = wT; 	/* NB: expect wT to be 1.0 == sum(clrs) */
            
            v2            = v1*f1*g1;
            dstW[jT][kT] += v2;
            dstT[jT][kT] += v2*vT;
 
            kT++;
            v2            = v1*f2*g1;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            jT++;
            v2            = v1*f2*g2;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            kT--;
            v2            = v1*f1*g2;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            kT--;
            v2            = v1*f0*g2;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;
  
            jT--;
            v2            = v1*f0*g1;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;
 
            jT--;
            v2            = v1*f0*g0;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            kT++;
            v2            = v1*f1*g0;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;
  
            kT++;
            v2            = v1*f2*g0;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

          }
        }
      }
    }
  } 
}


void rowSmatBiq4 ( ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH
                 , flt clrs[4], dbl trs, dbl infimum)
{
  int	dx1 = dX + dW - 1;
  int	dy1 = dY + dH - 1;
  int	sy1 = sY + sH - 1;
  int	sx1 = sX + sW - 1;
  ppf	XvA = fvX[0] + sY, YvA = fvY[0] + sY, srcA = srcS[0] + sY;
  ppf	XvB = fvX[1] + sY, YvB = fvY[1] + sY, srcB = srcS[1] + sY;
  ppf	XvC = fvX[2] + sY, YvC = fvY[2] + sY, srcC = srcS[2] + sY;
  ppf	XvD = fvX[3] + sY, YvD = fvY[3] + sY, srcD = srcS[3] + sY;
  pfl	XpA, YpA, XpB, YpB, XpC, YpC, XpD, YpD, pM;
  int	jA, kA,  jT, kT, scanM = srcB[1] - srcB[0] - 1;
  flt	f0,f1,f2, g0,g1,g2;
  flt	cA = clrs[0], cB = clrs[1], cC = clrs[2], cD = clrs[3];
  flt	fac = (flt) trs, inf = (flt) infimum;
  flt 	yM, xM, vT, wT, xT, yT;
  rgst	int jM, kM;
  rgst  flt v1, v2;

  rowZeroF(dstT,dX,dY,dW,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (jA = sY; jA <= sy1; jA++) {

    kA  =  sX;
    XpA = *XvA++ + kA, YpA = *YvA++ + kA;
    XpB = *XvB++ + kA, YpB = *YvB++ + kA;
    XpC = *XvC++ + kA, YpC = *YvC++ + kA;
    XpD = *XvD++ + kA, YpD = *YvD++ + kA;

    for ( ; kA <= sx1; XpA++, XpB++, YpB++, XpC++, YpC++, XpD++, YpD++, kA++) {

      yT  = jA - fac * *YpA++;		/* NB: expect trs == 1.0 */
      jT  = IFLOOR(yT);
      if (dY < jT && jT < dy1) {	/* was dY <= jT, a bug, but now border hits are lost */
        xT  = kA - fac * *XpA;		/* Target y value is within dst area */
        kT  = IFLOOR(xT);
        if (dX < kT && kT < dx1) {	/* (was dX<=kT) Target x value is also within dst area */

          wT  = cA;
          vT  = cA*srcA[jA][kA];

          /* @@ The following extractions of a pixvals from src B,C, & D
                are bilinear, not biquadratic */
                
#define EXTRACT_BIL(wT,vT,xM,yM,kM,jM,scanM,pvX,pvY,srcM,clrM)	\
          xM  = kA - *pvX;	/* NB (@@): srcM(yM,xM) will be same every frame/call! */\
          kM  = IFLOOR(xM);\
          if (sX <= kM && kM < sx1) {\
            yM  = jA - *pvY;\
            jM  = IFLOOR(yM);\
            if (sY <= jM && jM < sy1) {           /*  srcM(jM,kM) is in bounds */\
              xM -= kM;\
              yM -= jM;\
              /* Assuming all rows in srcM, etc, are separated/spaced equally: */\
              pM  =  srcM[jM]  + kM;\
              v1  =  *pM++;\
              v1 += (*pM - v1) * xM;\
              pM +=  scanM;\
              v2  =  *pM++;\
              v2 += (*pM - v2) * xM;\
              v1 += ( v2 - v1) * yM;\
              wT += clrM;\
              vT += clrM*v1;\
            }\
          }

          EXTRACT_BIL(wT,vT,xM,yM,kM,jM,scanM,XpB,YpB,srcB,cB)
          EXTRACT_BIL(wT,vT,xM,yM,kM,jM,scanM,XpC,YpC,srcC,cC)
          EXTRACT_BIL(wT,vT,xM,yM,kM,jM,scanM,XpD,YpD,srcD,cD)

          if (wT > inf) {

            /* Now (vT/wT) would yield weighted mean of interpolated values,
               but this is postponed til after distributing it over a 3x3 pxl block: */
            
            xT -= kT;
            v1  = ABS(xT);
            f1  = (1.0F - v1)*(1.0F + v1);
            v1 *=  0.5F;
            f0  = (1.0F - xT)*v1;
            f2  = (1.0F + xT)*v1;
	  			/* Which are better optimized: f's or g's? */
            yT -= jT;
            v1  = ABS(yT);
            g0  = (1.0F - yT);
            g2  = (1.0F + yT);
            g1  = g2*g0;
            v1 *= 0.5F;
            g0 *= v1;
            g2 *= v1;

            v1            = wT; 	/* NB: expect wT to be 1.0 == sum(clrs) */
            
            v2            = v1*f1*g1;
            dstW[jT][kT] += v2;
            dstT[jT][kT] += v2*vT;
 
            kT++;
            v2            = v1*f2*g1;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            jT++;
            v2            = v1*f2*g2;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            kT--;
            v2            = v1*f1*g2;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            kT--;
            v2            = v1*f0*g2;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;
  
            jT--;
            v2            = v1*f0*g1;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;
 
            jT--;
            v2            = v1*f0*g0;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            kT++;
            v2            = v1*f1*g0;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;
  
            kT++;
            v2            = v1*f2*g0;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

          }
        }
      }
    }
  } 
}



#define 	MAX_NUM 9	/* move */

void rowSmatBiqN ( ppf dstT, ppf dstW, ppf *srcS, ppf *fwdX, ppf *fwdY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH
                 , int num, flt *clrs, dbl trs, dbl infimum)
{
  int	dx1 = dX + dW - 1;
  int	dy1 = dY + dH - 1;
  int	sy1 = sY + sH - 1;
  int	sx1 = sX + sW - 1;
  ppf	XvA = fwdX[0] + sY, YvA = fwdY[0] + sY, srcA = srcS[0] + sY;

  pfl	XpA, YpA, Xp[MAX_NUM], Yp[MAX_NUM], pM;
  int	jA, kA,  jT, kT, mm, numM = num-1, scanM = srcA[1] - srcA[0] - 1;
  flt	f0,f1,f2, g0,g1,g2;
  flt	cA = clrs[0];
  flt	fac = (flt) trs;	/* NB: expect fac == 1.0, at least as of 93.9.12 */
  flt	inf = (flt) infimum;
  flt	yM, xM, vT, wT, xT, yT;
  rgst	int jM, kM;
  rgst  flt v1, v2;

  if (num > MAX_NUM)
    die("rowSmatBiqN: parameter num > MAX_NUM (src images)");
  
  rowZeroF(dstT,dX,dY,dW,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  jA = sY, mm = numM;
  do {
    fwdX[mm] += jA,
    fwdY[mm] += jA,
    srcS[mm] += jA;
  } while (mm--);			/* Include [0] */

  for ( ; jA <= sy1; jA++) {

    kA  =  sX, mm = numM;
    do {
      Xp[mm] = *(fwdX[mm])++ + kA,
      Yp[mm] = *(fwdY[mm])++ + kA;
    } while (--mm);			/* Exclude [0] */
    XpA = *XvA++ + kA, YpA = *YvA++ + kA;
    
    for ( ; kA <= sx1; kA++) {

      yT  = jA - fac * *YpA++;		/* NB: expect trs == 1.0 */
      jT  = IFLOOR(yT);
      if (dY < jT && jT < dy1) {	/* was dY <= jT, a bug, but now border hits are lost */
        xT  = kA - fac * *XpA;		/* Target y value is within dst area */
        kT  = IFLOOR(xT);
        if (dX < kT && kT < dx1) {	/* was dX <= kT, a bug, but now border hits are lost */
					/* Target x value is also within dst area */
          wT  = cA;
          vT  = cA*srcA[jA][kA];

          /* @@ The following extractions of a pixvals are bilinear, not biquadratic */
          
          mm = numM;
          do {				/* while (--mm);  (Exclude [0]) */
            xM  = kA - *(Xp[mm])++;	/* NB: srcB(yM,xM) is independent of fac */ 
            kM  = IFLOOR(xM);
            if (sX <= kM && kM < sx1) {
              yM  = jA - *(Yp[mm])++;
              jM  = IFLOOR(yM);
              if (sY <= jM && jM < sy1) {	/*  src[mm](jM,kM) is in bounds */
                xM -= kM;
                yM -= jM;
                /* Assuming all rows in src[mm], etc, are separated/spaced equally: */
                pM  =  srcS[mm][jM] + kM;
                v1  =  *pM++;
                v1 += (*pM - v1) * xM;
                pM +=  scanM;
                v2  =  *pM++;
                v2 += (*pM - v2) * xM;
                v1 += ( v2 - v1) * yM;
                wT += clrs[mm];
                vT += clrs[mm]*v1;
              }
            }
            else (Yp[mm])++;
          }  while (--mm);		/* Exlude [0] */
          

          if (wT > inf) {


            /* Now (vT/wT) would yield weighted mean of interpolated values,
               but this is postponed til after distributing it over a 3x3 pxl block: */
            
            xT -= kT;
            v1  = ABS(xT);
            f1  = (1.0F - v1)*(1.0F + v1);
            v1 *=  0.5F;
            f0  = (1.0F - xT)*v1;
            f2  = (1.0F + xT)*v1;
	  			/* Which are better optimized: f's or g's? */
            yT -= jT;
            v1  = ABS(yT);
            g0  = (1.0F - yT);
            g2  = (1.0F + yT);
            g1  = g2*g0;
            v1 *= 0.5F;
            g0 *= v1;
            g2 *= v1;

            v1            = wT; 	/* NB: expect wT to be 1.0 == sum(clrs) */
            
            v2            = v1*f1*g1;
            dstW[jT][kT] += v2;
            dstT[jT][kT] += v2*vT;
 
            kT++;
            v2            = v1*f2*g1;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            jT++;
            v2            = v1*f2*g2;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            kT--;
            v2            = v1*f1*g2;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            kT--;
            v2            = v1*f0*g2;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;
  
            jT--;
            v2            = v1*f0*g1;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;
 
            jT--;
            v2            = v1*f0*g0;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

            kT++;
            v2            = v1*f1*g0;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;
  
            kT++;
            v2            = v1*f2*g0;
            dstW[jT][kT] += v2   ;
            dstT[jT][kT] += v2*vT;

          }
        }
      }
    }
  } 
}
