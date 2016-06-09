/* Byte image allocation routines */

#include "imgdec.h"
#include "imgmacro.h"

extern int Image_border;

/*****************************************************************************/

void alloc_bimage(BIMAGE *image)
{
   int w=image->w, h=image->h;

   if (!(image->ptr = (byte **) alloc_image(w,h,sizeof(byte),Image_border))) {
     perror("alloc_bimage");
     die("alloc_bimage: die'd trying to allocate byte image %d x %d\n",w,h);
   }
}

/*****************************************************************************/

void free_bimage(BIMAGE img)
{
   free_image((char **) img.ptr, sizeof(byte), Image_border);
}

/*****************************************************************************/


