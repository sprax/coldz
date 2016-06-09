/*****************************************************************************
rowFill.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.11-26
Doesn't yet handle interleaved data of different C types.
TERMS: infimum: a strict lower bound, ie valid values must be > infimum.
       minimum: a semi-lower bound, ie valid values must be >= minimum.
       supremum: strict upper bound -- threshold values to be < supremum.
       maximum: semi-upper bound -- bounded values must be <= maximum.
Naming scheme for fill functions:
	Blot: from ix,iy fill the maximal square, given boundaries & radius.
	Fill: from ctr, fill entire rectable except for 1 pxl border.
	Soak: from ix,iy, fill entire rectangle, even edge pxls. 
*****************************************************************************/
         
#ifndef  im_rowFill_h__
#define  im_rowFill_h__

void
rowFillFlt ( ppf dstQ, ppf accW, int X, int Y, unt wide, unt tall
           , double infimum, double dmean);



void rowFillUnc (puc *row, int X, int Y, unt wide, unt tall, int val);

void rowSoakUnc ( puc *row, unt wide, unt tall, int val);
void rowBlotUnc ( puc *row, unt wide, unt tall
                , int ix, int iy, int radius, int val);

void rowFillUncOld(puc *row, unt wide, unt tall, int val);


#if 0
void rowSoakFlt ( pfl *row, unt wide, unt tall, dbl dval, dbl infimum);
void rowBlotFlt ( pfl *row, unt wide, unt tall
                , int ix,   int iy, int radius, dbl dval, dbl infimum);
void rowFillUncA (puc mem, unt scan, unt wide, unt tall, int infimum);
void rowFillUncB (puc mem, unt scan, unt wide, unt tall, int infimum);
void rowFillUncC (puc mem, unt scan, unt wide, unt tall, int infimum);
void rowFillUncD (puc mem, unt scan, unt wide, unt tall, int infimum);
void rowFillUncE (puc mem, unt scan, unt wide, unt tall, int infimum);
#endif

#endif   /* im_rowFill_h__ */


