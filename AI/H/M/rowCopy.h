/*****************************************************************************
rowCopy.h    S.R.Lines (spraxlo@ai)    93.6.12-7.11
#include <unTypes.h>
*****************************************************************************/
#ifndef im_rowCopy_h_
#define im_rowCopy_h_

void
rgbCopyFU(ppu dst[4], ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY);


#define	ROWCOPYFLT(dst,src,wide,tall) \
                  { int t=tall;  void dp = (void**)dst, sp = (void**)src; \
                    while (t--) memcpy(*dp++,*sp++,sizeof(flt)*wide); }

void rowCopy(     void **dst, void **src, unt wide, unt tall, unt dmmss, unt smmss);
void rowCopyVoid( void **dst, void **src, unt wide, unt tall);

void rowCopyToUnc(ppu dst, void **src, unt dW, unt dH, int sX, int sY, unt srctype);
void rowCopyToFlt(ppf dst, void **src, unt dW, unt dH, int sX, int sY, unt srctype);

void rowCopyUnc(unc **dst, unc **src, unt wide, unt tall);
void rowCopyFlt(flt **dst, flt **src, unt wide, unt tall);

void rowCopyFU(ppu dst, ppf src, int dX, int dY, unt wide, unt tall, int sX, int sY);
void rowCopyUU(ppu dst, ppu src, int dX, int dY, unt wide, unt tall, int sX, int sY);
void rowCopyFFIn(ppf dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY);
void rowCopyFF(ppf dst, ppf src, int dX, int dY, unt wide, unt tall, int sX, int sY);
void rowCopyUF(ppf dst, ppu src, int dX, int dY, unt wide, unt tall, int sX, int sY);

void rowCopyUncToFlt (flt **dst, unc **src, unt wide, unt tall);
void rowCopyFltToUnc (unc **dst, flt **src, unt wide, unt tall);

#endif	/* im_rowCopy_h_ */


