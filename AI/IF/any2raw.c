/*************************************************
 2raw.c    David Beymer, S.R.Lines    -93.5.22
 *************************************************/
#include <stdio.h>
#include <beymer/image.h>
#include <beymer/io.h>

main (argc, argv)
int argc;
char *argv[];
{
  image_t *image;

  if (argc != 3) {
    fprintf (stderr, "usage: %s input output\n", argv[0]);
    exit (-1);
  }
  if ((image = read_image (argv[1])) == NULL) {
    fprintf(stderr, "%s: error opening file %s for input.\n",argv[0],argv[1]);
    return(-1);
  } 
  return( write_raw (image, argv[2]) );
}


