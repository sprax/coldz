/*****************************************************************************
 rowSGI.h    S.R.Lines    93.8.15-10.12
 *****************************************************************************/

void rowGraySGIn (ppu src, int sX, int sY, unt sW, unt sH, char *path);

unt
sgiReadUn ( ppu *ret, unt *dW, unt *dH
          , int oX, int oY, unt mW, unt mH, char *path, unt flags);

unt
sgiReadU( ppu *ret, unt *dW, unt *dH
        , int oX, int oY, unt mW, unt mH, char *path, unt flags);

int    /** replaced rowWriteSGIN **/
rowWritSGImU(ppu *srcU, int dX, int dY, unt dW, unt dH, unt chan, char *path);

int
rowWritSgiU(ppu *srcU, int dX, int dY, unt dW, unt dH, unt chan, char *path);

void
rowWriteSGI(ppf src, int sX, int sY, unt sW, unt sH, char *path);

void
sgiWritFB(ppf rgbp[4], int dX, int dY, unt dW, unt dH, unt dD, char *path);

unt
readSgiUsh (ppus ret[4], unt *dW, unt *dH, int oX, int oY, char *path, unt flags);

void
writSgiUsh(ppus rgbp[4], unt dW, unt dH, char *path);

unt
sgiRead4U (ppu ret[4], unt *dW, unt *dH, int oX, int oY, char *path, unt flags);

void
sgiWritFlt (ppf src, int sX, int sY, unt sW, unt sH, char *path);

void
sgiWritUB(ppu rgbp[4], unt dW, unt dH, char *path);



	/* old style:	*/
void
rowGraySGI(ppu src, int xo, int yo, unt wide, unt tall, char *path);

void *
rowReadSGI(char *path, unt *wide, unt *tall, unt *deep, unt flags);

void
rowWriteSGIN(ppu *srcU, unt dW, unt dH, unt chan, char *path);

void
rgbWriteSGI(ppf rgbp, int dX, int dY, unt dW, unt dH, char *path);

void
rgbWriteSGIn(ppf rgbp, int dX, int dY, unt dW, unt dH, char *path);




