/*****************************************************************************
mgCopy.h    S.R.Lines (spraxlo@ai)    93.6.12-7.11
#include <unTypes.h>
*****************************************************************************/
#ifndef mgCopy_h
#define mgCopy_h

void mgCopy(     void **dst, void **src, unt wide, unt tall, unt dmmss, unt smmss);
void mgCopyVoid( void **dst, void **src, unt wide, unt tall);

void mgCopyToUnc(ppu dst, void **src, unt dW, unt dH, int sX, int sY, unt srctype);
void mgCopyToFlt(ppf dst, void **src, unt dW, unt dH, int sX, int sY, unt srctype);

void mgCopyUnc(unc **dst, unc **src, unt wide, unt tall);
void mgCopyFlt(flt **dst, flt **src, unt wide, unt tall);

void mgCopyFUold(ppu dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY);

void mgCopyFU(ppu dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY);
void mgCopyUU(ppu dst, ppu src, int dX, int dY, unt dW, unt dH, int sX, int sY);
void mgCopyFF(ppf dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY);
void mgCopyUF(ppf dst, ppu src, int dX, int dY, unt dW, unt dH, int sX, int sY);

void mgCopyUncToFlt (flt **dst, unc **src, unt wide, unt tall);
void mgCopyFltToUnc (unc **dst, flt **src, unt wide, unt tall);

#endif	/* mgCopy_h */


