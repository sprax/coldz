/* Byte array allocation routines */

typedef unsigned char byte;

byte **alloc_barray();
byte *alloc_bvector();

/*****************************************************************************/

byte **_alloc_barray(h,w)
int h,w;
{
  return(alloc_barray(0,h-1,0,w-1));
}

/*****************************************************************************/

byte **alloc_barray(lh,uh,lw,uw)
int lh,uh,lw,uw;
{
   register byte **ptr;

   if (!(ptr = (byte **) alloc_array(lh,uh,lw,uw,sizeof(byte)))) {
     perror("alloc_barray");
     die("alloc_barray: die'd trying to allocate byte array [%d:%d, %d:%d]\n",lh,uh,lw,uw);
   }
   return ptr;
}

/*****************************************************************************/

byte *_alloc_bvector(dim)
int dim;
{
  return(alloc_bvector(0,dim-1));
}

/*****************************************************************************/

byte *alloc_bvector(l,h)
int l,h;
{
   byte *ptr;

   if (!(ptr = (byte *) alloc_vector(l,h,sizeof(byte)))) {
     perror("alloc_bvector");
     die("alloc_bvector: die'd trying to allocate byte vector [%d:%d]\n",l,h);
   }
   return ptr;
}

/*****************************************************************************/

_free_barray(ptr)
byte **ptr;
{
  _free_array((void **) ptr, sizeof(byte));
}

/*****************************************************************************/

free_barray(ptr,lh,uh,lw,uw)
byte **ptr;
int lh,uh,lw,uw;
{
  free_array((void **) ptr,lh,uh,lw,uw,sizeof(byte));
}

/*****************************************************************************/

_free_bvector(ptr)
byte *ptr;
{
  _free_vect((void *) ptr,sizeof(byte));
}

/*****************************************************************************/

free_bvector(ptr,l,h)
byte *ptr;
int l,h;
{
  free_vect((void *) ptr,l,h,sizeof(byte));
}

/*****************************************************************************/

