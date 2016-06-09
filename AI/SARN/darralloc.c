/* Double array allocation routines */

double **alloc_darray();
double *alloc_dvector();

/*****************************************************************************/

double **_alloc_darray(h,w)
int h,w;
{
  return(alloc_darray(0,h-1,0,w-1));
}

/*****************************************************************************/

double **alloc_darray(lh,uh,lw,uw)
int lh,uh,lw,uw;
{
   register double **ptr;

   if (!(ptr = (double **) alloc_array(lh,uh,lw,uw,sizeof(double)))) {
     perror("alloc_darray");
     die("alloc_darray: die'd trying to allocate double array [%d:%d, %d:%d]\n",lh,uh,lw,uw);
   }
   return ptr;
}

/*****************************************************************************/

double *_alloc_dvector(dim)
int dim;
{
  return(alloc_dvector(0,dim-1));
}

/*****************************************************************************/

double *alloc_dvector(l,h)
int l,h;
{
   double *ptr;

   if (!(ptr = (double *) alloc_vector(l,h,sizeof(double)))) {
     perror("alloc_dvector");
     die("alloc_dvector: die'd trying to allocate double vector [%d:%d]\n",l,h);
   }
   return ptr;
}

/*****************************************************************************/

_free_darray(ptr)
double **ptr;
{
  _free_array((void **) ptr, sizeof(double));
}

/*****************************************************************************/

free_darray(ptr,lh,uh,lw,uw)
double **ptr;
int lh,uh,lw,uw;
{
  free_array((void **) ptr,lh,uh,lw,uw,sizeof(double));
}

/*****************************************************************************/

_free_dvector(ptr)
double *ptr;
{
  _free_vect((void *) ptr,sizeof(double));
}

/*****************************************************************************/

free_dvector(ptr,l,h)
double *ptr;
int l,h;
{
  free_vect((void *) ptr,l,h,sizeof(double));
}

/*****************************************************************************/

