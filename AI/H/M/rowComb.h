/*****************************************************************************
rowComb.h    S.R.Lines (spraxlo@ai.mit.edu)    93.6.12-8.17-9.8
#include <unTypes.h>
*****************************************************************************/
#ifndef im_rowComb_h_
#define im_rowComb_h_

void
rowCombFlt4 (ppf dst, ppf src[4], int dX, int dY, unt dW, unt dH, flt wts[4]);

void
rowCombFlt8 (ppf dst, ppf src[8], int dX, int dY, unt dW, unt dH, flt wts[8]);

void rowComb8UncRGB (ppu dst[4], ppu src[8][4], int wide, int tall, rgst flt wts[8]);
/* was:
void rowComb8UncRGB (ppu dst[3],ppu src[8][3],int wide,int tall,rgst flt wts[8]); */


void rowComb8Unc (ppu dst, ppu src[8], int wide, int tall, float wts[8]);

void rowCombN ( ppf dsX[9], ppf dsY[9], unt dW, unt dH, flt wts[9], unt num );

void rowComb3 ( flt **dvX, flt **dvY, flt **svX[3], flt **svY[3]
              , unt sW, unt sH, flt wts[3]);


void rowComb4 ( flt **dvX, flt **dvY, flt **svX[4], flt **svY[4]
              , flt wts[4], unt wide, unt tall);


void rowComb2Unc (ppu dst, ppu srcA, ppu srcB, int wide,int tall, double ctrs);
void rowComb2Flt (ppf dst, ppf srcA, ppf srcB, int wide,int tall, double ctrs);
void rowComb2FltUnc (ppu dst,ppf srcA,ppf srcB,int wide,int tall, double ctrs);

void rowComb2UU ( ppu dstM, ppu srcA, ppu srcB, int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, double ctrs);
void rowComb2FU ( ppu dstM, ppf srcA, ppf srcB, int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, double ctrs);
void rowComb2FF ( ppf dstM, ppf srcA, ppf srcB, int dX, int dY, unt dW, unt dH
                , int sX, int sY, unt sW, unt sH, double ctrs);

#endif	/* im_rowComb_h_ */

