/*****************************************************************************
mjMake.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.20-23
#include <unTypes.h>
*****************************************************************************/
#ifndef	mjMake_h
#define mjMake_h

ppu
mjSubUnc (ppu dst, ppu src, int dX, int dY, int sX, int sY, unt sW, unt sH);



#if 0000

ush	**mjMalUsh (int xi, int yi, unt wide, unt tall);
shr	**mjMalShr (int xi, int yi, unt wide, unt tall);

void	*mjMem (void **mj, void *mem, unt wide, unt tall, unt ctype);
unc	**mjMemUnc (unc **mj, unc *mem, unt wide, unt tall);
flt	**mjMemFlt (flt **mj, flt *mem, unt wide, unt tall);

void	*mjCal (int wide, int tall, int ctype);
void	*mjMal (int wide, int tall, int ctype);
dbl 	**mjCalDbl (int X, int Y, unt wide, unt tall);
dbl 	**mjMalDbl (int X, int Y, unt wide, unt tall);
flt 	**mjCalFlt (int X, int Y, unt wide, unt tall);
flt 	**mjMalFlt (int X, int Y, unt wide, unt tall);
flt 	**mjRalFlt (ppf old, int xi, int yi, unt wide, unt tall);

unc 	**mjCalUnc (int X, int Y, unt wide, unt tall);
unc 	**mjMalUnc (int X, int Y, unt wide, unt tall);
void	mjFree (void **mjs);
void	mjKillFlt (ppf mjs, int X, int Y);
void	mjKillUnc (ppu mjs, int X, int Y);

ppf	mjDupFlt(ppf src, int xi, int yi, unt wide, unt tall);


#endif	/* 0000 */
#endif	/* mjMake_h */


