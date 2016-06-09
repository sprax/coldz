/*****************************************************************************
rowMult.h    S.R.Lines (spraxlo@ai)    93.7.27
Pixelwise multiplication in place of an image stored as an array of
pointers to rows.
Trc truncates instead of rounds (unsigned char only).
*****************************************************************************/
#ifndef im_rowMult_h_
#define im_rowMult_h_

/* #include <unTypes.h> */

void rowMult    (void *dst, void *src, int X, int Y, unt dW, unt dH, unt mmss, double factor);
void rowMultFlt (flt **dst, flt **src, int X, int Y, unt dW, unt dH, double factor);
void rowMultTrc (unc **dst, unc **src, int X, int Y, unt dW, unt dH, double factor);
void rowMultUnc (unc **dst, unc **src, int X, int Y, unt dW, unt dH, double factor);

void
rowMultFltToUnc (ppu dst, ppf src, int X, int Y, unt dW, unt dH, double factor);

#endif	/* im_rowMult_h_ */

