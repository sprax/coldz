/******************************************************************************
 any2sgi.c    S.R.Lines    93.5.22
 *****************************************************************************/
#include	<SGI/image.h>
#include	<beymer/image.h>
#include	<beymer/io.h>
#include	<beymer/io_sgi.h>

void main (int argc, char **argv)
{
  image_t *image;

  if (argc != 3) {
    fprintf (stderr, "usage: %s input output\n", argv[0]);
    return;
  }
  if (! (image = read_image (argv[1]))) {
    fprintf(stderr,"%s: error reading %s.\n",argv[0],argv[1]);
    return;
  }
  write_sgi(image, argv[2]);
}


