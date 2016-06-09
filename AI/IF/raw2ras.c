/*****************************************************************************
 raw2ras.c
 convert raw format to sun rasterfile format
 *****************************************************************************/
#include <stdio.h>
#include "image.h"

int main (argc, argv) int argc; char *argv[];
{
  int width, height;
  image_t *image;

  if (argc != 5) {
    fprintf (stderr, "usage: %s input width height output\n", argv[0]);
    return(-1);
  }
  width  = atoi(argv[2]);
  height = atoi(argv[3]);
  if ( ! (image = read_raw (argv[1], width, height))) {
    fprintf(stderr,"%s: error opening file %s for input.\n",argv[0],argv[1]);
    return(-1);
  }
  return( write_rast (image, argv[4], 0) );
}


