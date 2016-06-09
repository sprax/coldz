/******************************************************************************
io_sgi.h    S.R.Lines (spraxlo@ai.mit.edu)    93.10.2
#include        <beymer/image.h>
******************************************************************************/


image_t *
read_sgi(char *path, int flags);

void
write_sgi(image_t *image, char *path);

void
write_sgiz(image_t *bimage[4], char *path, int chan);


int
read_sgiz(image_t **ims, char *path);

/* gutil/row.c */

void rgbrowtobw(unsigned short *rbuf, unsigned short *gbuf
              , unsigned short *bbuf, unsigned short *obuf, int n);


