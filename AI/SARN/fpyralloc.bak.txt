/* Float pyramid allocation routines */

#include "imgdec.h"
#include "imgmacro.h"

extern int Image_border;

/*****************************************************************************/

def_fpyrs(pyrs,npyr,lev,w,h)
FPYR pyrs[];
int npyr,lev,w,h;
{
   int j;

   for (j=0; j < npyr; j++)
     def_fpyr(pyrs[j],lev,w,h);
}

/*****************************************************************************/

alloc_fpyrs(pyrs,npyr,botlev,toplev)
FPYR pyrs[];
int npyr,botlev,toplev;
{
   int j;

   for (j=0; j < npyr; j++)
     alloc_fpyr(pyrs[j],botlev,toplev);
}

/*****************************************************************************/

def_fpyr(pyr,lev,w,h)
FPYR pyr;
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

alloc_fpyr(pyr,botlev,toplev)
FPYR pyr;
int botlev,toplev;
{
   int i;

   for (i=botlev; i <= toplev ; i++)
     alloc_fimage(&(pyr[i]));
}

/*****************************************************************************/

free_fpyr(pyr,botlev,toplev)
FPYR pyr;
int botlev,toplev;
{
   int i;

   for (i=botlev; i <= toplev ; i++)
     free_fimage(pyr[i]);
}

/*****************************************************************************/

