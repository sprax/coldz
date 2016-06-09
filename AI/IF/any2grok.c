/*************************************************
 2grok.c    David Beymer, S.R.Lines    -93.5.22
 *************************************************/
#include <stdio.h>
#include "beymer/image.h"

main (argc, argv)
int argc;
char *argv[];
{
  image_t *image;

  if (argc != 3) {
    fprintf (stderr, "usage: %s input output\n", argv[0]);
    return(-1);
  }
  if ((image = read_image (argv[1])) == NULL) {
    fprintf(stderr,"%s:  error opening file %s for input.\n",argv[0],argv[1]);
    return(-1);
  }
  return( write_grok(image, argv[2]) );
}


