/*****************************************************************************
rowClip.h    S.R.Lines (spraxlo@ai)    93.6.12-7.11
*****************************************************************************/
#ifndef im_rowClip_h_
#define im_rowClip_h_

/* #include <unTypes.h> */

void rowClip(void **dlp, void **slp, unt wide, unt tall, unt dmmss, unt smmss, dbl lo, dbl hi);

void
rowClipFU8 ( ppu dst, ppf src, int dX, int dY, unt dW, unt dH, int sX, int sY);

void rowClipToUnc(unc **dlp, void **slp, unt wide, unt tall, unt srctype, dbl lo, dbl hi);
void rowClipToFlt(flt **dlp, void **slp, unt wide, unt tall, unt srctype, dbl lo, dbl hi);

void rowClipVoid(void **dlp, void **slp, unt wide, unt tall, dbl lo, dbl hi);
void rowClipFlt(  flt **dlp, flt  **slp, unt wide, unt tall, dbl lo, dbl hi);
void rowClipUnc(  unc **dlp, unc  **slp, unt wide, unt tall, dbl lo, dbl hi);

void
rowClipFltToUnc ( ppu dst, ppf src, unt dW, unt dH, dbl lo, dbl hi);

void
rowClipFU ( ppu dst, ppf src
          , int dX, int dY, unt dW, unt dH
          , int sX, int sY, unt sW, unt sH, dbl lo, dbl hi);
void
rowMuCpFU ( ppu dst, ppf src
              , int dX, int dY, unt dW, unt dH
              , int sX, int sY, unt sW, unt sH, dbl factor, dbl lo, dbl hi);

#endif	/* im_rowClip_h_ */


