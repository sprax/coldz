/*************************************************
 jlm2ps.c    David Beymer, S.R.Lines    -93.5.22
 *************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "image.h"

int main (argc, argv) int argc; char *argv[];
{
  char *input, *output;
  int zoom;
  image_t *image;

  if (argc != 4) {
    fprintf (stderr, "usage: %s input zoom output\n", argv[0]);
    return(-1);
  }
  input = argv[1];
  zoom = atoi (argv[2]);
  output = argv[3];
  image = read_jlm (input);
  return( write_postscript (image, output, 0, zoom) );
}


