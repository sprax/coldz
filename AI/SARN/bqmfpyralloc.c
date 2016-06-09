/* QMF Pyramid allocation routines */

#include "imgdec.h"
#include "imgmacro.h"

extern int Image_border;

/*****************************************************************************/

def_bqmfpyr(pyr,lev,w,h)
BQMFPYR pyr;
int lev,w,h;
{
   int i,j;

   for (i=0; i < 4; i++) {
     pyr[lev][i].w = w;
     pyr[lev][i].h = h;
   }
   for (j=lev-1; j >= 0; j--)
     for (i=0; i < 4; i++) {
       pyr[j][i].w = pyr[j+1][0].w*2;
       pyr[j][i].h = pyr[j+1][0].h*2;
     }
   for (j=lev+1; MIN(pyr[j-1][0].w,pyr[j-1][0].h) > 1; j++)
     for (i=0; i < 4; i++) {
       pyr[j][i].w = pyr[j-1][0].w/2;
       pyr[j][i].h = pyr[j-1][0].h/2;
     }
}

/*****************************************************************************/

alloc_bqmfpyr(pyr,botlev,toplev)
BQMFPYR pyr;
int botlev,toplev;
{
   int lev,i;

   for (lev=botlev; lev <= toplev ; lev++)
     for (i=0; i < 4; i++) {
       if (lev == 0 && i > 0)
         break;
       alloc_fimage(&(pyr[lev][i]));
     }
}

/*****************************************************************************/

free_bqmfpyr(pyr,botlev,toplev)
BQMFPYR pyr;
int botlev,toplev;
{
   int lev,i;

   for (lev=botlev; lev <= toplev ; lev++)
     for (i=0; i < 4; i++) {
       if (lev == 0 && i > 0)
         break;
       free_fimage(pyr[lev][i]);
     }
}

/*****************************************************************************/

