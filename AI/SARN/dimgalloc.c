/* Double image allocation routines */

#include "imgdec.h"
#include "imgmacro.h"

extern int Image_border;

/*****************************************************************************/

alloc_dimage(image)
DIMAGE *image;
{
   int w=image->w, h=image->h;

   if (!(image->ptr = (double **) alloc_image(w,h,sizeof(double),Image_border))) {
     perror("alloc_dimage");
     die("alloc_dimage: die'd trying to allocate double image %d x %d\n",w,h);
   }
}

/*****************************************************************************/

free_dimage(img)
DIMAGE img;
{
   free_image((void **) img.ptr, sizeof(double), Image_border);
}

/*****************************************************************************/

