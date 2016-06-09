/* Short array allocation routines */

short **alloc_sarray();
short *alloc_svector();

/*****************************************************************************/

short **_alloc_sarray(h,w)
int h,w;
{
  return(alloc_sarray(0,h-1,0,w-1));
}

/*****************************************************************************/

short **alloc_sarray(lh,uh,lw,uw)
int lh,uh,lw,uw;
{
   register short **ptr;

   if (!(ptr = (short **) alloc_array(lh,uh,lw,uw,sizeof(short)))) {
     perror("alloc_sarray");
     die("alloc_sarray: die'd trying to allocate short array [%d:%d, %d:%d]\n",lh,uh,lw,uw);
   }
   return ptr;
}

/*****************************************************************************/

short *_alloc_svector(dim)
int dim;
{
  return(alloc_svector(0,dim-1));
}

/*****************************************************************************/

short *alloc_svector(l,h)
int l,h;
{
   short *ptr;

   if (!(ptr = (short *) alloc_vector(l,h,sizeof(short)))) {
     perror("alloc_svector");
     die("alloc_svector: die'd trying to allocate short vector [%d:%d]\n",l,h);
   }
   return ptr;
}

/*****************************************************************************/

_free_sarray(ptr)
short **ptr;
{
  _free_array((void **) ptr, sizeof(short));
}

/*****************************************************************************/

free_sarray(ptr,lh,uh,lw,uw)
short **ptr;
int lh,uh,lw,uw;
{
  free_array((void **) ptr,lh,uh,lw,uw,sizeof(short));
}

/*****************************************************************************/

_free_svector(ptr)
short *ptr;
{
  _free_vect((void *) ptr,sizeof(short));
}

/*****************************************************************************/

free_svector(ptr,l,h)
short *ptr;
int l,h;
{
  free_vect((void *) ptr,l,h,sizeof(short));
}

/*****************************************************************************/

