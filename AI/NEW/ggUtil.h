/*****************************************************************************
 ggUtil.h    S.R.Lines    93.8.17
#include <unTypes.h>
 *****************************************************************************/
#ifndef ggUtil_h__
#define ggUtil_h__

FILE *ggLogO ( int dX, int dY, unt dW, unt dH, unt sW, unt sH
, unt qW, unt qH, char *dir, char **iname, int ntweens, int pvim
, dbl trsBeg, dbl trsEnd);

FILE *ggLogH ( int *dims, char *dir, int pvim, FILE *pFiles
             , char **argStr, int *argInt, dbl *argDbl);

char	*pvimStr (unt pvim);

int	clipOn4 (float dst[4], float src[4]);

int	clipOnN (float *dst, float *src, int len);

#endif	/* ggUtil_h__ */
