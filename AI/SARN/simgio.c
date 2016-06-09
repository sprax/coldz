/* Short image I/O routines from files */

#define CHECK_FOR_OLD_IMAGE_FORMAT
#include "imgdec.h"
#include "imghdr.h"

/******************************************************************************/

read_simage(filename,image)
char *filename;
SIMAGE *image;
{
   FILE *fopen(),*stream;

   if ((stream=fopen(filename,"r")) == NULL)
      return(0);
   read_simgstr(stream,image);
   fclose(stream);
   return(1);
}

/*****************************************************************************/

write_simage(filename,image)
char *filename;
SIMAGE image;
{
  FILE *stream;
  int filetype();

  if ((stream=fopen(filename,"w")) == NULL)
    die("write_bimage: Couldn't open %s\n",filename);
  switch (filetype(filename)) {
    case UBYTE:
      {  BIMAGE scr;
         scr.w = image.w;  scr.h = image.h;  alloc_bimage(&scr);
         stob(scr,image);
         write_bimgstr(stream,scr);
         free_bimage(scr);
      }
      break;
    case FLOAT: 
      {  FIMAGE scr;
         scr.w = image.w;  scr.h = image.h;  alloc_fimage(&scr);
         stof(scr,image);
         write_fimgstr(stream,scr);
         free_fimage(scr);
      }
      break;
    case SHORT: 
    case DUNNO:
      write_simgstr(stream,image);
      break;
    case LONG: 
      {  LIMAGE scr;
         scr.w = image.w;  scr.h = image.h;  alloc_limage(&scr);
         stol(scr,image);
         write_limgstr(stream,scr);
         free_limage(scr);
      }
      break;
    default:
      die("write_simage: file %s is of unknown type\n",filename);
  }
  fclose(stream);
}

/******************************************************************************/

read_simgstr(stream,image)
FILE *stream;
SIMAGE *image;
{
  IMAGE_HEADER hdr;
  char *typestr();

  if (fread(&hdr,sizeof(hdr),1,stream) != 1)
    die("read_simgstr: end of file on stream\n");
#ifdef CHECK_FOR_OLD_IMAGE_FORMAT
  if (!hdr.width)
    die("read_simgstr(): Input file seems to be in the old image format\n     Use oip2nip to update file to new image format\n");
#endif
  image->w = hdr.width;
  image->h = hdr.height;
  alloc_simage(image);
  switch (hdr.type) {
    case UBYTE:
      {  BIMAGE scr;
         scr.w = image->w;  scr.h = image->h;  alloc_bimage(&scr);
         _read_bimgstr(stream,scr);
         btos(*image,scr);
         free_bimage(scr);
      }
      break;
    case FLOAT: 
      {  FIMAGE scr;
         scr.w = image->w;  scr.h = image->h;  alloc_fimage(&scr);
         _read_fimgstr(stream,scr);
         ftos(*image,scr);
         free_fimage(scr);
      }
      break;
    case SHORT: 
    case DUNNO:
      _read_simgstr(stream,*image);
      break;
    case LONG: 
      {  LIMAGE scr;
         scr.w = image->w;  scr.h = image->h;  alloc_limage(&scr);
         _read_limgstr(stream,scr);
         ltos(*image,scr);
         free_limage(scr);
      }
      break;
    case BYTE: 
    case USHORT:
      die("read_simgstr: Unsupported image type %s\n",typestr(hdr.type));
    default:
      die("read_simgstr: Unknown image type %d\n",hdr.type);
  }
}

/*****************************************************************************/

write_simgstr(stream,image)
FILE *stream;
SIMAGE image;
{
   register short **p=image.ptr,**fp;
   register int w=image.w, h=image.h;
   IMAGE_HEADER hdr;

   bzero((char *)&hdr, sizeof(hdr));
   hdr.width = w;  hdr.height = h;  hdr.type = SHORT;  hdr.depth = 1;
   fwrite(&hdr,sizeof(hdr),1,stream);
   for (fp=p+h; p < fp; p++)
     if (fwrite(*p,sizeof(**p),w,stream) == 0)
       die("write_simgstr: Error while writing to stream\n");
}

/*****************************************************************************/

_read_simgstr(stream,image)
FILE *stream;
SIMAGE image;
{
   register short **p=image.ptr,**fp;
   register int w=image.w, h=image.h;

   for (fp=p+h; p < fp; p++)
     if (fread(*p,sizeof(**p),w,stream) != w)
       die("_read_simgstr: End of file on stream\n");
}

