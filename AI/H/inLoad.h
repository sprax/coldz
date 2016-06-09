/*****************************************************************************
 inLoad.h    S.R.Lines     95.05.27
#include	<string.h>
#include	<unBug.h>
#include	<unMath.h>
#include	<unTypes.h>
#include	"mj.h"
 *****************************************************************************/
#ifndef inLoad_h__
#define inLoad_h__

int inReadFiles( ppu inpU[][4], LblFace sLbl[], char *iname[], pDmj dm);

void   inKill (ppf *sRGB, ppf *srcG, pDmj sD, pDmj fD);

void   inInit(void);

#endif  /* inLoad_h__ */


