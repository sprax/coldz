/*****************************************************************************
memMake.h    S.R.Lines (spraxlo@ai.mit.edu)    93.7.20-23
*****************************************************************************/
#ifndef im_memMake_h_
#define im_memMake_h_

/* #include <unTypes.h> */

void	 memFree (void *mem);
void	*memMal (int wide, int tall, int ctype);
void	*memCal (int wide, int tall, int ctype);
void	*memRow (void *mem, void **row, unt wide, unt tall, unt ctype);

flt	*memCalFlt (int wide, int tall);
flt	*memMalFlt (int wide, int tall);
flt	*memRowFlt (flt *mem, flt **row, unt wide, unt tall);

unc	*memCalUnc (int wide, int tall);
unc	*memMalUnc (int wide, int tall);
unc	*memRowUnc (unc *mem, unc **row, unt wide, unt tall);

#endif	/* im_memMake_h_ */

