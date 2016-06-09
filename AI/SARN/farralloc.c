/* Float array allocation routines */

#include "sarnoff/imgdec.h"

/*****************************************************************************/

float **_alloc_farray(int h, int w)
{
  return(alloc_farray(0,h-1,0,w-1));
}

/*****************************************************************************/

float **alloc_farray(int lh, int uh, int lw, int uw)
{
   register float **ptr;

   if (!(ptr = (float **) alloc_array(lh,uh,lw,uw,sizeof(float)))) {
     perror("alloc_farray");
     die("alloc_farray: die'd trying to allocate float array [%d:%d, %d:%d]\n",lh,uh,lw,uw);
   }
   return ptr;
}

/*****************************************************************************/

float *_alloc_fvector(int dim)
{
  return(alloc_fvector(0,dim-1));
}

/*****************************************************************************/

float *alloc_fvector(int l, int h)
{
   float *ptr;

   if (!(ptr = (float *) alloc_vector(l,h,sizeof(float)))) {
     perror("alloc_fvector");
     die("alloc_fvector: die'd trying to allocate float vector [%d:%d]\n",l,h);
   }
   return ptr;
}

/*****************************************************************************/

void _free_farray(float **ptr)
{
  _free_array((char **) ptr, sizeof(float));
}

/*****************************************************************************/

void free_farray(float **ptr, int lh, int uh, int lw, int uw)
{
  free_array((char **) ptr,lh,uh,lw,uw,sizeof(float));
}

/*****************************************************************************/

void _free_fvector(float *ptr)
{
  _free_vect((void *) ptr,sizeof(float));
}

/*****************************************************************************/

void free_fvector(float *ptr, int l, int h)
{
  free_vect((void *) ptr,l,h,sizeof(float));
}

/*****************************************************************************/



