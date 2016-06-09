/* Float image allocation routines */

#include "imgdec.h"
#include "imgmacro.h"

extern int Image_border;

/*****************************************************************************/

void alloc_fimage(FIMAGE *image)
{
   int w=image->w, h=image->h;

   if (!(image->ptr = (float **) alloc_image(w,h,sizeof(float),Image_border))) {
     perror("alloc_fimage");
     die("alloc_fimage: die'd trying to allocate float image %d x %d\n",w,h);
   }
}

/*****************************************************************************/

void free_fimage(FIMAGE img)
{
   free_image((char **) img.ptr, sizeof(float), Image_border);
}

/*****************************************************************************/


