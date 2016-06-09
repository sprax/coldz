#include "imgdec.h"

dctimg(in,out,xsize,ysize,flag)
FIMAGE in,out;
int xsize,ysize,flag;
{
  FIMAGE block;
  int xoff,yoff,iblk,jblk,nblkx=in.w/xsize,nblky=in.h/ysize;

  block.w = xsize;   block.h = ysize;
  alloc_fimage(&block);

  for (jblk=0,yoff=0; jblk < nblky; jblk++,yoff+=block.h) {
    for (iblk=0,xoff=0; iblk < nblkx; iblk++,xoff+=block.w) {
      getblock(block,in,xoff,yoff);
      if (flag != -1)
        dct2d(block);
      else
        idct2d(block);
      putblock(block,out,xoff,yoff);
    }
  }

  free_fimage(block);
}

