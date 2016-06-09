/*****************************************************************************
 rowSpat.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.30-10.20
 Bicubic, Bilinear, Biquadratic, Singleton (no interp. at all).
 *****************************************************************************/
#ifndef rowSpat_h__
#define rowSpat_h__

void rgbSpatBil8 ( ppf dstT, ppf dstW, ppf *srcS, ppf *vfoX, ppf *vfoY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, flt clrs[8], dbl trs);

typedef void Spat1 ( ppf dst, ppf wts, ppf src, ppf fwdX, ppf fwdY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH, dbl trs);
extern Spat1 rowSpatBil, rowSpatBiq, rowSpatSin, rowSpatCls, rowSpatTrn,
             rgbSpatBil, rgbSpatBiq, rgbSpatSin, rgbSpatCls, rgbSpatTrn;


void rgbSpatBil2 ( ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf vABX, ppf vABY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr);

typedef void Spat2 ( ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf fwdX, ppf fwdY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr);
extern Spat2 rowSpatBil2, rowSpatBiq2, rowSpatSin2,
                          rgbSpatBiq2, rgbSpatSin2;

void rgbSpatBil3 ( ppf dstT, ppf dstW, ppf *srcS, ppf *vfoX, ppf *vfoY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, flt clrs[4], dbl trs);


typedef void Spat3 ( ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH
                   , flt clrs[4], dbl trs, dbl infimum);
extern Spat3 rowSpatBil3, rowSpatBiq3, rowSpatSin3, rgbSpatBiq3, rgbSpatSin3; 

void rgbSpatBil4 ( ppf dstT, ppf dstW, ppf *srcS, ppf *vfoX, ppf *vfoY
                 , int dX, int dY, unt dW, unt dH
                 , int sX, int sY, unt sW, unt sH, flt clrs[4], dbl trs);

typedef void Spat4 ( ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH, flt clrs[4], dbl trs, dbl infimum);
extern Spat4 rowSpatBil4, rowSpatBiq4, rowSpatSin4,
             rgbSpatBiq4, rgbSpatSin4;

typedef void SpatN ( ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
                   , int dX, int dY, unt dW, unt dH
                   , int sX, int sY, unt sW, unt sH
                   , int num, flt clrs[4], dbl trs, dbl infimum);
extern SpatN rowSpatBilN, rowSpatBiqN, rowSpatSinN,
             rgbSpatBilN, rgbSpatBiqN, rgbSpatSinN;

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


#endif	/* rowSpat_h__ */


