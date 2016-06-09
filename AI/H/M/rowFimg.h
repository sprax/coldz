/***************************************************************************
 rowFimg.h    S.R.Lines    95.05.27
 ***************************************************************************/
#include        <m/rowMake.h>
#include        <m/rowFile.h>
#include        <sarnoff/imghdr.h>

int
rowReadFimg ( ppf *inp, int iX, int iY, unt iW, unt iH, unt sw, unt sh
           , unt hdrBytes, const char *path, const char *ext, unt flags);

int
rowWriteFimg ( ppf src, int xi, int yi, unt wide, unt tall
             , const char *path, const char *ext, unt flags);


