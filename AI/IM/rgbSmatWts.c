/*****************************************************************************
 rgbSmatWts.c    S.R.Lines     93.5.15-9.16

#include	<stdlib.h>
 *****************************************************************************/
#define 	 NDEBUG 1
#include	<assert.h>
#ifdef          __GNUC__
#include        <memory.h>      /* memset */
#else
#include        <string.h>      /* memset */
#endif
#include	<un/unBug.h>
#include	<un/unMath.h>
#include	<un/unTypes.h>
#include	<un/unFlags.h>
#include	<m/rowZero.h>
#include	<m/mj.h>
#include	<m/rowSmat.h>
/*___________________________________________________________________________*
 *___________________________________________________________________________*/

void
rgbSmatWtsBil2 	( ppf dsT, ppf dwT, ppf srcO, ppf srcM
		, ppf fwdX, ppf fwdY, ppf difW
		, int dX, int dY, unt dW, unt dH
		, int sX, int sY, unt sW, unt sH, dbl trs, dbl clr)
{
int	dx1 = dX + dW - 1;
int	dy1 = dY + dH - 1;
int	sx1 = sX + sW - 1;
int	sy1 = sY + sH - 1;
int	jO, kO, kM, jM, scanM = srcM[1] - srcM[0] - 2;
pfl	pX, pY, pM, pW;
flt	xT, yT, xM, yM, cM = (flt)clr, cO = (flt)(1.0F - cM);
int	jT, kT, kR, kG, kB;
flt	vR, vG, vB, wO, wM;
rgs flt	wR, wG, wB;

  memset((pvd)(dsT[dY]+3*dX), 0, sizeof(flt)*dW*dH*3);
  memset((pvd)(dwT[dY] + dX), 0, sizeof(flt)*dW*dH);
  
  for (jO = sY, fwdX += jO, fwdY += jO, difW += jO; jO <= sy1; jO++) {
    pX = *fwdX++ + sX, pY = *fwdY++ + sX, pW = *difW++ + sX;
    for (kO = sX; kO <= sx1; pX++, pW++, kO++) {

      yM = *pY++;
      yT = (flt)(jO - trs * yM);
      jT = IFLOOR(yT);

      if (dY <= jT && jT < dy1) {

        xM = *pX;
        xT = (flt)(kO - trs * xM);
        kT = IFLOOR(xT);

        if (dX <= kT && kT < dx1) {
       
          yM  = jO  -  yM;
          jM  = IFLOOR(yM);
          xM  = kO  -  xM;
          kM  = IFLOOR(xM);
          kR  = kO * 3;

          /* NB: (kM,jM) is NOT always inbounds. */
          if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {

            yM -= jM;
            xM -= kM;

            /* Assuming all rows in srcM are equally separated: */
            pM  =   srcM[jM]  + kM*3;
            wR  =  *pM++;
            wG  =  *pM++;
            wB  =  *pM++;
            wR += (*pM++ - wR) * xM;
            wG += (*pM++ - wG) * xM;
            wB += (*pM   - wB) * xM;
            pM +=  scanM;
            vR  =  *pM++;
            vG  =  *pM++;
            vB  =  *pM++;
            vR += (*pM++ - vR) * xM;
            vG += (*pM++ - vG) * xM;
            vB += (*pM   - vB) * xM;
            wR += ( vR   - wR) * yM;
            wG += ( vG   - wG) * yM;
            wB += ( vB   - wB) * yM;

#if 1
            wM  = *pW;    wO = 1.0F - wM;    /* replacing cM = clr, cO = 1-clr */
            vR  = wR*wM + wO*srcO[jO][kR++];
            vG  = wG*wM + wO*srcO[jO][kR++];
            vB  = wB*wM + wO*srcO[jO][kR  ];
#else
            vR  = wR*cM + cO*srcO[jO][kR++];
            vG  = wG*cM + cO*srcO[jO][kR++];
            vB  = wB*cM + cO*srcO[jO][kR  ];
#endif

            assert(-0.5 < vR && vR < 255.5);	/* Normally #define NDEBUG */
            assert(-0.5 < vG && vG < 255.5);
            assert(-0.5 < vB && vB < 255.5);
          }
          else {
            vR = srcO[jO][kR++];
            vG = srcO[jO][kR++];
            vB = srcO[jO][kR  ];
          }
          
          xT  -= kT;
          xM   = 1.0F - xT;
          yT  -= jT;
          yM   = 1.0F - yT;

          kR   = kT * 3;
          kG   = kR + 1;
          kB   = kG + 1;
          wR   = xM * yM;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * vR;
          dsT[jT][kG] += wR * vG;
          dsT[jT][kB] += wR * vB;
          
          jT++;
          wR           = xM * yT;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * vR;
          dsT[jT][kG] += wR * vG;
          dsT[jT][kB] += wR * vB;

          kT  ++  ;
          kR  += 3;
          kG   = kR + 1;
          kB   = kG + 1;
          wR   = xT * yT;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * vR;
          dsT[jT][kG] += wR * vG;
          dsT[jT][kB] += wR * vB;

          jT--;
          wR           = xT * yM;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * vR;
          dsT[jT][kG] += wR * vG;
          dsT[jT][kB] += wR * vB;
        }
      }
    }
  }
}



