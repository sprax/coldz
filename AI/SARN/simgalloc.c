/* Short image allocation routines */

#include "imgdec.h"
#include "imgmacro.h"

extern int Image_border;

/*****************************************************************************/

alloc_simage(image)
SIMAGE *image;
{
   int w=image->w, h=image->h;

   if (!(image->ptr = (short **) alloc_image(w,h,sizeof(short),Image_border))) {
     perror("alloc_simage");
     die("alloc_simage: die'd trying to allocate short image %d x %d\n",w,h);
   }
}

/*****************************************************************************/

free_simage(img)
SIMAGE img;
{
   free_image((void **) img.ptr, sizeof(short), Image_border);
}

/*****************************************************************************/


