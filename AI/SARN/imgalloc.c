/* image allocation routines */

#include <stdlib.h>
#include "imgdec.h"
#include "imgmacro.h"

void **_alloc_image(int w, int h, int size)
{
  return alloc_image(w,h,size,0);
}

/*****************************************************************************/

void **alloc_image(int w, int h, int size, int border)
{
   register char **ptr,**p,**fp;
   register char *area;
   int rsize=(w+2*border)*size;		/* full length of a row */

   ptr = (char **) malloc((h+2*border)*sizeof(*ptr));
   if (!ptr) return (void **) NULL;

   area = (char *) malloc((w+2*border)*(h+2*border)*size);
   if (!area) return (void **) NULL;
   for (p=ptr,fp=p+h+2*border,area+=border*size; p < fp; p++,area += rsize)
     *p = (char *) area;
   return (void **) (ptr+border);
}

/*****************************************************************************/

void
_free_image(char **ptr, int size)
{
   free(*ptr);
   free((void*)ptr);
}

/*****************************************************************************/

void
free_image(char **ptr, int size, int border)
{
   free(*(ptr-border) - border*size);
   free((void*)(ptr-border));
}

/*****************************************************************************/


