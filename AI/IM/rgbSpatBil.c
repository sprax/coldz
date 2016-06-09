/******************************************************************************
 rgbSpatBil.c    S.R.Lines     93.10.28
 Like rowSmatBil.c, except it's assumed the src's are prewarped (to srcA)

 BIG PROBLEM: Seems that jT=IFLOOR(yT) caused jump between 0th & 1st frames,
 while jT=IRINTF(yT) causes jump between last & ntl frames.  Probably both
 also cause jumps in between.

#include	<stdlib.h>
 *****************************************************************************/
#define 	 NDEBUG 1
#include	<assert.h>
#include	<un/unBug.h>
#include	<un/unMath.h>
#include	<un/unTypes.h>
#include	<m/rowZero.h>
#include	<m/rowSpat.h>

void rgbSpatBil2 ( ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf vABX, ppf vABY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr)
{
int 	dx1 = dX + dW - 1;
int 	dy1 = dY + dH - 1;
int 	sy1 = sY + sH - 1;
int 	sx1 = sX + sW - 1;
pfl 	pABX, pABY, dpT, dpW, spA, spB;
int 	jA, kA,  jT, kT, k3, sX3 = sX*3;
dbl 	cB = clr, cA = 1.0 - clr;
flt 	fac = (flt) trs;
flt 	yM, xM, vR, vG, vB, xT, yT;
rgs flt	v1;

  rowZeroF(dstT,dX*3,dY,dW*3,dH);
  rowZeroF(dstW,dX,  dY,dW,  dH);

  srcA += sY, srcB += sY;
  vABX += sY, vABY += sY;

  for (jA = sY; jA <= sy1; jA++) {

    pABX = *vABX++ + sX , pABY = *vABY++ + sX ;
    spA  = *srcA++ + sX3, spB  = *srcB++ + sX3;

    for (kA = sX ; kA <= sx1; pABX++, kA++) {
      yT  = jA - fac * *pABY++;		/* NB: expect trs == 1.0 */
      jT  = IFLOOR(yT);
      if (dY <= jT && jT < dy1) {	/* Target y value is within dst area */
        xT  = kA - fac * *pABX;
        kT  = IFLOOR(xT);
        if (dX <= kT && kT < dx1) {	/* Target x value is also within dst area */
          vR  = (flt)(cA**spA++ + cB**spB++);
          vG  = (flt)(cA**spA++ + cB**spB++);
          vB  = (flt)(cA**spA++ + cB**spB++);
          xT -=        kT;	/* ie: xT = (kA-fac**pABX) - floor(kA-fac**pABX); */
          xM  = 1.0F - xT;
          yT -=        jT;
          yM  = 1.0F - yT;
          k3  = kT * 3;
          dpT = dstT[jT];
          dpW = dstW[jT];
          v1              = xM * yM;
          dpW[kT++] += v1     ;
          dpT[k3++] += v1 * vR;
          dpT[k3++] += v1 * vG;
          dpT[k3++] += v1 * vB;
          v1              = xT * yM;
          dpW[kT  ] += v1     ;
          dpT[k3++] += v1 * vR;
          dpT[k3++] += v1 * vG;
          dpT[k3  ] += v1 * vB;
          jT++;
          v1              = xT * yT;
          dpW[kT--] += v1     ;
          dpT[k3--] += v1 * vB;
          dpT[k3--] += v1 * vG;
          dpT[k3--] += v1 * vR;
          v1              = xM * yT;
          dpW[kT  ] += v1     ;
          dpT[k3--] += v1 * vB;
          dpT[k3--] += v1 * vG;
          dpT[k3  ] += v1 * vR;
        }
        else { spA += 3,  spB += 3; }
      }
      else { spA += 3,  spB += 3; }
    }
  } 
}



