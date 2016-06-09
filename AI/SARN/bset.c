/* Set a byte image to a constant value */
#include "imgdec.h"

bset(image,c)
BIMAGE image;
byte c;
{
  register byte *p,*fp;
  register byte **img=image.ptr,**fin;
	
  for (fin=img+image.h; img < fin; img++)
    for (p= *img, fp=p+image.w; p < fp; *p++ = c)
      ;
}











