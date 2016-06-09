/*****************************************************************************
 wxyMake.h    S.R.Lines     93.8.17
#include	<unTypes.h>
 *****************************************************************************/
#ifndef wxyMake_h__
#define wxyMake_h__

typedef	struct	wxy {
	flt **w;
	flt **x;
	flt **y;
}	wxy;

void wxyZeroFlt ( flt **accW, flt **invX, flt **invY
                , int X, int Y, unt wide, unt tall);

#endif	/* wxyMake_h__ */





