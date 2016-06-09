/*****************************************************************************
memPlug.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.11-12
Doesn't yet handle interleaved data of different C types.
TERMS: infimum: a strict lower bound, ie valid values must be > infimum.
       minimum: a semi-lower bound, ie valid values must be >= minimum.
       supremum: strict upper bound -- threshold values to be < supremum.
       maximum: semi-upper bound -- bounded values must be <= maximum.
*****************************************************************************/
         
#ifndef  im_memPlug_h__
#define  im_memPlug_h__

void memPlugFltXYC(pfl srcX, pfl srcY, puc srcU, unt scan, unt wide, unt tall, int infimum);
void memPlugFltXYColl(pfl srcX, pfl srcY, puc srcU, unt scan, unt wide, unt tall, int infimum);

#endif   /* im_memPlug_h__ */


