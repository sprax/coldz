/*****************************************************************************
 rowWarpFlt.c    S.R.Lines     93.5.15-9.15
 NOTES: if trs < TEENY, just rowCopy src to dst?
 *****************************************************************************/
#define 	 NDEBUG 1	/* Assertions only fail on bad input */
#include	<assert.h>
#include	<un/unBug.h>
#include	<un/unMath.h>
#include	<un/unTypes.h>
#include	"rowWarp.h"

#undef	IRINTF		/* Return int nearest to float argument: want fast! */
#define IRINTF(f)  	((int)((f) < 0.0 ? ((f)-0.5F) : ((f)+0.5F)))

void rgbWarpBilFlt(ppf dst, ppf src, ppf vX, ppf vY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs)
{
    int 	j, k, k3, scanM = src[1] - src[0] - 5;
    int 	dx1 = dX + dW, eX = sX + sW - 2;
    int 	dy1 = dY + dH, eY = sY + sH - 2;
    flt 	*pX, *pY, fx, fy, bR, bG, bB;
    rgs int	ix, iy;
    rgs flt	*pxp, aR, aG, aB;

    for (j = dY, vX += j, vY += j; j < dy1; j++) {
        for (k = dX, k3 = k * 3, pX = *vX++ + k, pY = *vY++ + k; k < dx1; k++) {

            fy = (flt)(j - trs * *pY++);
            iy = IFLOOR(fy);
            if (iy > eY)  iy = eY, fy = 1.0F;	/* bot edge pxl(s) only */
            else if (iy < sY)  iy = sY, fy = 0.0F;	/* top edge pxl(s) only */
            else                        fy -= iy;

            fx = (flt)(k - trs * *pX++);
            ix = IFLOOR(fx);
            if (ix > eX)  ix = eX, fx = 1.0F;	/* right edge pxl(s) only */
            else if (ix < sX)  ix = sX, fx = 0.0F;	/*  left edge pxl(s) only */
            else                        fx -= ix;

            pxp = src[iy] + ix * 3;
            aR = *pxp++;
            aG = *pxp++;
            aB = *pxp++;
            aR += (*pxp++ - aR) * fx;
            aG += (*pxp++ - aG) * fx;
            aB += (*pxp - aB) * fx;
            pxp += scanM;
            bR = *pxp++;
            bG = *pxp++;
            bB = *pxp++;
            bR += (*pxp++ - bR) * fx;
            bG += (*pxp++ - bG) * fx;
            bB += (*pxp - bB) * fx;
            aR += (bR - aR) * fy;
            aG += (bG - aG) * fy;
            aB += (bB - aB) * fy;
            assert(-0.4999 < aR && aR < 255.4999);
            assert(-0.4999 < aG && aG < 255.4999);
            assert(-0.4999 < aB && aB < 255.4999);
            dst[j][k3++] = aR;
            dst[j][k3++] = aG;
            dst[j][k3++] = aB;
        }
    }
}

/*___________________________________________________________________________*
  Must have dims(dst) <= dims(vX) == dims(vY), and borders of srcs may be
  positive or negative (dims(src) <> dims(dst)), but they must be known
  explicitly.  In general, dims(dst) < dms(src) for best results.
  All all must be aligned (i.e., [0][0] must refer to same "place")
  --this makes the algorithm and subimage processing simple.

  Destination Boundaries:
  dY is the top, or coord of the first row, dWd is the rectangle's width, etc.
  Source Boundaries:
  sX is the coord of leftmost stored pxl, and eX is the rightmost pxl from
  which to anchor an interpolation, etc. -- for bilinear interpolation,
  think of sX as 0 and eX as wide-2.
 *___________________________________________________________________________*/
void rowWarpBilFlt(pfl *dst, pfl *src, pfl *vX, pfl *vY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs)
{
    int	j, k, scanM = src[1] - src[0] - 1;
    int	dx1 = dX + dW, eX = sX + sW - 2;
    int	dy1 = dY + dH, eY = sY + sH - 2;
    float	*pX, *pY, fx, fy;
    register float *pxp;
    register int	ix, iy;
    register float pixv1, pixv2;

    for (j = dY, vX += j, vY += j; j < dy1; j++) {
        for (k = dX, pX = *vX++ + k, pY = *vY++ + k; k < dx1; k++) {

            fy = (flt)(j - trs * *pY++);
            iy = IFLOOR(fy);
            if (iy > eY)  iy = eY, fy = 1.0F;	/* bot edge pxl(s) only */
            else if (iy < sY)  iy = sY, fy = 0.0F;	/* top edge pxl(s) only */
            else                          fy -= iy;

            fx = (flt)(k - trs * *pX++);
            ix = IFLOOR(fx);
            if (ix > eX)  ix = eX, fx = 1.0F;	/* right edge pxl(s) only */
            else if (ix < sX)  ix = sX, fx = 0.0F;	/*  left edge pxl(s) only */
            else                          fx -= ix;

            pxp = src[iy] + ix;

#if 1	/* This should run the fastest:  */
            pixv1 = *pxp++;
            pixv1 += (*pxp - pixv1) * fx;
            pxp += scanM;
            pixv2 = *pxp++;
            pixv2 += (*pxp - pixv2) * fx;
            pixv1 += (pixv2 - pixv1) * fy;
#else	/* This may run almost as fast:  */
            pixv1 = *(pxp + 1)*fx + (1.0 - fx)**pxp;
            pxp += scan;
            pixv2 = *(pxp + 1)*fx + (1.0 - fx)**pxp;
            pixv1 += (pixv2 - pixv1) * fy;
#endif
            assert(-0.4999 < pixv1 && pixv1 < 255.4999);
            dst[j][k] = pixv1;
        }
    }
}


