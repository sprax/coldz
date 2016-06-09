/******************************************************************************
rowDist.h    S.R.Lines (spraxlo@ai.mit.edu)    93.12.7
******************************************************************************/
 
void
rowDist (pvd dst, pvd src, int xi, int yi, unt dW, unt dH, unt mmss, dbl *dist);

void
rowDistFlt (ppf dst, ppf src, int xi, int yi, unt dW, unt dH, dbl *dist);

void
rowDistUnc (ppu dst, ppu src, int xi, int yi, unt dW, unt dH, dbl *dist);


