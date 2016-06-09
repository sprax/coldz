/*****************************************************************************
 rowCpack.h    S.R.Lines    93.10.10
#include      <unTypes.h>
 *****************************************************************************/
#ifndef im_rowCpack_h__ 
#define im_rowCpack_h__

void cPackFltRGBI(rgst unl *l, ppf rgbf, int sX, int sY, unt sW, unt sH);
void cPackUncRGBP(rgst unl *l, ppu rgbp[], int x, int y, unt w, unt h);
void cpackUncGray(rgst unl *l, rgst unc *u, unt n);
void cpackShrGray(rgst unl *l, shr *b, unt n);
void cpackFltGray(rgst unl *l, rgst flt *f, register size_t n);
void cpackUncRGBP(rgst unl *l, rgst unc *r, rgst puc g, rgst puc b, rgst int n);
void cpackFltRGBI(rgst unl *l, rgst flt *f, rgst int n);

#endif  /* im_rowCpack_h__ */


