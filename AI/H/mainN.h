/*****************************************************************************
 mainJ.h     S.R.Lines    93.6.9-9.19
		--would like to see this shrivel to nothing.
#include	<unMath.h>
#include	<assert.h>
#include	"ggParse.h"
 *****************************************************************************/
#include	<un/unParse.h>
#include	"gg.h"
#include        <un/unMath.h>
#include        <m/mj.h>
#include        <m/mjBord.h>
#include        "manCor.h"

#include	"beymer/lists.h"
#include	"beymer/label.h"
#include	"labelBase.h"
#include	"rpBase.h"
/*****************************************
#include	"allocImage.h"
#include	"morphBase.h"
 *****************************************/

#include	"inExtern.h"
#include	"inParm.h"
#include	"inLoad.h"
#include	"mob.h"
#include	"jk.h"

#define	NTWEENS 9	/* default */

/* GLOBAL VARIABLES SET TO DEFAULT VALUES */

char    *ArgStr[MAXARG];
int      ArgInt[MAXARG];
dbl      ArgDbl[MAXARG];

dbl	 Amplif  = 5.00;
dbl      TrsMed  = 0.50;
dbl      TrsEnd  = 1.00;
dbl      TrsBeg  = 0.00;
dbl      InFill  = 0.27;
flt      MvZoom  = 2.00F;
int      OutIFF  = FF_SGI;
int      PyrTop  = 4;
int      PyrBot  = 0;
int      Ntweens = 5;
int      Ytweens = 4;
int      Ztweens = 3;
int      Image_border=6;   /* SHOULD ALWAYS BE >= 4, else Sarnoff code
			crashes in reflect.c:ver_reflect(...,border=4,...); */