/*___________________________________________________________________________*
 rowWarpBiqFlt
 Optimization of code by Rajesh Hingorani, RCA David Sarnoff Research Center.
 Vector flow field warping with bicubic or "Langrangian" interpolation of
 pixvals from non-integer coord. locations.
 *___________________________________________________________________________*/

void rowWarpBiqFlt(pfl *dst, pfl *src, pfl *vX, pfl *vY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs)
{
    int	j, k;
    int	dx1 = dX + dW, eX = sX + sW - 2;
    int	dy1 = dY + dH, eY = sY + sH - 2;
    flt      *pX, *pY;
    float    fx, fy, pixv0, pixv1, pixv2;
    register float xLfac, xCfac, xRfac;
    register int	xL, xC, xR, yy;

    sX++, sY++;			/* To give 1 pxl brdr on top & left sides */
    for (j = dY, vX += j, vY += j; j < dy1; j++) {
        for (k = dX, pX = *vX++ + k, pY = *vY++ + k; k < dx1; k++) {
            fx = (flt)(k - trs * *pX++);
            xC = IRINTF(fx);
            if (xC > eX)  xC = eX, fx = 1.0F;
            else if (xC < sX)  xC = sX, fx = -1.0F;
            else                          fx -= xC;
            xCfac = (fx - 1.0F) * (fx + 1.0F);
            xLfac = 0.5F  * fx * (fx - 1.0F);
            xRfac = 0.5F  * fx * (fx + 1.0F);
            xL = xC - 1;
            xR = xC + 1;

            fy = (flt)(j - trs * *pY++);
            yy = IRINTF(fy);
            if (yy > eY)  yy = eY, fy = 1.0F;
            else if (yy < sY)  yy = sY, fy = -1.0F;
            else                          fy -= yy;
            yy--;
            pixv0 = xLfac*src[yy][xL] - xCfac*src[yy][xC] + xRfac*src[yy][xR];
            yy++;
            pixv1 = xLfac*src[yy][xL] - xCfac*src[yy][xC] + xRfac*src[yy][xR];
            yy++;
            pixv2 = xLfac*src[yy][xL] - xCfac*src[yy][xC] + xRfac*src[yy][xR];
            dst[j][k] = 0.5F * fy * ((fy - 1.0F)*pixv0 + (fy + 1.0F)*pixv2)
                - (fy + 1.0F)*(fy - 1.0F)*pixv1;
            assert(-0.4999 < dst[j][k] && dst[j][k] < 255.4999);
        }
    }
}


void rowWarpBiqAffFlt(ppf dst, ppf src, ppf AA, pfl ss
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH)
{ /* A,s are the 2D affine trans from out to in (!) */
    int	j, k;
    int	dx1 = dX + dW, eX = sX + sW - 2;
    int	dy1 = dY + dH, eY = sY + sH - 2;
    float    fx, fy, pixv0, pixv1, pixv2;
    register float xLfac, xCfac, xRfac;
    register int	xL, xC, xR, yy;

    sX++, sY++;		/* To give 1 pxl brdr on top & left sides */
    for (j = dY; j < dy1; j++) {
        for (k = dX; k < dx1; k++) {
            fx = (flt)(AA[1][1] * k + AA[1][2] * j + ss[1]);
            xC = NINTF(fx);
            if (xC > eX)  xC = eX, fx = 1.0F;
            else if (xC < sX)  xC = sX, fx = -1.0F;
            else                          fx -= xC;
            xCfac = (fx - 1.0F) * (fx + 1.0F);
            xLfac = 0.5F  * fx * (fx - 1.0F);
            xRfac = 0.5F  * fx * (fx + 1.0F);
            xL = xC - 1;
            xR = xC + 1;

            fy = AA[2][1] * k + AA[2][2] * j + ss[2];
            yy = NINTF(fy);
            if (yy > eY)  yy = eY, fy = 1.0F;
            else if (yy < sY)  yy = sY, fy = -1.0F;
            else                          fy -= yy;
            yy--;
            pixv0 = xLfac*src[yy][xL] - xCfac*src[yy][xC] + xRfac*src[yy][xR];
            yy++;
            pixv1 = xLfac*src[yy][xL] - xCfac*src[yy][xC] + xRfac*src[yy][xR];
            yy++;
            pixv2 = xLfac*src[yy][xL] - xCfac*src[yy][xC] + xRfac*src[yy][xR];
            dst[j][k] = 0.5F * fy * ((fy - 1.0F)*pixv0 + (fy + 1.0F)*pixv2)
                - (fy + 1.0F)*(fy - 1.0F)*pixv1;
            assert(-0.4999 < dst[j][k] && dst[j][k] < 255.4999);
        }
    }
}

