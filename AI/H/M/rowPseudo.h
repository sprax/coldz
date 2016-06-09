/*****************************************************************************
rowPseudo.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.27-8.17
Pseudo-colorize an image stored as an array of pointers to rows,
where the pixel values can be negative.
*****************************************************************************/
void
rowPseudoToUnc ( unc **dst, void **src
               , int dX, int dY, unt dW, unt dH
               , int sX, int sY, unt sW, unt sH
               , unt mmss, dbl mean, dbl sdev);


void
rowPseudoFltToUnc ( unc **dst ,flt **src
                  , int dX, int dY, unt dW, unt dH
                  , int sX, int sY, unt sW, unt sH
                  , dbl mean, dbl sdev);


