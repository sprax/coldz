/*****************************************************************************
memSoak.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.11-12
Doesn't yet handle interleaved data of different C types.
TERMS: infimum: a strict lower bound, ie valid values must be > infimum.
       minimum: a semi-lower bound, ie valid values must be >= minimum.
       supremum: strict upper bound -- threshold values to be < supremum.
       maximum: semi-upper bound -- bounded values must be <= maximum.
*****************************************************************************/
         
#ifndef  im_memSoak_h__
#define  im_memSoak_h__

void memSoakUnc ( puc mem, unt scan, unt wide, unt tall
                , int ix, int iy, int radius, int val);

void memSoakUncA (puc mem, unt scan, unt wide, unt tall, int infimum);
void memSoakUncB (puc mem, unt scan, unt wide, unt tall, int infimum);
void memSoakUncC (puc mem, unt scan, unt wide, unt tall, int infimum);
void memSoakUncD (puc mem, unt scan, unt wide, unt tall, int infimum);
void memSoakUncE (puc mem, unt scan, unt wide, unt tall, int infimum);

#endif   /* im_memSoak_h__ */


