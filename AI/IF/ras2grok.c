#include <stdio.h>
#include "image.h"

main (argc, argv)
int argc;
char *argv[];
{
  image_t *image;

  if (argc != 3) {
    fprintf (stderr, "usage: %s input output\n", argv[0]);
    exit (-1);
  }
  if ((image = read_rast (argv[1])) == NULL) {
    fprintf (stderr, "%s:  error opening file %s for input.\n", argv[0], argv[1]);
  } else {
    write_grok (image, argv[2]);
  }
}


