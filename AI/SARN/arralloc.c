/* array and vector allocation routines */
#include <stdlib.h>
#include "imgdec.h"
#include "imgmacro.h"

/*****************************************************************************/

void **_alloc_array(int h, int w, int size)
{
  return alloc_array(0,h-1,0,w-1,size);
}

/*****************************************************************************/

void **alloc_array(int lh, int uh, int lw, int uw, int size)
{
  register char **ptr,**p,**fp;
  register char *area;
  int rsize = (uw-lw+1)*size;		/* size of a row */

  ptr = (char **) malloc((uh-lh+1)*sizeof(*ptr));
  if (!ptr) return (void **) NULL;

  area = (char *) malloc((uw-lw+1)*(uh-lh+1)*size);
  if (!area) return (void **) NULL;
  for (p=ptr,fp=p+(uh-lh+1),area-=lw*size; p < fp; p++,area += rsize)
     *p = (char *) area;
   return (void **) (ptr-lh);
}

/*****************************************************************************/

void *_alloc_vector(int dim, int size)
{
  return alloc_vector(0,dim-1,size);
}

/*****************************************************************************/

void *alloc_vector(int l, int h, int size)
{
  char *ptr;

  ptr = (char *) malloc((h-l+1)*size);
  return (ptr) ? (void *) (ptr-l*size) : NULL;
}

/*****************************************************************************/

void
_free_array(char **ptr, int size)
{
  free(*ptr);
  free((void*)ptr);
}

/*****************************************************************************/

void
free_array(char **ptr, int lh, int uh, int lw, int uw, int size)
{
  free(*(ptr+lh) + lw*size);
  free((void*)(ptr+lh));
}

/*****************************************************************************/

void
_free_vect(char *ptr, int size)
{
  free(ptr);
}

/*****************************************************************************/

void
free_vect(char *ptr, int l, int h, int size)
{
  free(ptr+l*size);
}

/*****************************************************************************/

