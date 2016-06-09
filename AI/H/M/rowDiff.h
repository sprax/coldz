/******************************************************************************
rowDiff.h    S.R.Lines (spraxlo@ai.mit.edu)    93.12.7
Pixelwise difference between two images stored as arrays of pointers to rows.
******************************************************************************/

void
rowAddFlt (ppf dst, ppf src, ppf add, int xi, int yi, unt dW, unt dH);

void
rgbDiffFracAmp (ppf dst, ppf src, ppf sub, int dX, int dY, unt dW, unt dH);

void
rgbAbsDiffFrac (ppf dst, ppf src, ppf sub, int dX, int dY, unt dW, unt dH);

void
rowAbsDiffFrac (ppf dst, ppf src, ppf sub, int dX, int dY, unt dW, unt dH);

void
rgbMaxAbsDiffFrac (ppf dst, ppf src, ppf sub, int dX, int dY, unt dW, unt dH);

void
rowCompWts(ppf dst, ppf src, int dX, int dY, unt dW, unt dH, dbl clr, dbl amp);


void
rgbDiffFlt(ppf dst, ppf src, ppf sub, int xi, int yi, unt dW, unt dH, dbl mid);
 
void
rowDiff(void *dst,void *src,void *sub,int xi,int yi,unt dW,unt dH,unt mmss);

void
rowDiffFlt (ppf dst, ppf src, ppf sub, int xi, int yi, unt dW, unt dH);

void
rowDiffUnc (ppu dst, ppu src, ppu sub, int xi, int yi, unt dW, unt dH);


