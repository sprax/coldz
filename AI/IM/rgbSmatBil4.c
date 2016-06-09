/*****************************************************************************
 rgbSmatBil4.c    S.R.Lines     93.5.15-9.16
 using the total weight method
 *****************************************************************************/
#define          NDEBUG 1
#include        <assert.h>
#ifdef          __GNUC__
#include        <memory.h>      /* memset */
#else
#include        <string.h>      /* memset */
#endif
#include        <math.h>
#include        <un/unBug.h>
#include        <un/unMath.h>
#include        <un/unTypes.h>
#include        <un/unFlags.h>
#include        <m/rowZero.h>
#include        <m/mj.h>
#include        <m/rowSmat.h>
#define SRC_A   1
#define SRC_B   2
#define SRC_C   4
#define SRC_D   8


void
rgbSmatBil4 ( ppf dsT, ppf dwT, ppf sRGB[], ppf vOX[], ppf vOY[]
            , int dX, int dY, unt dW, unt dH
            , int sX, int sY, unt sW, unt sH, flt cfrac[], flt trs)
{
rflt	wR, wG, wB;
int	dx1 = dX + dW - 1;
int	dy1 = dY + dH - 1;
int	sx1 = sX + sW - 1;
int	sy1 = sY + sH - 1;
ppf	srcA = sRGB[0], srcB = sRGB[1], srcC = sRGB[2], srcD = sRGB[3]; 
ppf     vOTX = vOX[0] + sY, vOBX = vOX[1] + sY, vOCX = vOX[2] + sY, vODX = vOX[3] + sY;
ppf     vOTY = vOY[0] + sY, vOBY = vOY[1] + sY, vOCY = vOY[2] + sY, vODY = vOY[3] + sY;
pfl	pOTX,pOTY,  pOBX,pOBY,  pOCX,pOCY,  pODX,pODY, pM, psA;
int	jT, kT, kR, kG, kB, jO, kO, kM, jM, scanM = srcB[1] - srcB[0] - 2;
unt 	son;
flt	redT, grnT, bluT, redA,grnA,bluA, redB,grnB,bluB, redC,grnC,bluC, redD,grnD,bluD;
flt	xT, yT, xM, yM, wT;
flt 	cA = cfrac[0], cB = cfrac[1], cC = cfrac[2], cD = cfrac[3];
flt     cABC = 1.0F/(cA + cB + cC);
flt     cABD = 1.0F/(cA + cB + cD);
flt     cACD = 1.0F/(cA + cC + cD);
flt     cAB  = 1.0F/(cA + cB);
flt     cAC  = 1.0F/(cA + cC);
flt     cAD  = 1.0F/(cA + cD);

  memset((pvd)(dsT[dY]+3*dX), 0, sizeof(flt)*dW*dH*3);
  memset((pvd)(dwT[dY] + dX), 0, sizeof(flt)*dW*dH);
  
  for (jO = sY; jO <= sy1; jO++) {

    pOTX = *vOTX++ + sX, pOTY = *vOTY++ + sX;
    pOBX = *vOBX++ + sX, pOBY = *vOBY++ + sX;
    pOCX = *vOCX++ + sX, pOCY = *vOCY++ + sX;
    pODX = *vODX++ + sX, pODY = *vODY++ + sX;

    for (kO = sX; kO <= sx1; pOTX++, pOBX++,pOBY++, pOCX++,pOCY++, pODX++,pODY++, kO++) {

      yT = (flt)(jO - trs * *pOTY++);
      jT = IFLOOR(yT);

      if (dY <= jT && jT < dy1) {

        xT = (flt)(kO - trs * *pOTX);
        kT = IFLOOR(xT);

        if (dX <= kT && kT < dx1) {
       
          yM   = jO  -  *pOBY;
          jM   = IFLOOR(yM);

          xM   = kO  -  *pOBX;
          kM   = IFLOOR(xM);

          kR   = kO * 3;
          psA  = srcA[jO];	/* @@ efficiency -- move !! */
          redA = psA[kR++];
          grnA = psA[kR++];
          bluA = psA[kR  ];    /* kR will be reset */
          son  = SRC_A;

          /* NB: (kM,jM) is NOT always inbounds. */
          if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {

            yM -= jM;
            xM -= kM;

            /* Assuming all rows in srcB are equally separated: */
            pM  =  srcB[jM]  + kM*3;
            wR  =  *pM++;
            wG  =  *pM++;
            wB  =  *pM++;
            wR += (*pM++ - wR) * xM;
            wG += (*pM++ - wG) * xM;
            wB += (*pM   - wB) * xM;
            pM +=  scanM;
            redT  =  *pM++;
            grnT  =  *pM++;
            bluT  =  *pM++;
            redT += (*pM++ - redT) * xM;
            grnT += (*pM++ - grnT) * xM;
            bluT += (*pM   - bluT) * xM;
            wR += ( redT   - wR) * yM;
            wG += ( grnT   - wG) * yM;
            wB += ( bluT   - wB) * yM;
            assert(-0.5 < wR && wR < 255.5);	/* Normally #define NDEBUG */
            assert(-0.5 < wG && wG < 255.5);
            assert(-0.5 < wB && wB < 255.5);
            redB  = wR;
            grnB  = wG;
            bluB  = wB;
            son  |= SRC_B;
          }

          yM  = jO  -  *pOCY;
          jM  = IFLOOR(yM);
  
          xM  = kO  -  *pOCX;
          kM  = IFLOOR(xM);

          /* NB: (kM,jM) is NOT always inbounds. */
          if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {
          
            yM -= jM;
            xM -= kM;

            /* Assuming all rows in srcB are equally separated: */
            pM  =  srcC[jM]  + kM*3;
            wR  =  *pM++;
            wG  =  *pM++;
            wB  =  *pM++;
            wR += (*pM++ - wR) * xM;
            wG += (*pM++ - wG) * xM;
            wB += (*pM   - wB) * xM;
            pM +=  scanM;
            redT  =  *pM++;
            grnT  =  *pM++;
            bluT  =  *pM++;
            redT += (*pM++ - redT) * xM;
            grnT += (*pM++ - grnT) * xM;
            bluT += (*pM   - bluT) * xM;
            wR += ( redT   - wR) * yM;
            wG += ( grnT   - wG) * yM;
            wB += ( bluT   - wB) * yM;
            assert(-0.5 < wR && wR < 255.5);    /* Normally #define NDEBUG */
            assert(-0.5 < wG && wG < 255.5);
            assert(-0.5 < wB && wB < 255.5);
            redC  = wR;
            grnC  = wG;
            bluC  = wB;
            son |= SRC_C;
          }
          
          yM  = jO  -  *pODY;
          jM  = IFLOOR(yM);
 
          xM  = kO  -  *pODX;
          kM  = IFLOOR(xM);

          /* NB: (kM,jM) is NOT always inbounds. */
          if (sX <= kM && kM < sx1 && sY <= jM && jM < sy1) {

            yM -= jM;
            xM -= kM;

            /* Assuming all rows in srcB are equally separated: */
            pM  =  srcD[jM]  + kM*3;
            wR  =  *pM++;
            wG  =  *pM++;
            wB  =  *pM++;
            wR += (*pM++ - wR) * xM;
            wG += (*pM++ - wG) * xM;
            wB += (*pM   - wB) * xM;
            pM +=  scanM;
            redT  =  *pM++;
            grnT  =  *pM++;
            bluT  =  *pM++;
            redT += (*pM++ - redT) * xM;
            grnT += (*pM++ - grnT) * xM;
            bluT += (*pM   - bluT) * xM;
            wR += ( redT   - wR) * yM;
            wG += ( grnT   - wG) * yM;
            wB += ( bluT   - wB) * yM;
            assert(-0.5 < wR && wR < 255.5);    /* Normally #define NDEBUG */
            assert(-0.5 < wG && wG < 255.5);
            assert(-0.5 < wB && wB < 255.5);
            redD  = wR;
            grnD  = wG;
            bluD  = wB;
            son |= SRC_D;
          }


          switch(son) {
            case (SRC_A|SRC_B|SRC_C|SRC_D):
                        redT = cA*redA + cB*redB + cC*redC + cD*redD;
                        grnT = cA*grnA + cB*grnB + cC*grnC + cD*grnD;
                        bluT = cA*bluA + cB*bluB + cC*bluC + cD*bluD;
                        break;
            case (SRC_A|SRC_B|SRC_C):
                        redT = (cA*redA + cB*redB + cC*redC)*cABC;
                        grnT = (cA*grnA + cB*grnB + cC*grnC)*cABC;
                        bluT = (cA*bluA + cB*bluB + cC*bluC)*cABC;
                        break;
            case (SRC_A|SRC_B|SRC_D):
                        redT = (cA*redA + cB*redB + cD*redD)*cABD;
                        grnT = (cA*grnA + cB*grnB + cD*grnD)*cABD;
                        bluT = (cA*bluA + cB*bluB + cD*bluD)*cABD;
                        break;
            case (SRC_A|SRC_C|SRC_D):
                        redT = (cA*redA + cC*redC + cD*redD)*cACD;
                        grnT = (cA*grnA + cC*grnC + cD*grnD)*cACD;
                        bluT = (cA*bluA + cC*bluC + cD*bluD)*cACD;
                        break;
            case (SRC_A|SRC_B):
                        redT = (cA*redA + cB*redB)*cAB;
                        grnT = (cA*grnA + cB*grnB)*cAB;
                        bluT = (cA*bluA + cB*bluB)*cAB;
                        break;
            case (SRC_A|SRC_C):
                        redT = (cA*redA + cC*redC)*cAC;
                        grnT = (cA*grnA + cC*grnC)*cAC;
                        bluT = (cA*bluA + cC*bluC)*cAC;
                        break;
            case (SRC_A|SRC_D):
                        redT = (cA*redA + cD*redD)*cAD;
                        grnT = (cA*grnA + cD*grnD)*cAD;
                        bluT = (cA*bluA + cD*bluD)*cAD;
                        break;
            case SRC_A: /* wT = MAX(inf,cA); redT = wT*redA, grnT = wT*grnA, bluT = wT*bluA;  break; */
                        redT = redA, grnT = grnA, bluT = bluA;  break;
            case SRC_B: /* wT = MAX(inf,cB); redT = wT*redB, grnT = wT*grnB, bluT = wT*bluB;  break; */
                        redT = redB, grnT = grnB, bluT = bluB;  break;
            case SRC_C: /* wT = MAX(inf,cC); redT = wT*redC, grnT = wT*grnC, bluT = wT*bluC;  break; */
                        redT = redC, grnT = grnC, bluT = bluC;  break;
            case SRC_D: /* wT = MAX(inf,cD); redT = wT*redD, grnT = wT*grnD, bluT = wT*bluD;  break; */
                        redT = redD, grnT = grnD, bluT = bluD;  break;
            case (SRC_B|SRC_C):
                        die("rgbSmatBil4: Only srcs B & C ?? How the hell did this happen?");
                        wT = cB + cC;
                        /* if (wT < inf) { wT = inf/wT; redT = wT*(cB*redB + cC*redC), ... */
                        redT = cB*redB + cC*redC;
                        grnT = cB*grnB + cC*grnC;
                        bluT = cB*bluB + cC*bluC;
                        break;
            default: die("rgbSmatBil4: the bad son has sinned (%d)", son);
          }

          xT  -= kT;
          xM   = 1.0F - xT;
          yT  -= jT;
          yM   = 1.0F - yT;

          kR   = kT * 3;
          kG   = kR + 1;
          kB   = kG + 1;
          wR           = xM * yM;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * redT;
          dsT[jT][kG] += wR * grnT;
          dsT[jT][kB] += wR * bluT;
          
          jT++;
          wR           = xM * yT;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * redT;
          dsT[jT][kG] += wR * grnT;
          dsT[jT][kB] += wR * bluT;

          kT  ++  ;
          kR  += 3;
          kG   = kR + 1;
          kB   = kG + 1;
          wR           = xT * yT;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * redT;
          dsT[jT][kG] += wR * grnT;
          dsT[jT][kB] += wR * bluT;

          jT--;
          wR           = xT * yM;
          dwT[jT][kT] += wR;
          dsT[jT][kR] += wR * redT;
          dsT[jT][kG] += wR * grnT;
          dsT[jT][kB] += wR * bluT;
        }
      }
    }
  }
}




