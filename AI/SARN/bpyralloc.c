/* Byte pyramid allocation routines */

#include "imgdec.h"
#include "imgmacro.h"

extern int Image_border;

/*****************************************************************************/

def_bpyr(pyr,lev,w,h)
BPYR pyr;
int lev,w,h;
{
   int j;

   pyr[lev].w = w;
   pyr[lev].h = h;
   for (j=lev-1; j >= 0; j--) {
     pyr[j].w = pyr[j+1].w*2;
     pyr[j].h = pyr[j+1].h*2;
   }
   for (j=lev+1; MIN(pyr[j-1].w,pyr[j-1].h) > 1; j++) {
     pyr[j].w = (pyr[j-1].w+1)/2;
     pyr[j].h = (pyr[j-1].h+1)/2;
   }
}

/*****************************************************************************/

alloc_bpyr(pyr,botlev,toplev)
BPYR pyr;
int botlev,toplev;
{
   int i;

   for (i=botlev; i <= toplev ; i++)
     alloc_bimage(&(pyr[i].w));
}

/*****************************************************************************/

free_bpyr(pyr,botlev,toplev)
BPYR pyr;
int botlev,toplev;
{
   int i;

   for (i=botlev; i <= toplev ; i++)
     free_bimage(pyr[i]);
}

/*****************************************************************************/

