/*****************************************************************************
 ggMain.h     S.R.Lines    93.6.9-9.19
		--would like to see this shrivel to nothing.
#include	<unMath.h>
 *****************************************************************************/
#include	<assert.h>
#include	<unParse.h>
#include	"gg.h"
#include	"ggParse.h"
#include	"sarnoff.h"
#include	"img_util.h"

#define BEYMER 1

#ifdef   BEYMER
#include	"beymer/image.h"
#include	"beymer/io.h"
#include	"beymer/io_sgi.h"

#include	"beymer/lists.h"

#include	"beymer/label.h"
#include	"beymer/image_new.h"
#include	"beymer/interface.h"

#include	"label_base.h"
#include	"morph_base.h"
#if	0
#include	"rp_base.h"
#else
void	fix_eye_locations(image_t *image, List labels, int pose_index);
int 	read_image_and_labels(image_t **image, List *labels, char *filename);
#endif

#include	"allocImage.h"
#include	"labelBase.h"
#include	"morphBase.h"
#include	"rpBase.h"

#endif /*BEYMER*/

#include	"ggExtern.h"

#define	NTWEENS 9	/* default */


/* GLOBAL VARIABLES SET TO DEFAULT VALUES */

char    *ArgStr[MAXARG];
int      ArgInt[MAXARG];
dbl      ArgDbl[MAXARG];

int      PyrTop  = 4;
int      PyrBot  = 0;

dbl 	TrsMed  = 0.50;
dbl 	TrsEnd  = 1.00;
dbl 	TrsBeg  = 0.00;
dbl 	InFill  = 0.30;
dbl 	MvZoom  = 2.00;
int     Image_border=4;   /* SHOULD ALWAYS BE >= 4, else Sarnoff code
			crashes in reflect.c:ver_reflect(...,border=4,...); */



