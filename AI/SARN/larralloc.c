/* Long array allocation routines */

long **alloc_larray();
long *alloc_lvector();

/*****************************************************************************/

long **_alloc_larray(h,w)
int h,w;
{
  return(alloc_larray(0,h-1,0,w-1));
}

/*****************************************************************************/

long **alloc_larray(lh,uh,lw,uw)
int lh,uh,lw,uw;
{
   register long **ptr;

   if (!(ptr = (long **) alloc_array(lh,uh,lw,uw,sizeof(long)))) {
     perror("alloc_larray");
     die("alloc_larray: die'd trying to allocate long array [%d:%d, %d:%d]\n",lh,uh,lw,uw);
   }
   return ptr;
}

/*****************************************************************************/

long *_alloc_lvector(dim)
int dim;
{
  return(alloc_lvector(0,dim-1));
}

/*****************************************************************************/

long *alloc_lvector(l,h)
int l,h;
{
   long *ptr;

   if (!(ptr = (long *) alloc_vector(l,h,sizeof(long)))) {
     perror("alloc_lvector");
     die("alloc_lvector: die'd trying to allocate long vector [%d:%d]\n",l,h);
   }
   return ptr;
}

/*****************************************************************************/

_free_larray(ptr)
long **ptr;
{
  _free_array((void **) ptr, sizeof(long));
}

/*****************************************************************************/

free_larray(ptr,lh,uh,lw,uw)
long **ptr;
int lh,uh,lw,uw;
{
  free_array((void **) ptr,lh,uh,lw,uw,sizeof(long));
}

/*****************************************************************************/

_free_lvector(ptr)
long *ptr;
{
  _free_vect((void *) ptr,sizeof(long));
}

/*****************************************************************************/

free_lvector(ptr,l,h)
long *ptr;
int l,h;
{
  free_vect((void *) ptr,l,h,sizeof(long));
}

/*****************************************************************************/

