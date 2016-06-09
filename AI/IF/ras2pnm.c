#include <stdio.h>
#include "image.h"

main (argc, argv)
int argc;
char *argv[];
{
  char *input, *output;
  int type;
  image_t *image, *dest;

  if (argc != 4) {
    fprintf (stderr, "usage: %s input type output\n", argv[0]);
    exit (1);
  }
  input = argv[1];
  type = atoi (argv[2]);
  output = argv[3];
  image = read_image (input);
  switch (type) {
  case 1:
    /* simple grey level image */
    if (image->type != IMG_U8BIT) {
      fprintf (stderr, "%s: expected 8 bit image.\n", argv[0]);
      exit(1);
    }
    dest = image;
    break;
  case 2:
    /* simple binary image */
    if (image->type != IMG_1BIT) {
      fprintf (stderr, "%s: expected binary image.\n", argv[0]);
      exit(1);
    }
    dest = image;
    break;
  case 3:
    /* grey levels and color */
    if (image->type != IMG_U8BIT) {
      fprintf (stderr, "%s: expected 8 bit image.\n", argv[0]);
      exit(1);
    }
    dest = allocate_image (image->width, image->height, IMG_U8BIT);
    mix_grey_color (dest, image);
    break;
  case 4:
    /* color --> binary */
    if (image->type != IMG_U8BIT) {
      fprintf (stderr, "%s: expected 8 bit image.\n", argv[0]);
      exit(1);
    }
    dest = allocate_image (image->width, image->height, IMG_1BIT);
    threshold_grey (dest, image, 127);
    break;
  }
  write_portable (dest, output);
}


