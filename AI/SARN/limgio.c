/* Long image I/O routines from files */

#define CHECK_FOR_OLD_IMAGE_FORMAT
#include "imgdec.h"
#include "imghdr.h"

/******************************************************************************/

read_limage(filename,image)
char *filename;
LIMAGE *image;
{
   FILE *stream;

   if ((stream=fopen(filename,"r")) == NULL)
      return(0);
   read_limgstr(stream,image);
   fclose(stream);
   return(1);
}

/*****************************************************************************/

write_limage(filename,image)
char *filename;
LIMAGE image;
{
  FILE *stream;
  int filetype();

  if ((stream=fopen(filename,"w")) == NULL)
    die("write_bimage: Couldn't open %s\n",filename);
  switch (filetype(filename)) {
    case UBYTE:
      {  BIMAGE scr;
         scr.w = image.w;  scr.h = image.h;  alloc_bimage(&scr);
         ltob(scr,image);
         write_bimgstr(stream,scr);
         free_bimage(scr);
      }
      break;
    case FLOAT: 
      {  FIMAGE scr;
         scr.w = image.w;  scr.h = image.h;  alloc_fimage(&scr);
         ltof(scr,image);
         write_fimgstr(stream,scr);
         free_fimage(scr);
      }
      break;
    case SHORT: 
      {  SIMAGE scr;
         scr.w = image.w;  scr.h = image.h;  alloc_simage(&scr);
         ltos(scr,image);
         write_simgstr(stream,scr);
         free_simage(scr);
      }
      break;
    case LONG: 
    case DUNNO:
      write_limgstr(stream,image);
      break;
    default:
      die("write_limage: file %s is of unknown type\n",filename);
  }
  fclose(stream);
}

/******************************************************************************/

read_limgstr(stream,image)
FILE *stream;
LIMAGE *image;
{
  IMAGE_HEADER hdr;
   char *typestr();

  if (fread(&hdr,sizeof(hdr),1,stream) != 1)
    die("read_limgstr: end of file on stream\n");
#ifdef CHECK_FOR_OLD_IMAGE_FORMAT
  if (!hdr.width)
    die("read_bimgstr(): Input file seems to be in the old image format\n     Use oip2nip to update file to new image format\n");
#endif
  image->w = hdr.width;
  image->h = hdr.height;
  alloc_limage(image);
  switch (hdr.type) {
    case UBYTE:
      {  BIMAGE scr;
         scr.w = image->w;  scr.h = image->h;  alloc_bimage(&scr);
         _read_bimgstr(stream,scr);
         btol(*image,scr);
         free_bimage(scr);
      }
      break;
    case FLOAT: 
      {  FIMAGE scr;
         scr.w = image->w;  scr.h = image->h;  alloc_fimage(&scr);
         _read_fimgstr(stream,scr);
         ftol(*image,scr);
         free_fimage(scr);
      }
      break;
    case SHORT: 
      {  SIMAGE scr;
         scr.w = image->w;  scr.h = image->h;  alloc_simage(&scr);
         _read_simgstr(stream,scr);
         stol(*image,scr);
         free_simage(scr);
      }
      break;
    case LONG: 
    case DUNNO:
      _read_limgstr(stream,*image);
      break;
    case BYTE: 
    case USHORT:
      die("read_limgstr: Unsupported image type %s\n",typestr(hdr.type));
    default:
      die("read_limgstr: Unknown image type %d\n",hdr.type);
  }
}

/*****************************************************************************/

write_limgstr(stream,image)
FILE *stream;
LIMAGE image;
{
   register long **p=image.ptr,**fp;
   register int w=image.w, h=image.h;
   IMAGE_HEADER hdr;

   bzero((char *)&hdr, sizeof(hdr));
   hdr.width = w;  hdr.height = h;  hdr.type = LONG;  hdr.depth = 1;
   fwrite(&hdr,sizeof(hdr),1,stream);
   for (fp=p+h; p < fp; p++)
     if (fwrite(*p,sizeof(**p),w,stream) == 0)
       die("write_limgstr: Error while writing to stream\n");
}

/*****************************************************************************/

_read_limgstr(stream,image)
FILE *stream;
LIMAGE image;
{
   register long **p=image.ptr,**fp;
   register int w=image.w, h=image.h;

   for (fp=p+h; p < fp; p++)
     if (fread(*p,sizeof(**p),w,stream) != w)
       die("_read_limgstr: End of file on stream\n");
}

