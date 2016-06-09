/*****************************************************************************
 rowSmat.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.30-10.20
 Bicubic, Bilinear, Biquadratic, Nearest Neighbor=Singleton (no interp. at all)
 *****************************************************************************/
#ifndef rowSmat_h__
#define rowSmat_h__

void rowWarpAff(ppu dst, ppu src, pDmj dD, pDmj sD, AffTrf *aT);

void
rgbSmatBil8(ppf dsT, ppf dwT, ppf sRGB[], ppf vOX[], ppf vOY[]
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, flt cfrac[], dbl trs);

void
rgbSmatBil4(ppf dsT, ppf dwT, ppf sRGB[], ppf vOX[], ppf vOY[]
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, flt cfrac[], flt trs);

void
rgbSmatAffBil3
(ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf srcC
    , ppu inpA[4], ppu inpB[4], ppu inpC[4]
    , ppf vATX, ppf vATY, ppf vABX, ppf vABY, ppf vACX, ppf vACY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH
    , int iX, int iY, unt iW, unt iH
    , dbl trs, dbl clr, flt *cfr
    , AffTrf *afA, AffTrf *afB, AffTrf *afC
    , LblFace *sLblA, LblFace *sLblB, LblFace *sLblC
);

void
rgbSmatAffBilSym2
(ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppu inpA[4], ppu inpB[4]
    , ppf vATX, ppf vATY, ppf vABX, ppf vABY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH
    , int iX, int iY, unt iW, unt iH
    , flt trs, flt clr
    , AffTrf *afA
    , AffTrf *afB
    , LblFace *sLblA
    , LblFace *sLblB
);


void
rgbSmatAffBil2T
(ppf dstT, ppf dstW, ppf srcA, ppf srcB
    , ppu inpU[][4], ppf vATX, ppf vATY, ppf vABX, ppf vABY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH
    , int iX, int iY, unt iW, unt iH
    , dbl trs, dbl clr
    , AffTrf *aT
    , LblFace *sLbl
);


void
rgbSmatAffBil2Nxt
(ppf dstT, ppf dstW, ppf srcA, ppf srcB
    , ppu inpU[][4]
    , ppf fwdX, ppf fwdY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH
    , int iX, int iY, unt iW, unt iH
    , dbl trs, dbl clr
    , AffTrf *aT
    , LblFace *sLbl
);


void
rgbSmatAffQuadBil3
(ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf srcC
    , ppu srcU[][4], ppu inpU[][4]
    , ppf vATX, ppf vATY, ppf vABX, ppf vABY, ppf vACX, ppf vACY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH
    , int iX, int iY, unt iW, unt iH
    , flt trs, flt *cfrac
    , AffTrf *aT
    , LblFace *dLbl
    , LblFace *sLbl
);


void
ixyFwd2Trip
(ppf dst, ppf wts, ppf dsp, ppf wtp, ppf srcA, ppf srcB
    , ppf foX, ppf foY, ppf raX, ppf raY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH
    , unl method, dbl trans, dbl cfrac, dbl infimum, dbl initval, dbl over
    , flt trip[][12], int numTri, unt flags
);

void
rgbSmatWtsBil2(ppf dsT, ppf dwT, ppf srcO, ppf srcM
    , ppf fwdX, ppf fwdY, ppf difW
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr);
void
rgbSmatQuadBil2(ppf dsT, ppf dwT, ppf srcO, ppf srcP
    , ppf vOPX, ppf vOPY, ppf vOTX, ppf vOTY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr);

void
rgbSmatQuadBil3(ppf dsT, ppf dwT, ppf srcO, ppf srcL, ppf srcR
    , ppf vOLX, ppf vOLY, ppf vORX, ppf vORY, ppf vOTX, ppf vOTY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs, flt *cfrac);

typedef void Smat1(ppf dst, ppf wts, ppf src, ppf fwdX, ppf fwdY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs);
extern Smat1 rowSmatBil, rowSmatBiq, rowSmatSin, rowSmatCls, rowSmatTrn,
rgbSmatBil, rgbSmatBiq, rgbSmatSin, rgbSmatCls, rgbSmatTrn;


typedef void Smat2(ppf dstT, ppf dstW, ppf srcA, ppf srcB, ppf fwdX, ppf fwdY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs, dbl clr);
extern Smat2 rowSmatBil2, rowSmatBiq2, rowSmatSin2,
rgbSmatBil2, rgbSmatBiq2, rgbSmatSin2;


void
rgbSmatBil3(ppf dsT, ppf dwT, ppf srcA, ppf srcB, ppf srcC
    , ppf *vOX, ppf *vOY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH, dbl trs, flt *cfrac, dbl inf);


typedef void Smat3(ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH
    , flt clrs[4], dbl trs, dbl infimum);
extern Smat3 rowSmatBil3, rowSmatBiq3, rowSmatSin3,
rgbSmatBiq3, rgbSmatSin3;

typedef void Smat4(ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH
    , flt clrs[5], dbl trs, dbl infimum);
extern Smat4 rowSmatBil4, rowSmatBiq4, rowSmatSin4,
rgbSmatBiq4, rgbSmatSin4;


typedef void SmatN(ppf dstT, ppf dstW, ppf *srcS, ppf *fvX, ppf *fvY
    , int dX, int dY, unt dW, unt dH
    , int sX, int sY, unt sW, unt sH
    , int num, flt clrs[4], dbl trs, dbl infimum);
extern SmatN rowSmatBilN, rowSmatBiqN, rowSmatSinN,
rgbSmatBilN, rgbSmatBiqN, rgbSmatSinN;

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


#endif	/* rowSmat_h__ */


