/*****************************************************************************
 rowZero.h    S.R.Lines (spraxlo@ai)    93.6.1-9.20
#include <unTypes.h>
 *****************************************************************************/

void rowSetF ( ppf dst, int xo, int yo, unt wide, unt tall, double fv);

void rowZeroF ( ppf dst, int xo, int yo, unt wide, unt tall );
void rowZeroU ( ppu dst, int xo, int yo, unt wide, unt tall );

void rowZeroFlt (ppf dst,                unt wide, unt tall );
void rowZeroUnc (ppu dlp,                unt wide, unt tall );

void rowZero ( void **dst, unt wide, unt tall, unt mmss);



