/*****************************************************************************
rowMake.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.20-23
#include <unTypes.h>
*****************************************************************************/
#ifndef	im_rowMake_h_
#define im_rowMake_h_

ppf rowPntFlt (ppf src, int dX, int dY, int sX, int sY, unt sW, unt sH);
ppu rowPntUnc (ppu src, int dX, int dY, int sX, int sY, unt sW, unt sH);


void **rowMalVoid (int xi, int yi, unt wide, unt tall, size_t eltSize);


ush	**rowMalUsh (int xi, int yi, unt wide, unt tall);
shr	**rowMalShr (int xi, int yi, unt wide, unt tall);

void	*rowMem (void **row, void *mem, unt wide, unt tall, unt ctype);
unc	**rowMemUnc (unc **row, unc *mem, unt wide, unt tall);
flt	**rowMemFlt (flt **row, flt *mem, unt wide, unt tall);

void	*rowCal (int wide, int tall, int ctype);
void	*rowMal (int wide, int tall, int ctype);
dbl 	**rowCalDbl (int X, int Y, unt wide, unt tall);
dbl 	**rowMalDbl (int X, int Y, unt wide, unt tall);
flt 	**rowCalFlt (int X, int Y, unt wide, unt tall);
flt 	**rowMalFlt (int X, int Y, unt wide, unt tall);
flt 	**rowRalFlt (ppf old, int xi, int yi, unt wide, unt tall);

unc 	**rowCalUnc (int X, int Y, unt wide, unt tall);
unc 	**rowMalUnc (int X, int Y, unt wide, unt tall);
void	rowFree (void **rows);
void	rowKillFlt (ppf rows, int X, int Y);
void	rowKillUnc (ppu rows, int X, int Y);

ppf	rowDupFlt(ppf src, int xi, int yi, unt wide, unt tall);





#endif	/* im_rowMake_h_ */


