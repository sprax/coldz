/* 
 rgb2ppm.c
 */
#include <sys/types.h>
#include <stdio.h>
#include "image.h"

#define PPM_MAGIC_NUMBER		"P6"

main (argc, argv)
int argc;
char *argv[];
{
  u_char *red_ptr, *green_ptr, *blue_ptr;
  int x, y;
  image_t *red, *green, *blue;
  FILE *fp;

  if (argc != 5) {
    fprintf (stderr, "usage: %s <red> <green> <blue> <output>\n", argv[0]);
    exit (-1);
  }
  if ((red = read_image (argv[1])) == NULL) {
    fprintf (stderr, "%s: error opening %s\n", argv[0], argv[1]);
    exit (-1);
  }
  if ((green = read_image (argv[2])) == NULL) {
    fprintf (stderr, "%s: error opening %s\n", argv[0], argv[2]);
    exit (-1);
  }
  if ((blue = read_image (argv[3])) == NULL) {
    fprintf (stderr, "%s: error opening %s\n", argv[0], argv[3]);
    exit (-1);
  }
  if ((fp = fopen (argv[4], "w")) == NULL) {
    fprintf (stderr, "%s: error opening %s for output.\n", argv[0], argv[4]);
    exit (-1);
  }
  fprintf (fp, "%s\n%d %d\n%d\n", PPM_MAGIC_NUMBER, red->width, red->height,
	   255);
  for (y = 0; y < red->height; y++) {
    red_ptr = (u_char *) red->image[y];
    green_ptr = (u_char *) green->image[y];
    blue_ptr = (u_char *) blue->image[y];
    for (x = 0; x < red->width; x++) {
      putc (*red_ptr, fp);	red_ptr++;
      putc (*green_ptr, fp);	green_ptr++;
      putc (*blue_ptr, fp);	blue_ptr++;
    }
  }
  fclose (fp);
}

