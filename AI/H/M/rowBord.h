/*****************************************************************************
 rowBord.h    S.R.Lines     93.7.2-11.13
 *****************************************************************************/
#ifndef rowBord_h__
#define rowBord_h__

#define BD_RFLX_ODD	1
#define BD_RFLX_EVEN	2

#define BD_COPY	3
#define BD_MEAN	1
#define BD_ROTV	1
#define BD_ROTH	1
/******
#define BD_BLAK	1
#define BD_WHIT	1
#define BD_GRAY	1
******/

void
rowBordFlt(ppf src, int sX, int sY, unt sW, unt sH, unt bord, unt rtype);


#endif /* rowBord_h__ */