void rgbSpatBil8 ( ppf dstT, ppf dstW, ppf *srcS, ppf *vfoX, ppf *vfoY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, flt clrs[8], dbl trs)
{
int 	dx1 = dX + dW - 1;
int 	dy1 = dY + dH - 1;
int 	sy1 = sY + sH - 1;
int 	sx1 = sX + sW - 1;
ppf 	vOTX = vfoX[0] + sY, vOTY = vfoY[0] + sY;
ppf	src0 = srcS[0] + sY, src1 = srcS[1] + sY, src2 = srcS[2] + sY, src3 = srcS[3] + sY;
ppf	src4 = srcS[4] + sY, src5 = srcS[5] + sY, src6 = srcS[6] + sY, src7 = srcS[7] + sY;
pfl 	dpT, dpW, sp0, sp1, sp2, sp3, sp4, sp5, sp6, sp7;
pfl 	pOTX, pOTY;
int 	j0, k0,  jT, kT, k3, sX3 = sX*3;
flt 	c0 = clrs[0], c1 = clrs[1], c2 = clrs[2], c3 = clrs[3];
flt 	c4 = clrs[4], c5 = clrs[5], c6 = clrs[6], c7 = clrs[7];
flt 	fac = (flt) trs;
flt 	yM, xM, vR, vG, vB, xT, yT;
rgs flt	v1;

  rowZeroF(dstT,dX*3,dY,dW*3,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (j0 = sY; j0 <= sy1; j0++) {

    pOTX = *vOTX++ + sX , pOTY = *vOTY++ + sX ;
    sp0  = *src0++ + sX3, sp1  = *src1++ + sX3, sp2  = *src2++ + sX3, sp3  = *src3++ + sX3;
    sp4  = *src4++ + sX3, sp5  = *src5++ + sX3, sp6  = *src6++ + sX3, sp7  = *src7++ + sX3;

    for (k0 = sX ; k0 <= sx1; pOTX++, k0++) {

      yT  = j0 - fac * *pOTY++;	
      jT  = IFLOOR(yT);
      if (dY <= jT && jT < dy1) {	/* Target y value is within dst area */
        xT  = k0 - fac * *pOTX;
        kT  = IFLOOR(xT);
        if (dX <= kT && kT < dx1) {	/* Target x value is also within dst area */

          vR  = c0**sp0++ + c1**sp1++ + c2**sp2++ + c3**sp3++ + c4**sp4++ + c5**sp5++ + c6**sp6++ + c7**sp7++;
          vG  = c0**sp0++ + c1**sp1++ + c2**sp2++ + c3**sp3++ + c4**sp4++ + c5**sp5++ + c6**sp6++ + c7**sp7++;
          vB  = c0**sp0++ + c1**sp1++ + c2**sp2++ + c3**sp3++ + c4**sp4++ + c5**sp5++ + c6**sp6++ + c7**sp7++;

          xT -=        kT;	/* ie: xT = (k0-fac**pOTX) - floor(k0-fac**pOTX); */
          xM  = 1.0F - xT;
          yT -=        jT;
          yM  = 1.0F - yT;

          k3 = kT * 3;
          dpT = dstT[jT];
          dpW = dstW[jT];

          v1              = xM * yM;
          dpW[kT++] += v1     ;
          dpT[k3++] += v1 * vR;
          dpT[k3++] += v1 * vG;
          dpT[k3++] += v1 * vB;

          v1              = xT * yM;
          dpW[kT  ] += v1     ;
          dpT[k3++] += v1 * vR;
          dpT[k3++] += v1 * vG;
          dpT[k3  ] += v1 * vB;

          jT++;
          v1              = xT * yT;
          dpW[kT--] += v1     ;
          dpT[k3--] += v1 * vB;
          dpT[k3--] += v1 * vG;
          dpT[k3--] += v1 * vR;

          v1              = xM * yT;
          dpW[kT  ] += v1     ;
          dpT[k3--] += v1 * vB;
          dpT[k3--] += v1 * vG;
          dpT[k3  ] += v1 * vR;
        }
        else {
          sp0 += 3,  sp1 += 3,  sp2 +=3,  sp3 += 3, sp4 += 3,  sp5 += 3,  sp6 +=3,  sp7 += 3;
        }
      }
      else {
        sp0 += 3,  sp1 += 3,  sp2 +=3,  sp3 += 3, sp4 += 3,  sp5 += 3,  sp6 +=3,  sp7 += 3;
      }
    }
  } 
}

void rgbSpatBil4 ( ppf dstT, ppf dstW, ppf *srcS, ppf *vfoX, ppf *vfoY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, flt clrs[4], dbl trs)
{
int 	dx1 = dX + dW - 1;
int 	dy1 = dY + dH - 1;
int 	sy1 = sY + sH - 1;
int 	sx1 = sX + sW - 1;
ppf 	vOTX = vfoX[0] + sY, vOTY = vfoY[0] + sY;
ppf	srcA = srcS[0] + sY, srcB = srcS[1] + sY;
ppf 	srcC = srcS[2] + sY, srcD = srcS[3] + sY;
pfl 	dpT, dpW, spA, spB, spC, spD;
pfl 	pOTX, pOTY;
int 	jA, kA,  jT, kT, k3, sX3 = sX*3;
flt 	cA = clrs[0], cB = clrs[1], cC = clrs[2], cD = clrs[3];
flt 	fac = (flt) trs;
flt 	yM, xM, vR, vG, vB, xT, yT;
rgs flt	v1;

  rowZeroF(dstT,dX*3,dY,dW*3,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (jA = sY; jA <= sy1; jA++) {

    pOTX = *vOTX++ + sX , pOTY = *vOTY++ + sX ;
    spA  = *srcA++ + sX3, spB  = *srcB++ + sX3;
    spC  = *srcC++ + sX3, spD  = *srcD++ + sX3;

    for (kA = sX ; kA <= sx1; pOTX++, kA++) {

      yT  = jA - fac * *pOTY++;		/* NB: expect trs == 1.0 */
      jT  = IFLOOR(yT);
      if (dY <= jT && jT < dy1) {	/* Target y value is within dst area */
        xT  = kA - fac * *pOTX;
        kT  = IFLOOR(xT);
        if (dX <= kT && kT < dx1) {	/* Target x value is also within dst area */

          vR  = cA**spA++ + cB**spB++ + cC**spC++ + cD**spD++;
          vG  = cA**spA++ + cB**spB++ + cC**spC++ + cD**spD++;
          vB  = cA**spA++ + cB**spB++ + cC**spC++ + cD**spD++;

          xT -=        kT;	/* ie: xT = (kA-fac**pOTX) - floor(kA-fac**pOTX); */
          xM  = 1.0F - xT;
          yT -=        jT;
          yM  = 1.0F - yT;

          k3 = kT * 3;
          dpT = dstT[jT];
          dpW = dstW[jT];

          v1              = xM * yM;
          dpW[kT++] += v1     ;
          dpT[k3++] += v1 * vR;
          dpT[k3++] += v1 * vG;
          dpT[k3++] += v1 * vB;

          v1              = xT * yM;
          dpW[kT  ] += v1     ;
          dpT[k3++] += v1 * vR;
          dpT[k3++] += v1 * vG;
          dpT[k3  ] += v1 * vB;

          jT++;
          v1              = xT * yT;
          dpW[kT--] += v1     ;
          dpT[k3--] += v1 * vB;
          dpT[k3--] += v1 * vG;
          dpT[k3--] += v1 * vR;

          v1              = xM * yT;
          dpW[kT  ] += v1     ;
          dpT[k3--] += v1 * vB;
          dpT[k3--] += v1 * vG;
          dpT[k3  ] += v1 * vR;
        }
        else {
          spA += 3,  spB += 3,  spC +=3,  spD += 3;
        }
      }
      else {
        spA += 3,  spB += 3,  spC +=3,  spD += 3;
      }
    }
  } 
}


void rgbSpatBil3 ( ppf dstT, ppf dstW, ppf *srcS, ppf *vfoX, ppf *vfoY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, flt clrs[4], dbl trs)
{
int 	dx1 = dX + dW - 1;
int 	dy1 = dY + dH - 1;
int 	sy1 = sY + sH - 1;
int 	sx1 = sX + sW - 1;
ppf 	vOTX = vfoX[0] + sY, vOTY = vfoY[0] + sY;
ppf	srcA = srcS[0] + sY, srcB = srcS[1] + sY, srcC = srcS[2] + sY;
pfl 	dpT, dpW, spA, spB, spC;
pfl 	pOTX, pOTY;
int 	jA, kA,  jT, kT, k3, sX3 = sX*3;
flt 	cA = clrs[0], cB = clrs[1], cC = clrs[2];
flt 	fac = (flt) trs;
flt 	yM, xM, vR, vG, vB, xT, yT;
rgs flt	v1;

  rowZeroF(dstT,dX*3,dY,dW*3,dH);
  rowZeroF(dstW,dX,dY,dW,dH);

  for (jA = sY; jA <= sy1; jA++) {

    pOTX = *vOTX++ + sX , pOTY = *vOTY++ + sX ;
    spA  = *srcA++ + sX3, spB  = *srcB++ + sX3;
    spC  = *srcC++ + sX3;

    for (kA = sX ; kA <= sx1; pOTX++, kA++) {

      yT  = jA - fac * *pOTY++;		/* NB: expect trs == 1.0 */
      jT  = IFLOOR(yT);
      if (dY <= jT && jT < dy1) {	/* Target y value is within dst area */
        xT  = kA - fac * *pOTX;
        kT  = IFLOOR(xT);
        if (dX <= kT && kT < dx1) {	/* Target x value is also within dst area */

          vR  = cA**spA++ + cB**spB++ + cC**spC++;
          vG  = cA**spA++ + cB**spB++ + cC**spC++;
          vB  = cA**spA++ + cB**spB++ + cC**spC++;

          xT -=        kT;	/* ie: xT = (kA-fac**pOTX) - floor(kA-fac**pOTX); */
          xM  = 1.0F - xT;
          yT -=        jT;
          yM  = 1.0F - yT;

          k3 = kT * 3;
          dpT = dstT[jT];
          dpW = dstW[jT];

          v1              = xM * yM;
          dpW[kT++] += v1     ;
          dpT[k3++] += v1 * vR;
          dpT[k3++] += v1 * vG;
          dpT[k3++] += v1 * vB;

          v1              = xT * yM;
          dpW[kT  ] += v1     ;
          dpT[k3++] += v1 * vR;
          dpT[k3++] += v1 * vG;
          dpT[k3  ] += v1 * vB;

          jT++;
          v1              = xT * yT;
          dpW[kT--] += v1     ;
          dpT[k3--] += v1 * vB;
          dpT[k3--] += v1 * vG;
          dpT[k3--] += v1 * vR;

          v1              = xM * yT;
          dpW[kT  ] += v1     ;
          dpT[k3--] += v1 * vB;
          dpT[k3--] += v1 * vG;
          dpT[k3  ] += v1 * vR;
        }
        else { spA += 3,  spB += 3,  spC +=3; }
      }
      else { spA += 3,  spB += 3,  spC +=3; }
    }
  } 
}




