/*****************************************************************************
 mjBord.h    S.R.Lines     94.4.25
 *****************************************************************************/

void
mjBordUnc ( ppu img[]
, int dL, int dT, unt dW, unt dH
, int cL, int cT, unt cW, unt cH
, int sL, int sT, unt sW, unt sH, int rtype, int dD);

void
rowBordUnc ( ppu img
, int dL, int dT, unt dW, unt dH
, int cL, int cT, unt cW, unt cH
, int sL, int sT, unt sW, unt sH, int rtype);

void
mjBordFlt ( ppf img[]
, int dL, int dT, unt dW, unt dH
, int cL, int cT, unt cW, unt cH
, int sL, int sT, unt sW, unt sH, int rtype, int dD);

void
mjBordFltBrd ( ppf img
, int dL, int dT, unt dW, unt dH
, int cL, int cT, unt cW, unt cH
, int sL, int sT, unt sW, unt sH, int rtype);

