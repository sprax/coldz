/*****************************************************************************
 mjRead.h    S.R.Lines    94.7.30
 *****************************************************************************/
#ifndef mjRead_h__
#define mjRead_h__

unt mjReadBordUnc ( ppu mcU[], pDmj D, char *path, unt flags);

unt
mjReadBordUncSgi
( ppu ret[], unt *dW, unt *dH, int *L, int *R, int *T, int *B
, int oX, int oY, char *path, unt flags);

#endif/*mjRead_h__*/


