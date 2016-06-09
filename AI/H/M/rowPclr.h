/*****************************************************************************
rowPclr.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.27-8.17
Pseudo-colorize an image stored as an array of pointers to rows,
where the pixel values can be negative.
*****************************************************************************/

void
rowPclrFF ( ppf dst, ppf src
          , int dX, int dY, unt dW, unt dH
          , int sX, int sY, unt sW, unt sH
          , dbl factor, dbl mean, dbl rms, dbl lo, dbl hi);

void
rowPclrToUnc ( unc **dst, void **src
               , int dX, int dY, unt dW, unt dH
               , int sX, int sY, unt sW, unt sH
               , unt mmss, dbl mean, dbl sdev);


void
rowPclrFU ( unc **dst ,flt **src
          , int dX, int dY, unt dW, unt dH
          , int sX, int sY, unt sW, unt sH
          , dbl mean, dbl sdev);


