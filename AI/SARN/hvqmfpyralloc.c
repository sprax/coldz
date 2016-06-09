/* HV QMF Pyramid allocation routines */

#include "imgdec.h"
#include "imgmacro.h"

extern int Image_border;

/*****************************************************************************/

def_hvqmfpyr(pyr,lev,w,h)
HVQMFPYR pyr;
int lev,w,h;
{
   int i,j;

   for (i=0; i < 2; i++) {
     pyr[lev][i].w = w;
     pyr[lev][i].h = h;
   }
   for (j=lev-1; j >= 0; j--)
     for (i=0; i < 2; i++) {
       pyr[j][i].w = (j%2) ? pyr[j+1][0].w*2 : pyr[j+1][0].w;
       pyr[j][i].h = (j%2) ? pyr[j+1][0].h : pyr[j+1][0].h*2;
     }
   for (j=lev+1; MIN(pyr[j-1][0].w,pyr[j-1][0].h) > 1; j++)
     for (i=0; i < 2; i++) {
       pyr[j][i].w = (j%2) ? pyr[j-1][0].w/2 : pyr[j-1][0].w;
       pyr[j][i].h = (j%2) ? pyr[j-1][0].h : pyr[j-1][0].h/2;
     }
}

/*****************************************************************************/

alloc_hvqmfpyr(pyr,botlev,toplev)
HVQMFPYR pyr;
int botlev,toplev;
{
   int lev,i;

   for (lev=botlev; lev <= toplev ; lev++)
     for (i=0; i < 2; i++) {
       if (lev == 0 && i > 0)
         break;
       alloc_fimage(&(pyr[lev][i]));
     }
}

/*****************************************************************************/

free_hvqmfpyr(pyr,botlev,toplev)
HVQMFPYR pyr;
int botlev,toplev;
{
   int lev,i;

   for (lev=botlev; lev <= toplev ; lev++)
     for (i=0; i < 2; i++) {
       if (lev == 0 && i > 0)
         break;
       free_fimage(pyr[lev][i]);
     }
}

/*****************************************************************************/

