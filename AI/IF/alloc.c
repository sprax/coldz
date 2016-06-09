/*****************************************************************************
 alloc.c -- creation and destruction of David Beymer's image_t   -93.6.6
 Not to be confused with Sarnoff's imgalloc.c, etc:
 Sarnoff uses alloc_image, David uses allocate_image.
 *****************************************************************************/

#include <stdio.h>
#include "beymer/image.h"
#include "beymer/morph.h"

image_t
*allocate_image (width, height, type)
int width, height, type;
{
  image_t *image;

  image = (image_t *) calloc (1, sizeof(image_t));
  image->width = width;
  image->height = height;
  image->type = type;
  if (allocate_image_pixrect (image, width, height, type_bits_per_pixel (type)))
    fprintf(stderr,"allocate_image in %s: bad allocate_image_pixrect\n"
	,__FILE__);
  allocate_line_pointers (image);
  return image;
}

image_t
*allocate_image_like (image)
image_t *image;
{
  return allocate_image (image->width, image->height, image->type);
}

image_t
*copy_image (image)
image_t *image;
{
  image_t *copy;

  copy = allocate_image_like (image);
  op_image_image (copy, image, COPY);
  return copy;
}

void
allocate_line_pointers (image)
image_t *image;
{
  int height, y;
  char **line, *line_ptr;

  height = image->height;
  line = (char **) calloc (height, sizeof (char *));
  image->image = line;
  line_ptr = image->mem;
  for (y = 0; y < height; y++) {
    line[y] = line_ptr;
    line_ptr += image->bytes_per_line;
  }
}

/*
 * image destruction
 */

deallocate_image (image)
image_t *image;
{
  deallocate_image_pixrect (image);
  free (image->image);
  free (image);
}

type_bits_per_pixel (type)
int type;
{
  switch (type) {
  case IMG_1BIT:
    return 1;
  case IMG_U8BIT:
  case IMG_S8BIT:
    return 8;
  case IMG_S32BIT:
  case IMG_FLOAT:
    return 32;
  default:
    fprintf (stderr, "type_bits_per_pixel:  unsupported type.\n");
  }
  return -1;
}

