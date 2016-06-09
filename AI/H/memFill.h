/*****************************************************************************
memFill.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.11-12
Doesn't yet handle interleaved data of different C types.
TERMS: infimum: a strict lower bound, ie valid values must be > infimum.
       minimum: a semi-lower bound, ie valid values must be >= minimum.
       supremum: strict upper bound -- threshold values to be < supremum.
       maximum: semi-upper bound -- bounded values must be <= maximum.
*****************************************************************************/
         
#ifndef  im_memFill_h__
#define  im_memFill_h__

void memFillUnc  (puc mem, unt scan, unt wide, unt tall);

void memFillUncA (puc mem, unt scan, unt wide, unt tall, int infimum);
void memFillUncB (puc mem, unt scan, unt wide, unt tall, int infimum);
void memFillUncC (puc mem, unt scan, unt wide, unt tall, int infimum);
void memFillUncD (puc mem, unt scan, unt wide, unt tall, int infimum);
void memFillUncE (puc mem, unt scan, unt wide, unt tall, int infimum);

#endif   /* im_memFill_h__ */


