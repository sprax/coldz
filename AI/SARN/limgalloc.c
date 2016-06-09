/* Long image allocation routines */

#include "imgdec.h"
#include "imgmacro.h"

extern int Image_border;

/*****************************************************************************/

alloc_limage(image)
LIMAGE *image;
{
   int w=image->w, h=image->h;

   if (!(image->ptr = (long **) alloc_image(w,h,sizeof(long),Image_border))) {
     perror("alloc_limage");
     die("alloc_limage: die'd trying to allocate long image %d x %d\n",w,h);
   }
}

/*****************************************************************************/

free_limage(img)
LIMAGE img;
{
   free_image((void **) img.ptr, sizeof(long), Image_border);
}

/*****************************************************************************/