void rowWarpBiqProjFlt(ppf dst, ppf src, ppf AA
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH)
{ /* matrix A[1..3,1..3] is a projective trans of the plane from dstloc to srcloc */
    int	j, k;
    int	dx1 = dX + dW, eX = sX + sW - 2;
    int	dy1 = dY + dH, eY = sY + sH - 2;
    float    ft, fx, fy, pixv0, pixv1, pixv2;
    register float xLfac, xCfac, xRfac;
    register int	xL, xC, xR, yy;

    sX++, sY++;		/* To give 1 pxl brdr on top & left sides */
    for (j = dY; j < dy1; j++) {
        for (k = dX; k < dx1; k++) {
            ft = (flt)(AA[3][1] * k + AA[3][2] * j + AA[3][3] * 100.0F);
            fx = (flt)(((AA[1][1] * k + AA[1][2] * j + AA[1][3] * 100.0) / ft)*100.0F);
            xC = NINTF(fx);
            if (xC > eX)  xC = eX, fx = 1.0F;
            else if (xC < sX)  xC = sX, fx = -1.0F;
            else                          fx -= xC;
            xCfac = (fx - 1.0F) * (fx + 1.0F);
            xLfac = 0.5F  * fx * (fx - 1.0F);
            xRfac = 0.5F  * fx * (fx + 1.0F);
            xL = xC - 1;
            xR = xC + 1;

            fy = (flt)(((AA[2][1] * k + AA[2][2] * j + AA[2][3] * 100.0F) / ft)*100.0F);
            yy = NINTF(fy);
            if (yy > eY)  yy = eY, fy = 1.0F;
            else if (yy < sY)  yy = sY, fy = -1.0F;
            else                          fy -= yy;
            yy--;
            pixv0 = xLfac*src[yy][xL] - xCfac*src[yy][xC] + xRfac*src[yy][xR];
            yy++;
            pixv1 = xLfac*src[yy][xL] - xCfac*src[yy][xC] + xRfac*src[yy][xR];
            yy++;
            pixv2 = xLfac*src[yy][xL] - xCfac*src[yy][xC] + xRfac*src[yy][xR];
            dst[j][k] = 0.5F * fy * ((fy - 1.0F)*pixv0 + (fy + 1.0F)*pixv2)
                - (fy + 1.0F)*(fy - 1.0F)*pixv1;
            assert(-0.4999 < dst[j][k] && dst[j][k] < 255.4999);
        }
    }
}

/*___________________________________________________________________________*
 rowWarpSinFlt, rowWarpTrnFlt
 Single pixval, rounded pixlocs -- visible quantization errors.
 Single pixval, truncated pixlocs -- even worse.
 Collisions and holes abound, and out-of-bounds is mapped on border to see it.
 Use bilinear or bicubic interpolation instead.
*___________________________________________________________________________*/

void rowWarpSinFlt(pfl *dst, pfl *src, pfl *vX, pfl *vY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs)
{
    int	j, k;
    int	dx1 = dX + dW, eX = sX + sW - 1;
    int	dy1 = dY + dH, eY = sY + sH - 1;
    float *pX, *pY;
    float  fx, fy;
    dbl  fact = trs;
    register int ix, iy;

    for (j = dY, vX += j, vY += j; j < dy1; j++) {
        for (k = dX, pX = *vX++ + k, pY = *vY++ + k; k < dx1; k++) {
            fy = (flt)(fact * *pY++);
            iy = j - IRINTF(fy);
            if (iy > eY)  iy = eY;
            else if (iy < sY)  iy = sY;
            fx = (flt)(fact * *pX++);
            ix = k - IRINTF(fx);
            if (ix > eX)  ix = eX;
            else if (ix < sX)  ix = sX;
            dst[j][k] = src[iy][ix];	/* Single pixval: no clip, round, etc. */
        }
    }
}


void rowWarpTrnFlt(pfl *dpp, pfl *spp, pfl *vX, pfl *vY
    , unt wide, unt tall, double trs)
{
    unt j, k;
    float *pX, *pY;
    register unt ix, iy;
    register float fx, fy;
    register double fact = trs;

    --wide, --tall;
    for (j = 0; j <= tall; j++) {
        for (pX = *vX++, pY = *vY++, k = 0; k <= wide; k++) {
            fy = (float)(fact * *pY++);
            iy = (int)(j - ITRUNC(fy));
            if (iy > tall)
                iy = tall;
            else if (iy < 0)
                iy = 0;
            fx = (flt)(fact * *pX++);
            fx = k - ITRUNC(fx);
            if (ix > wide)
                ix = wide;
            else if (ix < 0)
                ix = 0;
            dpp[j][k] = spp[iy][ix];
        }
    }
}
